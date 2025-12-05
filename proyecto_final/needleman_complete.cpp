// needleman_complete.cpp
// compilar: g++ -std=c++17 -O2 -o needleman_complete needleman_complete.cpp//
//comparar la hemoglobina con la insulina 
// ./needleman_complete -c gene.fna gene2.fna

//comparar dos secuencias dentro de gene2.fna
// ./needleman_complete -w gene2.fna

//comparar dos secuencias dentro de gene.fna
//    ./needleman_complete -w gene.fna

//comparación simple (sin matriz csv, con gap por defecto)
//    ./needleman_complete -c gene.fna gene2.fna



#include <algorithm>   // para max, reverse, sort
#include <cctype>      // para isspace, toupper
#include <cstdlib>     // para system()
#include <fstream>     // para ifstream y ofstream
#include <iomanip>     // para setw
#include <iostream>    // para cout y cin
#include <sstream>     // para stringstream
#include <string>      // para usar string facilmente
#include <vector>      // para usar vector
#include <map>         // para map (matriz de puntuacion)
using namespace std;

// ------------------------------------------------------------------------------------
// configuracion por defecto del algoritmo
// ------------------------------------------------------------------------------------

// match por defecto (cuando se usan valores internos y no matriz csv)
const int DEFAULT_MATCH = 1;

// mismatch por defecto (cuando no se usa csv)
const int DEFAULT_MISMATCH = -1;

// gap por defecto (cuando no se pasa argumento extra)
const int DEFAULT_GAP = -2;

// ------------------------------------------------------------------------------------
// utilidades para leer archivos fasta (.fasta .fa .fna)
// ------------------------------------------------------------------------------------

// esta funcion abre un archivo fasta y devuelve todas las cabeceras encontradas
// esto es importante porque un archivo fasta puede tener muchas secuencias dentro
// cada una comienza con '>'
vector<string> listar_cabeceras_fasta(const string &ruta) {

    vector<string> cabeceras;            // vector donde guardaremos las cabeceras
    ifstream ifs(ruta);                  // abrimos archivo en modo lectura

    // si el archivo no abre, informamos error
    if (!ifs) {
        cerr << "error: no se pudo abrir " << ruta << endl;
        return cabeceras;
    }

    string linea;

    // recorremos linea por linea el archivo completo
    while (getline(ifs, linea)) {

        // si la linea comienza con '>' entonces es una cabecera
        if (!linea.empty() && linea[0] == '>') {
            cabeceras.push_back(linea);
        }
    }

    ifs.close(); // cerramos archivo
    return cabeceras;
}

// esta funcion extrae una secuencia del archivo fasta dada por un indice 1-based
// si en el archivo hay varias secuencias, esto permite elegir cual cargar
string extraer_secuencia_por_indice(const string &ruta, int indice) {

    ifstream ifs(ruta);
    if (!ifs) {
        cerr << "error: no se pudo abrir " << ruta << endl;
        return "";
    }

    string linea;
    int contador = 0;             // contador para saber en que cabecera estamos
    string seq = "";              // aqui acumulamos la secuencia extraida
    bool leyendo = false;         // indica si ya empezamos a leer la secuencia correcta

    // recorremos archivo completo
    while (getline(ifs, linea)) {

        // si la linea comienza con '>' significa que comienza una nueva secuencia
        if (!linea.empty() && linea[0] == '>') {

            contador++;           // incrementamos el contador de cabeceras

            // si esta cabecera coincide con el indice elegido
            if (contador == indice) {

                leyendo = true;   // comenzamos a leer la secuencia asociada
                continue;         // saltamos esta linea porque es la cabecera

            } else if (leyendo) {
                // si ya estabamos leyendo y llegamos a otra cabecera,
                // significa que la secuencia termino
                break;
            }
        }

        // si estamos leyendo la secuencia (lineas sin '>')
        if (leyendo) {

            // recorremos caracter por caracter para limpiar espacios
            for (char c : linea) {

                // ignoramos espacios y saltos de linea
                if (!isspace((unsigned char)c)) {

                    // guardamos el caracter en mayusculas para estandarizar
                    seq.push_back(toupper(c));
                }
            }
        }
    }

    ifs.close();
    return seq;      // devolvemos la secuencia extraida
}

