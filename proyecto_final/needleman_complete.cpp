// compilar:
// g++ -std=c++17 -O2 -o needleman_complete needleman_complete.cpp
// ejemplos:
//      ./needleman_complete -c gene.fna gene2.fna
//      ./needleman_complete -w gene.fna
//      ./needleman_complete -c gene.fna gene2.fna -5

#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <string>
#include <map>
#include <algorithm>
#include <cctype>
#include <cstdlib>
#include <iomanip>

using namespace std;

// CONFIGURACIÓN GENERAL

const int MATCH_DEF = 1;
const int MISMATCH_DEF = -1;
const int GAP_DEF = -2;

// FUNCIONES PARA LEER FASTA

vector<string> obtenerCabeceras(const string &nombreArchivo) {
    vector<string> cabeceras;
    ifstream archivo(nombreArchivo);

    if (!archivo.is_open()) {
        cerr << "error: no pude abrir el archivo fasta: " << nombreArchivo << "\n";
        return cabeceras;
    }

    string linea;
    while (getline(archivo, linea)) {
        if (!linea.empty() && linea[0] == '>') {
            cabeceras.push_back(linea);
        }
    }
    archivo.close();
    return cabeceras;
}

string extraerSecuencia(const string &nombreArchivo, int indice) {
    ifstream archivo(nombreArchivo);
    if (!archivo.is_open()) {
        cerr << "error al abrir fasta: " << nombreArchivo << "\n";
        return "";
    }

    int contadorCab = 0;
    string linea;
    bool leyendo = false;
    string secuencia = "";

    while (getline(archivo, linea)) {
        if (!linea.empty() && linea[0] == '>') {
            contadorCab++;

            if (contadorCab == indice) {
                leyendo = true;
                continue;
            }
            else if (leyendo) {
                break;
            }
        }

        if (leyendo) {
            for (char c : linea) {
                if (!isspace((unsigned char)c)) {
                    secuencia.push_back(toupper(c));
                }
            }
        }
    }

    archivo.close();
    return secuencia;
}

string leerPrimeraSecuencia(const string &nombreArchivo) {
    ifstream archivo(nombreArchivo);
    if (!archivo.is_open()) {
        return "";
    }

    string linea;
    string secuencia = "";
    bool leer = false;

    while (getline(archivo, linea)) {
        if (!linea.empty() && linea[0] == '>') {
            leer = true;
            continue;
        }

        if (leer) {
            for (char c : linea) {
                if (!isspace((unsigned char)c)) {
                    secuencia.push_back(toupper(c));
                }
            }
        }
    }

    archivo.close();
    return secuencia;
}

// RESULTADO DE NEEDLEMAN-WUNSCH

struct ResultadoNW {
    vector<vector<int>> matrizScore;
    vector<vector<int>> matrizDireccion;
    int puntajeFinal;
    string alineada1;
    string alineada2;
};

// ALGORITMO NEEDLEMAN-WUNSCH 

ResultadoNW needlemanWunsch(
        const string &s1,
        const string &s2,
        int penalidadGAP,
        int matchDef,
        int mismatchDef
) {
    int m = s1.size();
    int n = s2.size();

    vector<vector<int>> dp(n+1, vector<int>(m+1, 0));
    vector<vector<int>> dir(n+1, vector<int>(m+1, 0));

    dp[0][0] = 0;
    dir[0][0] = -1;

    for (int j = 1; j <= m; j++) {
        dp[0][j] = dp[0][j-1] + penalidadGAP;
        dir[0][j] = 2;
    }

    for (int i = 1; i <= n; i++) {
        dp[i][0] = dp[i-1][0] + penalidadGAP;
        dir[i][0] = 1;
    }

    for (int i = 1; i <= n; i++) {
        for (int j = 1; j <= m; j++) {

            char letra1 = s1[j-1];
            char letra2 = s2[i-1];

            int scoreDiag = dp[i-1][j-1] + (letra1 == letra2 ? matchDef : mismatchDef);
            int scoreUp   = dp[i-1][j] + penalidadGAP;
            int scoreLeft = dp[i][j-1] + penalidadGAP;

            if (scoreDiag >= scoreUp && scoreDiag >= scoreLeft) {
                dp[i][j] = scoreDiag;
                dir[i][j] = 0;
            }
            else if (scoreUp >= scoreLeft) {
                dp[i][j] = scoreUp;
                dir[i][j] = 1;
            }
            else {
                dp[i][j] = scoreLeft;
                dir[i][j] = 2;
            }
        }
    }

    string alineada1 = "";
    string alineada2 = "";
    int i = n, j = m;

    while (i > 0 || j > 0) {

        if (i > 0 && j > 0 && dir[i][j] == 0) {
            alineada1.push_back(s1[j-1]);
            alineada2.push_back(s2[i-1]);
            i--; j--;
        }
        else if (j > 0 && (i == 0 || dir[i][j] == 2)) {
            alineada1.push_back(s1[j-1]);
            alineada2.push_back('-');
            j--;
        }
        else {
            alineada1.push_back('-');
            alineada2.push_back(s2[i-1]);
            i--;
        }
    }

    reverse(alineada1.begin(), alineada1.end());
    reverse(alineada2.begin(), alineada2.end());

    ResultadoNW R;
    R.matrizScore = dp;
    R.matrizDireccion = dir;
    R.puntajeFinal = dp[n][m];
    R.alineada1 = alineada1;
    R.alineada2 = alineada2;

    return R;
}

