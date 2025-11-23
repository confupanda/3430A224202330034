

#include <iostream>
#include <vector>
#include <list>
#include <iomanip>
#include <fstream>
#include <limits>

using namespace std;

const int M = 20;

// =======================
// hash basico
// =======================

int h1(int k) {
    return k % M;
}

int h2(int k) {
    return 1 + (k % (M - 1));
}

// =======================
// impresion
// =======================

void imprimir_abierto(const vector<int>& t) {
    cout << "----------------------------------\n";
    for (int i = 0; i < M; i++) {
        cout << "[" << setw(2) << i << "]: ";
        if (t[i] == -1) cout << "--";
        else cout << t[i];
        cout << "\n";
    }
    cout << "----------------------------------\n";
}

void imprimir_encadenado(const vector<list<int>>& te) {
    cout << "----------------------------------\n";
    for (int i = 0; i < M; i++) {
        cout << "[" << setw(2) << i << "]: ";
        if (te[i].empty()) cout << "--";
        else {
            int c = 0;
            for (int v : te[i]) {
                cout << v;
                if (++c < (int)te[i].size()) cout << " -> ";
            }
        }
        cout << "\n";
    }
    cout << "----------------------------------\n";
}

// =======================
// guardar archivo
// =======================

void guardar_abierto(const vector<int>& t, const string& nombre, const string& modo) {
    ofstream out(nombre);

    if (!out.is_open()) {
        cout << "no se pudo crear el archivo\n";
        return;
    }

    out << "tabla hash guardada\n";
    out << "modo: " << modo << "\n\n";

    for (int i = 0; i < M; i++) {
        out << "[" << i << "]: ";
        if (t[i] == -1) out << "--";
        else out << t[i];
        out << "\n";
    }

    out.close();
    cout << "archivo guardado: " << nombre << "\n";
}

void guardar_encadenado(const vector<list<int>>& te, const string& nombre) {
    ofstream out(nombre);

    if (!out.is_open()) {
        cout << "no se pudo crear el archivo\n";
        return;
    }

    out << "tabla hash guardada\n";
    out << "modo: encadenamiento\n\n";

    for (int i = 0; i < M; i++) {
        out << "[" << i << "]: ";
        if (te[i].empty()) {
            out << "--";
        } else {
            int c = 0;
            for (int v : te[i]) {
                out << v;
                if (++c < (int)te[i].size()) out << " -> ";
            }
        }
        out << "\n";
    }

    out.close();
    cout << "archivo guardado: " << nombre << "\n";
}

// =======================
// insertar
// =======================

void insertar_lineal(vector<int>& t, int k) {
    int base = h1(k);
    int pos = base;
    int i = 0;

    while (t[pos] != -1) {
        cout << "colision en " << pos << "\n";
        i++;
        pos = (base + i) % M;
        if (i >= M) {
            cout << "tabla llena\n";
            return;
        }
    }

    if (i > 0) cout << "desplazamiento final: " << i << "\n";

    t[pos] = k;
    cout << "insertado en posicion " << pos << "\n";
}

void insertar_cuadratica(vector<int>& t, int k) {
    int base = h1(k);
    int pos = base;
    int i = 0;

    while (t[pos] != -1) {
        cout << "colision en " << pos << "\n";
        i++;
        pos = (base + i * i) % M;
        if (i >= M) {
            cout << "tabla llena\n";
            return;
        }
    }

    if (i > 0) cout << "desplazamiento final: " << i << "\n";

    t[pos] = k;
    cout << "insertado en posicion " << pos << "\n";
}

void insertar_doble(vector<int>& t, int k) {
    int base = h1(k);
    int pos = base;
    int i = 0;

    while (t[pos] != -1) {
        cout << "colision en " << pos << "\n";
        i++;
        pos = (base + i * h2(k)) % M;
        if (i >= M) {
            cout << "tabla llena\n";
            return;
        }
    }

    if (i > 0) cout << "desplazamiento final: " << i << "\n";

    t[pos] = k;
    cout << "insertado en posicion " << pos << "\n";
}