// esta funcion sirve cuando sabemos que el archivo fasta solo tiene una secuencia
// es util para pruebas rapidas o cuando el archivo no tiene multiples cabeceras
string leer_primera_secuencia(const string &ruta) {

    ifstream ifs(ruta);
    if (!ifs) return "";

    string linea, seq = "";
    bool leyendo = false;

    // recorremos archivo completo
    while (getline(ifs, linea)) {

        // detectamos cabecera
        if (!linea.empty() && linea[0] == '>') {
            leyendo = true;
            continue;
        }

        if (leyendo) {

            // agregamos todos los caracteres validos sin espacios
            for (char c : linea) {
                if (!isspace((unsigned char)c)) {
                    seq.push_back(toupper(c));
                }
            }
        }
    }

    ifs.close();
    return seq;
}

// ------------------------------------------------------------------------------------
// lectura de matriz csv opcional (A,C,G,T,...)
// ------------------------------------------------------------------------------------
// el csv esperado tiene forma:
//
//   ,A,C,G,T
//   A, 1,-1,-1,-1
//   C,-1, 1,-1,-1
//   G,-1,-1, 1,-1
//   T,-1,-1,-1, 1
//
// un encabezado vacio o '-' en el primer campo
//
// devolvemos una estructura map<char,map<char,int>> para consultas rapidas
//
// ademas devolvemos un vector<char> con los encabezados (orden)

// esta funcion lee una matriz de csv de aminoacidos o nucleotidos
bool leer_matriz_csv(
    const string &ruta,
    map<char, map<char,int>> &matriz,
    vector<char> &encabezados_out
) {
    ifstream ifs(ruta);
    if (!ifs) {
        cerr << "error: no se pudo abrir matriz csv " << ruta << endl;
        return false;
    }

    string linea;

    // leemos la primera fila para obtener encabezados
    if (!getline(ifs, linea)) {
        cerr << "error: csv vacio\n";
        return false;
    }

    stringstream ss(linea);
    string campo;

    vector<char> encabezados;
    bool primera = true;

    // procesamos cada valor separado por ','
    while (getline(ss, campo, ',')) {

        // primer campo es vacio o '-', asi que se salta
        if (primera) {
            primera = false;
            continue;
        }

        // agregamos el primer caracter del campo como encabezado
        if (!campo.empty()) {
            encabezados.push_back(toupper(campo[0]));
        }
    }

    if (encabezados.empty()) {
        cerr << "error: encabezados invalidos en csv\n";
        return false;
    }

    // ahora procesamos las filas de datos
    while (getline(ifs, linea)) {

        if (linea.empty()) continue;

        stringstream ss2(linea);
        string fila_nombre;

        // extraemos la etiqueta de la fila (primer campo)
        if (!getline(ss2, fila_nombre, ',')) continue;

        char fila_char = toupper(fila_nombre[0]);
        int col = 0;
        string valor;

        // procesamos los valores numericos de la fila
        while (getline(ss2, valor, ',')) {

            // si hay mas columnas que encabezados, ignoramos exceso
            if (col >= (int)encabezados.size()) break;

            int punt = 0;

            try {
                punt = stoi(valor);
            } catch (...) {
                cerr << "advertencia: valor no valido en csv '" << valor << "' -> se asigna 0\n";
                punt = 0;
            }

            // asignamos puntaje a la matriz
            matriz[fila_char][encabezados[col]] = punt;
            col++;
        }
    }

    ifs.close();
    encabezados_out = encabezados;
    return true;
}

// esta funcion permite consultar la matriz csv de puntuacion
// si el par (fila,columna) no existe en la matriz cargada, usamos valores por defecto
int puntaje_matriz(
    const map<char, map<char,int>> &mat,
    char a,
    char b,
    int default_match,
    int default_mismatch
) {
    auto it = mat.find(a);
    if (it != mat.end()) {

        auto it2 = it->second.find(b);
        if (it2 != it->second.end()) {

            // si existe valor en la matriz csv lo devolvemos
            return it2->second;
        }
    }

    // si no existe, devolvemos match/mismatch por defecto
    return (a == b ? default_match : default_mismatch);
}

// ------------------------------------------------------------------------------------
// estructura para almacenar el resultado completo del algoritmo needleman-wunsch
// ------------------------------------------------------------------------------------