// GUARDAR ALINEAMIENTO

void guardarAlineamientoTXT(
        const string &a1,
        const string &a2,
        const string &nombre = "Alineamiento.txt",
        int ancho = 60
) {
    ofstream out(nombre);
    if (!out.is_open()) {
        cerr << "error: no pude crear archivo " << nombre << "\n";
        return;
    }

    long long L = max(a1.size(), a2.size());

    out << "alineamiento global (needleman-wunsch)\n\n";

    for (long long i = 0; i < L; i += ancho) {

        string f1 = (i < (long long)a1.size()) ? a1.substr(i, ancho) : "";
        string f2 = (i < (long long)a2.size()) ? a2.substr(i, ancho) : "";

        out << "secuencia 1: " << f1 << "\n";

        string mid = "";
        for (size_t k = 0; k < f1.size() || k < f2.size(); k++) {
            char c1 = (k < f1.size()) ? f1[k] : ' ';
            char c2 = (k < f2.size()) ? f2[k] : ' ';
            mid.push_back((c1 == c2 && c1 != '-') ? '|' : ' ');
        }

        out << "             " << mid << "\n";
        out << "secuencia 2: " << f2 << "\n\n";
    }

    out.close();
    cout << "archivo guardado: " << nombre << "\n";
}

// GRAPHVIZ

void generarDOT(const string &a1, const string &a2) {

    int start, end;

    while (true) {
        cout << "intervalo para grafico (ej: 100 200, max 100 nt): ";

        if (!(cin >> start >> end)) {
            cin.clear();
            string basura;
            getline(cin, basura);
            cout << "entrada invalida\n";
            continue;
        }

        if (start < 0) {
            cout << "start no puede ser negativo\n";
            continue;
        }

        if (end <= start) {
            cout << "end debe ser mayor que start\n";
            continue;
        }

        if (end - start > 100) {
            cout << "maximo 100 nucleotidos\n";
            continue;
        }

        break;
    }

    ofstream dot("alineamiento.dot");
    if (!dot.is_open()) {
        cerr << "no se pudo crear alineamiento.dot\n";
        return;
    }

    dot << "digraph G {\n";
    dot << "  rankdir=LR;\n";
    dot << "  node [shape=plaintext, fontname=\"Courier New\"];\n";

    for (int i = start; i < end && i < (int)a1.size() && i < (int)a2.size(); i++) {

        string A = "A" + to_string(i);
        string B = "B" + to_string(i);

        dot << "  " << A << " [label=\"" << a1[i] << "\"];\n";
        dot << "  " << B << " [label=\"" << a2[i] << "\"];\n";

        string tipo = (a1[i] == a2[i] && a1[i] != '-') ? "match" : "gap";

        dot << "  " << A << " -> " << B << " [label=\"" << tipo << "\"];\n";
    }

    dot << "}\n";
    dot.close();
    cout << "archivo generado: alineamiento.dot\n";

    int ret = system("dot -Tpng alineamiento.dot -o alineamiento.png");

    if (ret == 0)
        cout << "imagen generada: alineamiento.png\n";
    else
        cout << "no se pudo generar .png (¿graphviz instalado?)\n";
}

void imprimirMatriz(const vector<vector<int>> &mat) {
    for (const auto &fila : mat) {
        for (int v : fila) {
            cout << setw(5) << v;
        }
        cout << "\n";
    }
}
double calcularPorcentajeIdentidad(const string &a1, const string &a2) {
    int iguales = 0;
    int total = 0;

    for (size_t i = 0; i < a1.size(); i++) {
        char c1 = a1[i];
        char c2 = a2[i];

        // contamos solo columnas válidas (al menos un nucleótido)
        if (c1 != '-' || c2 != '-') {
            total++;
            if (c1 == c2 && c1 != '-') {
                iguales++;
            }
        }
    }

    if (total == 0) return 0.0;

    return (100.0 * iguales) / total;
}