void insertar_enc(vector<list<int>>& te, int k) {
    int pos = h1(k);
    bool col = !te[pos].empty();

    te[pos].push_back(k);

    if (col) cout << "colision en " << pos << ", agregado al final\n";

    cout << "insertado en lista " << pos << "\n";
}

// =======================
// buscar
// =======================

void buscar_abierto(const vector<int>& t, int k, const string& modo) {
    int base = h1(k);
    int pos = base;
    int i = 0;

    while (t[pos] != -1) {
        if (t[pos] == k) {
            cout << "encontrado en posicion " << pos << "\n";
            if (i > 0) cout << "desplazamiento usado: " << i << "\n";
            return;
        }

        i++;

        if (modo == "lineal") pos = (base + i) % M;
        else if (modo == "cuadratica") pos = (base + i * i) % M;
        else pos = (base + i * h2(k)) % M;

        if (i >= M) break;
    }

    cout << "no encontrado\n";
}

void buscar_enc(const vector<list<int>>& te, int k) {
    int pos = h1(k);
    int idx = 0;

    for (int v : te[pos]) {
        if (v == k) {
            cout << "encontrado en indice " << pos << ", posicion en lista " << idx << "\n";
            return;
        }
        idx++;
    }

    cout << "no encontrado\n";
}

// =======================
// menu interactivo
// =======================

void menu_interactivo(char modo, vector<int> &tabla, vector<list<int>> &tabla_e) {
    while (true) {
        cout << "\n---- menú ----\n";
        cout << "1) insertar valor\n";
        cout << "2) buscar valor\n";
        cout << "3) mostrar tabla\n";
        cout << "4) vaciar tabla\n";
        cout << "5) salir\n";
        cout << "opción: ";

        int op;
        if (!(cin >> op)) {
            cin.clear();
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            cout << "entrada invalida\n";
            continue;
        }

        if (op == 1) {
            cout << "ingrese valor a insertar: ";
            int v; cin >> v;

            if (modo == 'l') insertar_lineal(tabla, v);
            else if (modo == 'c') insertar_cuadratica(tabla, v);
            else if (modo == 'd') insertar_doble(tabla, v);
            else insertar_enc(tabla_e, v);

        } else if (op == 2) {
            cout << "ingrese valor a buscar: ";
            int v; cin >> v;

            if (modo == 'e') buscar_enc(tabla_e, v);
            else {
                string m =
                    (modo=='l'?"lineal":
                    (modo=='c'?"cuadratica":"doble"));
                buscar_abierto(tabla, v, m);
            }

        } else if (op == 3) {
            if (modo == 'e') imprimir_encadenado(tabla_e);
            else imprimir_abierto(tabla);

        } else if (op == 4) {
            tabla.assign(M, -1);
            for (int i = 0; i < M; i++) tabla_e[i].clear();
            cout << "tabla vaciada\n";

        } else if (op == 5) {
            cout << "guardando tabla en archivo...\n";

            if (modo == 'e')
                guardar_encadenado(tabla_e, "hash_salida.txt");
            else {
                string m =
                    (modo=='l'?"lineal":
                    (modo=='c'?"cuadratica":"doble hash"));
                guardar_abierto(tabla, "hash_salida.txt", m);
            }

            cout << "saliendo del programa\n";
            break;

        } else {
            cout << "opcion no valida\n";
        }
    }
}

// =======================
// main
// =======================

int main(int argc, char *argv[]) {

    if (argc < 2) {
        cout << "uso: ./programa {l|c|d|e}\n";
        return 0;
    }

    char modo = tolower(argv[1][0]);

    vector<int> tabla(M, -1);
    vector<list<int>> tabla_e(M);

    menu_interactivo(modo, tabla, tabla_e);

    return 0;
}