struct NWResult {

    vector<vector<int>> score;   // matriz dp de puntuacion final
    vector<vector<int>> direc;   // matriz de direcciones (0 diag, 1 up, 2 left)
    int optimal;                 // valor optimo final
    string aligned1;             // secuencia 1 alineada
    string aligned2;             // secuencia 2 alineada
};

// ------------------------------------------------------------------------------------
// algoritmo needleman-wunsch completo
// ------------------------------------------------------------------------------------

NWResult needleman_wunsch_compute(
    const string &s1,              // secuencia horizontal
    const string &s2,              // secuencia vertical
    const map<char, map<char,int>> &matriz_csv,
    bool use_csv,                  // indica si usar matriz csv o valores por defecto
    int gap_penalty,               // penalizacion de gap
    int default_match,
    int default_mismatch
) {
    int m = (int)s1.size();
    int n = (int)s2.size();

    // dp[i][j] almacenara el puntaje optimo al alinear s2[0..i-1] con s1[0..j-1]
    vector<vector<int>> dp(n+1, vector<int>(m+1, 0));

    // matriz de direcciones para reconstruccion del camino
    vector<vector<int>> dir(n+1, vector<int>(m+1, 0));

    // inicializamos posicion (0,0)
    dp[0][0] = 0;
    dir[0][0] = -1;     // -1 indica casilla inicial sin direccion

    // inicializamos la primera fila (solo gaps horizontales)
    for (int j = 1; j <= m; j++) {

        dp[0][j] = dp[0][j-1] + gap_penalty;
        dir[0][j] = 2;          // 2 = left
    }

    // inicializamos la primera columna (solo gaps verticales)
    for (int i = 1; i <= n; i++) {

        dp[i][0] = dp[i-1][0] + gap_penalty;
        dir[i][0] = 1;          // 1 = up
    }

    // llenado de la matriz dp principal
    for (int i = 1; i <= n; i++) {
        for (int j = 1; j <= m; j++) {

            char a = s1[j-1];   // base horizontal
            char b = s2[i-1];   // base vertical

            int score_diag;

            // calculamos puntaje de avanzar diagonalmente segun csv o defaults
            if (use_csv) {

                score_diag = dp[i-1][j-1] +
                              puntaje_matriz(matriz_csv, b, a, default_match, default_mismatch);

            } else {

                score_diag = dp[i-1][j-1] +
                              ((a == b) ? default_match : default_mismatch);
            }

            // puntaje si avanzamos desde arriba (gap en s1)
            int score_up   = dp[i-1][j] + gap_penalty;

            // puntaje si avanzamos desde la izquierda (gap en s2)
            int score_left = dp[i][j-1] + gap_penalty;

            // seleccionamos el maximo entre diagonal, arriba, izquierda
            if (score_diag >= score_up && score_diag >= score_left) {

                dp[i][j] = score_diag;
                dir[i][j] = 0;          // diagonal

            } else if (score_up >= score_left) {

                dp[i][j] = score_up;
                dir[i][j] = 1;          // up

            } else {

                dp[i][j] = score_left;
                dir[i][j] = 2;          // left
            }
        }
    }

    // --------------------------------------------------------------------------------
    // una vez construida la matriz dp y la matriz de direcciones dir,
    // procedemos al traceback para reconstruir el alineamiento optimo
    // --------------------------------------------------------------------------------

    int i = n;      // empezamos desde la ultima fila
    int j = m;      // y la ultima columna
    string a_align = "";     // aqui construiremos la secuencia alineada 1
    string b_align = "";     // aqui construiremos la secuencia alineada 2

    // recorremos mientras aun haya filas o columnas pendientes
    while (i > 0 || j > 0) {

        // caso diagonal (0): caracteres alineados uno frente al otro
        if (i > 0 && j > 0 && dir[i][j] == 0) {

            a_align.push_back(s1[j-1]);
            b_align.push_back(s2[i-1]);
            i--;
            j--;
        }

        // caso izquierda (2): gap en secuencia 2
        else if (j > 0 && (i == 0 || dir[i][j] == 2)) {

            a_align.push_back(s1[j-1]);    // caracter de s1 alineado
            b_align.push_back('-');       // gap en s2
            j--;
        }

        // caso arriba (1): gap en secuencia 1
        else {

            a_align.push_back('-');
            b_align.push_back(s2[i-1]);
            i--;
        }
    }

    // las cadenas quedaron invertidas por la forma en que se recorre traceback
    reverse(a_align.begin(), a_align.end());
    reverse(b_align.begin(), b_align.end());

    // construimos el resultado
    NWResult res;
    res.score = dp;
    res.direc = dir;
    res.optimal = dp[n][m];
    res.aligned1 = a_align;
    res.aligned2 = b_align;

    return res;
}