int main(int argc, char** argv) {

    if (argc < 3) {
        cerr << "uso:\n";
        cerr << "  " << argv[0] << " -c <fasta1> <fasta2> [gap]\n";
        cerr << "  " << argv[0] << " -w <fasta> [gap]\n";
        return 1;
    }

    string modo = argv[1];
    string archivo1, archivo2;
    bool mismoArchivo = false;

    if (modo == "-c") {
        if (argc < 4) {
            cerr << "faltan archivos para -c\n";
            return 1;
        }
        archivo1 = argv[2];
        archivo2 = argv[3];
    }
    else if (modo == "-w") {
        archivo1 = argv[2];
        archivo2 = archivo1;
        mismoArchivo = true;
    }
    else {
        cerr << "modo invalido, usa -c o -w\n";
        return 1;
    }

    int offset = (modo == "-c" ? 4 : 3);
    int gap = GAP_DEF;

    if (argc > offset) {
        try {
            gap = stoi(argv[offset]);
        } catch (...) {
            gap = GAP_DEF;
        }
    }

    cout << "leyendo cabeceras...\n";

    vector<string> cab1 = obtenerCabeceras(archivo1);
    if (cab1.empty()) {
        cerr << "error: no encontre cabeceras en " << archivo1 << "\n";
        return 1;
    }

    cout << "cabeceras de " << archivo1 << ":\n";
    for (size_t i = 0; i < cab1.size(); i++) {
        cout << "  " << i+1 << ") " << cab1[i] << "\n";
    }

    int idx1, idx2;

    if (mismoArchivo) {
        cout << "elige indice 1: ";
        while (!(cin >> idx1) || idx1 < 1 || idx1 > (int)cab1.size()) {
            cin.clear(); string basura; getline(cin, basura);
            cout << "indice invalido: ";
        }

        cout << "elige indice 2: ";
        while (!(cin >> idx2) || idx2 < 1 || idx2 > (int)cab1.size()) {
            cin.clear(); string basura; getline(cin, basura);
            cout << "indice invalido: ";
        }
    }
    else {
        vector<string> cab2 = obtenerCabeceras(archivo2);
        if (cab2.empty()) {
            cerr << "error: no encontre cabeceras en " << archivo2 << "\n";
            return 1;
        }

        cout << "\ncabeceras de " << archivo2 << ":\n";
        for (size_t i = 0; i < cab2.size(); i++) {
            cout << "  " << i+1 << ") " << cab2[i] << "\n";
        }

        cout << "elige indice para " << archivo1 << ": ";
        while (!(cin >> idx1) || idx1 < 1 || idx1 > (int)cab1.size()) {
            cin.clear(); string basura; getline(cin, basura);
            cout << "indice invalido: ";
        }

        cout << "elige indice para " << archivo2 << ": ";
        while (!(cin >> idx2) || idx2 < 1 || idx2 > (int)cab2.size()) {
            cin.clear(); string basura; getline(cin, basura);
            cout << "indice invalido: ";
        }
    }

    cout << "extrayendo secuencias...\n";

    string seq1 = extraerSecuencia(archivo1, idx1);
    string seq2 = extraerSecuencia(archivo2, idx2);

    if (seq1.empty() || seq2.empty()) {
        cerr << "error leyendo secuencias\n";
        return 1;
    }

    cout << "largo seq1: " << seq1.size() << "\n";
    cout << "largo seq2: " << seq2.size() << "\n";

    cout << "ejecutando needleman-wunsch...\n";

    ResultadoNW R = needlemanWunsch(
        seq1, seq2,
        gap, MATCH_DEF, MISMATCH_DEF
    );

    cout << "puntaje optimo = " << R.puntajeFinal << "\n";

    double identidad = calcularPorcentajeIdentidad(R.alineada1, R.alineada2);
    cout << fixed << setprecision(2);
    cout << "porcentaje de identidad = " << identidad << "%\n";


    guardarAlineamientoTXT(R.alineada1, R.alineada2);

    while (true) {
        cout << "\n--- menu ---\n";
        cout << "1) generar imagen graphviz\n";
        cout << "2) ver matrices dp y direccion\n";
        cout << "3) ver alineamiento en pantalla\n";
        cout << "4) salir\n";
        cout << "opcion: ";

        int op;  
        if (!(cin >> op)) {
            cin.clear();
            string basura; getline(cin, basura);
            continue;
        }

        if (op == 1) {
            generarDOT(R.alineada1, R.alineada2);
        }
        else if (op == 2) {
            cout << "\nmatriz DP:\n";
            imprimirMatriz(R.matrizScore);

            cout << "\nmatriz Direcciones:\n";
            imprimirMatriz(R.matrizDireccion);
        }
        else if (op == 3) {
            cout << "\nalineamiento:\n";
            cout << R.alineada1 << "\n";

            string mid = "";
            for (size_t i = 0; i < R.alineada1.size(); i++) {
                char c1 = R.alineada1[i];
                char c2 = R.alineada2[i];
                mid.push_back((c1 == c2 && c1 != '-') ? '|' : ' ');
            }
            cout << mid << "\n";
            cout << R.alineada2 << "\n";
        }
        else if (op == 4) {
            cout << "saliendo...\n";
            break;
        }
        else {
            cout << "opcion invalida\n";
        }
    }

    return 0;
}