// ------------------------------------------------------------------------------------
// funcion para guardar el alineamiento en un archivo de texto bien formateado
// ------------------------------------------------------------------------------------

void guardar_alineamiento_txt(
    const string &a1,
    const string &a2,
    const string &nombre = "Alineamiento.txt",
    int ancho = 60
) {
    ofstream ofs(nombre);
    if (!ofs) {
        cerr << "error: no se pudo crear " << nombre << endl;
        return;
    }

    // obtenemos longitud maxima entre ambas secuencias alineadas
    long long len = max(a1.size(), a2.size());

    ofs << "alineamiento global (needleman-wunsch)\n\n";

    // mostramos bloques de tamaño "ancho"
    for (long long i = 0; i < len; i += ancho) {

        string frag1 = (i < (long long)a1.size()) ? a1.substr(i, ancho) : "";
        string frag2 = (i < (long long)a2.size()) ? a2.substr(i, ancho) : "";

        ofs << "secuencia 1: " << frag1 << "\n";

        // generamos linea intermedia con | para indicar matches
        string mid = "";
        for (size_t k = 0; k < frag1.size() || k < frag2.size(); k++) {

            char c1 = (k < frag1.size()) ? frag1[k] : ' ';
            char c2 = (k < frag2.size()) ? frag2[k] : ' ';

            if (c1 == c2 && c1 != '-' && c2 != '-') mid.push_back('|');
            else mid.push_back(' ');
        }

        ofs << "             " << mid << "\n";
        ofs << "secuencia 2: " << frag2 << "\n\n";
    }

    ofs.close();
    cout << "archivo guardado: " << nombre << endl;
}

// ------------------------------------------------------------------------------------
// funcion para generar archivo graphviz .dot
// ------------------------------------------------------------------------------------

void generar_dot_intervalo(const string &a1, const string &a2) {

    int start = 0;
    int end = 0;

    // pedimos al usuario un intervalo valido
    while (true) {

        cout << "ingresa intervalo para generar grafico (ej: 200 300). ";
        cout << "maximo 100 nucleotidos: ";

        if (!(cin >> start >> end)) {

            cin.clear();
            string junk;
            getline(cin, junk);
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
            cout << "intervalo demasiado grande\n";
            continue;
        }

        if (end > (int)a1.size() || end > (int)a2.size()) {
            cout << "advertencia: end excede longitud del alineamiento, se recortara\n";
        }

        break;
    }

    ofstream dot("alineamiento.dot");
    if (!dot) {
        cerr << "error: no se pudo crear alineamiento.dot\n";
        return;
    }

    dot << "digraph G {\n";
    dot << "  rankdir=LR;\n";
    dot << "  node [shape=plaintext, fontname=\"Courier New\"];\n";

    // creamos nodos y arcos para cada posicion del alineamiento
    for (int k = start; k < end && k < (int)a1.size() && k < (int)a2.size(); k++) {

        string idA = "A" + to_string(k);
        string idB = "B" + to_string(k);

        dot << "  " << idA << " [label=\"" << a1[k] << "\"];\n";
        dot << "  " << idB << " [label=\"" << a2[k] << "\"];\n";

        // match = bases iguales; gap = si alguna es '-'
        string label = (a1[k] != '-' && a2[k] != '-') ? "match" : "gap";

        dot << "  " << idA << " -> " << idB << " [label=\"" << label << "\"];\n";
    }

    dot << "}\n";
    dot.close();

    cout << "archivo generado: alineamiento.dot\n";

    // intentamos convertir a png usando graphviz (dot)
    int ret = system("dot -V > /dev/null 2>&1");

    if (ret == 0) {

        system("dot -Tpng alineamiento.dot -o alineamiento.png");
        cout << "imagen generada: alineamiento.png\n";

    } else {

        cout << "graphviz no esta instalado o no esta en el PATH\n";
    }
}

// ------------------------------------------------------------------------------------
// funcion para imprimir cualquier matriz numerica
// ------------------------------------------------------------------------------------

void imprimir_matriz_tipo(const vector<vector<int>> &mat) {

    for (const auto &fila : mat) {

        for (int v : fila) {

            cout << setw(5) << v;
        }

        cout << "\n";
    }
}

// ------------------------------------------------------------------------------------
// funcion principal (main)
// ------------------------------------------------------------------------------------

int main(int argc, char **argv) {

    //ios::sync_with_stdio(false);
    //cin.tie(nullptr);

    // si no hay suficientes argumentos, mostramos ayuda
    if (argc < 3) {

        cerr << "uso:\n";
        cerr << "  " << argv[0] << " -c <archivo1.fasta> <archivo2.fasta> [matriz.csv] [gap]\n";
        cerr << "  " << argv[0] << " -w <archivo.fasta> [matriz.csv] [gap]\n\n";

        cerr << "ejemplos:\n";
        cerr << "  " << argv[0] << " -c gene.fna otro_gene.fna scoring.csv -2\n";
        cerr << "  " << argv[0] << " -w gene.fna\n\n";

        return 1;
    }

    string modo = argv[1];
    string archivo1 = "";
    string archivo2 = "";
    bool mismo_archivo = false;

    // modo comparacion entre archivos
    if (modo == "-c") {

        if (argc < 4) {
            cerr << "faltan archivos para -c\n";
            return 1;
        }

        archivo1 = argv[2];
        archivo2 = argv[3];
    }

    // modo comparar dentro del mismo archivo
    else if (modo == "-w") {

        archivo1 = argv[2];
        archivo2 = archivo1;
        mismo_archivo = true;
    }

    else {

        cerr << "modo invalido, usa -c o -w\n";
        return 1;
    }

    // parametros opcionales: matriz csv y penalidad de gap
    string matriz_csv_path = "";
    int arg_offset = (modo == "-c") ? 4 : 3;

    if (argc > arg_offset) {
        matriz_csv_path = argv[arg_offset];
    }

    int gap_penalty = DEFAULT_GAP;

    if (argc > arg_offset + 1) {

        try {
            gap_penalty = stoi(argv[arg_offset + 1]);
        } catch (...) {
            gap_penalty = DEFAULT_GAP;
        }
    }

    // --------------------------------------------------------------------------------
    // listar cabeceras del primer archivo fasta
    // --------------------------------------------------------------------------------

    cout << "leyendo archivo fasta...\n";

    vector<string> cab1 = listar_cabeceras_fasta(archivo1);

    if (cab1.empty()) {

        cerr << "error: no se encontraron cabeceras en " << archivo1 << endl;
        return 1;
    }

    cout << "cabeceras en " << archivo1 << ":\n";
    for (size_t i = 0; i < cab1.size(); i++) {

        cout << "  " << (i + 1) << ") " << cab1[i] << "\n";
    }

    int idx1 = 1;
    int idx2 = 1;

    // modo -w: ambos índices vienen del mismo archivo
    if (mismo_archivo) {

        cout << "\nelige indice de la primera secuencia: ";

        while (!(cin >> idx1) || idx1 < 1 || idx1 > (int)cab1.size()) {

            cin.clear();
            string junk;
            getline(cin, junk);
            cout << "indice invalido, intenta de nuevo: ";
        }

        cout << "elige indice de la segunda secuencia: ";

        while (!(cin >> idx2) || idx2 < 1 || idx2 > (int)cab1.size()) {

            cin.clear();
            string junk;
            getline(cin, junk);
            cout << "indice invalido, intenta de nuevo: ";
        }
    }

    // modo -c: indices para cada archivo por separado
    else {

        vector<string> cab2 = listar_cabeceras_fasta(archivo2);

        if (cab2.empty()) {

            cerr << "error: no se encontraron cabeceras en " << archivo2 << endl;
            return 1;
        }

        cout << "\ncabeceras en " << archivo2 << ":\n";

        for (size_t i = 0; i < cab2.size(); i++) {

            cout << "  " << (i + 1) << ") " << cab2[i] << "\n";
        }

        cout << "\nelige indice de secuencia en " << archivo1 << ": ";

        while (!(cin >> idx1) || idx1 < 1 || idx1 > (int)cab1.size()) {

            cin.clear();
            string junk;
            getline(cin, junk);
            cout << "indice invalido: ";
        }

        cout << "elige indice de secuencia en " << archivo2 << ": ";

        while (!(cin >> idx2) || idx2 < 1 || idx2 > (int)cab2.size()) {

            cin.clear();
            string junk;
            getline(cin, junk);
            cout << "indice invalido: ";
        }
    }

    // --------------------------------------------------------------------------------
    // extraer las dos secuencias seleccionadas
    // --------------------------------------------------------------------------------

    cout << "extrayendo secuencias...\n";

    string seq1 = extraer_secuencia_por_indice(archivo1, idx1);
    string seq2 = extraer_secuencia_por_indice(archivo2, idx2);

    if (seq1.empty() || seq2.empty()) {

        cerr << "error: no se pudo extraer alguna de las secuencias\n";
        return 1;
    }

    cout << "longitud secuencia 1 = " << seq1.size() << "\n";
    cout << "longitud secuencia 2 = " << seq2.size() << "\n";

    // --------------------------------------------------------------------------------
    // si existe una matriz csv, la cargamos
    // --------------------------------------------------------------------------------

    map<char, map<char,int>> matriz_csv;
    vector<char> encabezados_csv;
    bool use_csv = false;

    if (!matriz_csv_path.empty()) {

        if (leer_matriz_csv(matriz_csv_path, matriz_csv, encabezados_csv)) {

            use_csv = true;

            cout << "matriz csv cargada. encabezados: ";
            for (char c : encabezados_csv) cout << c << " ";
            cout << "\n";
        }

        else {

            cout << "no se pudo leer csv, usando valores por defecto\n";
            use_csv = false;
        }
    }

    // --------------------------------------------------------------------------------
    // ejecutar algoritmo needleman-wunsch
    // --------------------------------------------------------------------------------

    cout << "ejecutando needleman-wunsch...\n";

    NWResult resultado = needleman_wunsch_compute(
        seq1,
        seq2,
        matriz_csv,
        use_csv,
        gap_penalty,
        DEFAULT_MATCH,
        DEFAULT_MISMATCH
    );

    cout << "valor optimo del alineamiento = " << resultado.optimal << "\n";

    // --------------------------------------------------------------------------------
    // guardar alineamiento en archivo
    // --------------------------------------------------------------------------------

    guardar_alineamiento_txt(resultado.aligned1, resultado.aligned2, "Alineamiento.txt", 60);

    // --------------------------------------------------------------------------------
    // menu interactivo posterior
    // --------------------------------------------------------------------------------

    while (true) {

        cout << "\n--- menu ---\n";
        cout << "1) generar imagen graphviz de un intervalo\n";
        cout << "2) imprimir matrices dp y direcciones\n";
        cout << "3) mostrar alineamiento completo en pantalla\n";
        cout << "4) salir\n";
        cout << "elige opcion: ";

        int opt;
        if (!(cin >> opt)) {

            cin.clear();
            string junk;
            getline(cin, junk);
            cout << "entrada invalida\n";
            continue;
        }

        if (opt == 1) {

            generar_dot_intervalo(resultado.aligned1, resultado.aligned2);
        }

        else if (opt == 2) {

            cout << "\nmatriz de puntuacion dp:\n";
            imprimir_matriz_tipo(resultado.score);

            cout << "\nmatriz de direcciones:\n";
            imprimir_matriz_tipo(resultado.direc);
        }

        else if (opt == 3) {

            cout << "\nalineamiento completo:\n";
            cout << resultado.aligned1 << "\n";

            string mid = "";
            for (size_t i = 0; i < resultado.aligned1.size(); i++) {

                char c1 = resultado.aligned1[i];
                char c2 = resultado.aligned2[i];

                if (c1 == c2 && c1 != '-') mid.push_back('|');
                else mid.push_back(' ');
            }

            cout << mid << "\n";
            cout << resultado.aligned2 << "\n";
        }

        else if (opt == 4) {

            cout << "saliendo...\n";
            break;
        }

        else {

            cout << "opcion invalida\n";
        }
    }

    return 0;
}

