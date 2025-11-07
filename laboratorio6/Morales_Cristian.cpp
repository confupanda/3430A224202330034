// cristian_morales.cpp
// compilar: g++ -std=c++17 -O2 -o 1 cristian_morales.cpp


#include <bits/stdc++.h>
using namespace std;

const int NO_ARISTA = -1; // corresponde a "infinito"
const long long INF = (long long)4e18;

void inicializar_vector_caracter(vector<char>& V) {
    for (size_t i = 0; i < V.size(); ++i) V[i] = ' ';
}

void leer_nodos_ascii(vector<char>& V) {
    // llena con letras mayúsculas A, B, C...
    for (size_t i = 0; i < V.size(); ++i) V[i] = char('A' + (int)i);
}

void imprimir_matriz(const vector<vector<int>>& M) {
    int n = (int)M.size();
    cout << "\nMatriz M (adjacencia):\n";
    for (int i = 0; i < n; ++i) {
        for (int j = 0; j < n; ++j) {
            if (M[i][j] == NO_ARISTA) cout << "  -1 ";
            else cout << setw(4) << M[i][j] << " ";
        }
        cout << "\n";
    }
    cout << endl;
}

void imprimir_vector_caracter(const vector<char>& V, const string& nombre) {
    for (size_t i = 0; i < V.size(); ++i) {
        cout << nombre << "[" << i << "]: " << V[i] << "  ";
    }
    cout << "\n";
}

void imprimir_vector_entero(const vector<int>& D) {
    for (size_t i = 0; i < D.size(); ++i) {
        cout << "D[" << i << "]: " << D[i] << "  ";
    }
    cout << "\n";
}

int buscar_indice_caracter(const vector<char>& V, char caracter) {
    for (size_t i = 0; i < V.size(); ++i) if (V[i] == caracter) return (int)i;
    return (int)V.size(); // si no encuentra, devuelve n
}

int busca_caracter(const vector<char>& V, char c) {
    for (size_t i = 0; i < V.size(); ++i) if (V[i] == c) return 1;
    return 0;
}

void inicializar_vector_D(vector<int>& D, const vector<vector<int>>& M, int origen) {
    int n = (int)M.size();
    for (int j = 0; j < n; ++j) {
        D[j] = M[origen][j];
    }
}

// agrega vértice (carácter) a S[] en el primer espacio libre
void agrega_vertice_a_S(vector<char>& S, char vertice) {
    for (size_t i = 0; i < S.size(); ++i) {
        if (S[i] == ' ') { S[i] = vertice; return; }
    }
}

// actualizar VS: lista de vértices no en S (orden de V)
void actualizar_VS(const vector<char>& V, const vector<char>& S, vector<char>& VS) {
    inicializar_vector_caracter(VS);
    int k = 0;
    for (size_t j = 0; j < V.size(); ++j) {
        if (!busca_caracter(S, V[j])) {
            VS[k++] = V[j];
        }
    }
}

// calcula minimo entre dw (actual D[w]) y dv + mvw
int calcular_minimo(int dw, int dv, int mvw) {
    int minv;
    if (dw == NO_ARISTA) {
        if (dv != NO_ARISTA && mvw != NO_ARISTA)
            minv = dv + mvw;
        else
            minv = NO_ARISTA;
    } else {
        if (dv != NO_ARISTA && mvw != NO_ARISTA) {
            int candidato = dv + mvw;
            minv = (dw <= candidato) ? dw : candidato;
        } else {
            minv = dw;
        }
    }
    // imprimir detalle (útil para el laboratorio)
    cout << "dw: " << dw << " dv: " << dv << " mvw: " << mvw << " => min: " << minv << "\n";
    return minv;
}

// actualiza pesos en D[] usando el vértice v recién agregado a S
void actualizar_pesos(vector<int>& D, const vector<char>& VS, const vector<vector<int>>& M,
                      const vector<char>& V, char v_char) {
    cout << "\n> actualiza pesos en D[] usando vertice '" << v_char << "'\n";
    int indice_v = buscar_indice_caracter(V, v_char);
    int i = 0;
    while (i < (int)VS.size() && VS[i] != ' ') {
        char w_char = VS[i];
        if (w_char != v_char) {
            int indice_w = buscar_indice_caracter(V, w_char);
            int dw = D[indice_w];
            int dv = D[indice_v];
            int mvw = M[indice_v][indice_w];
            D[indice_w] = calcular_minimo(dw, dv, mvw);
        }
        ++i;
    }
}

// elige vértice con menor peso en VS[] (según D[])
// devuelve el char del vértice (no su índice)
char elegir_vertice(const vector<char>& VS, const vector<int>& D, const vector<char>& V) {
    int i = 0;
    int menor = 0;
    bool asignado = false;
    char vertice = ' ';
    while (i < (int)VS.size() && VS[i] != ' ') {
        int idx = buscar_indice_caracter(V, VS[i]);
        int peso = D[idx];
        if ((peso != NO_ARISTA) && (peso != 0)) {
            if (!asignado) {
                menor = peso;
                vertice = VS[i];
                asignado = true;
            } else {
                if (peso < menor) {
                    menor = peso;
                    vertice = VS[i];
                }
            }
        }
        ++i;
    }
    if (!asignado) {
        // no se encontró (todos -1 o 0) -> buscar primer VS no espacio y retornarlo
        i = 0;
        while (i < (int)VS.size() && VS[i] != ' ') {
            vertice = VS[i];
            break;
        ++i;
        }
    }
    cout << "\nvertice elegido: " << vertice << "\n\n";
    return vertice;
}

// función que realiza el algoritmo, imprimiendo pasos (modelo similar al dijkstra.c que enviaste)
void aplicar_dijkstra(vector<char>& V, vector<char>& S, vector<char>& VS, vector<int>& D,
                      const vector<vector<int>>& M, int origen_index) {
    int n = (int)V.size();

    // inicializar D con fila del origen
    inicializar_vector_D(D, M, origen_index);

    cout << "--------- Estados iniciales ---------------------------------------\n";
    imprimir_matriz(M);
    cout << "\n";
    imprimir_vector_caracter(S, "S");
    imprimir_vector_caracter(VS, "VS");
    imprimir_vector_entero(D);
    cout << "------------------------------------------------------------------\n\n";

    // agrega primer vértice: V[origen_index]
    cout << "> agrega primer vértice V[" << origen_index << "] = '" << V[origen_index]
         << "' a S[] y actualiza VS[]\n\n";
    agrega_vertice_a_S(S, V[origen_index]);
    imprimir_vector_caracter(S, "S");

    actualizar_VS(V, S, VS);
    imprimir_vector_caracter(VS, "VS");
    imprimir_vector_entero(D);

    // iterar N-1 veces para completar S
    for (int iter = 1; iter < n; ++iter) {
        cout << "\n> elige vertice menor en VS[] segun valores en D[]\n";
        cout << "> lo agrega a S[] y actualiza VS[]\n";
        char v = elegir_vertice(VS, D, V);

        // si v es espacio (no hay VS), salir
        if (v == ' ' || buscar_indice_caracter(V, v) >= n) {
            cout << "No hay mas vertices validos en VS[]. Termina.\n";
            break;
        }

        agrega_vertice_a_S(S, v);
        imprimir_vector_caracter(S, "S");

        actualizar_VS(V, S, VS);
        imprimir_vector_caracter(VS, "VS");

        actualizar_pesos(D, VS, M, V, v);
        imprimir_vector_entero(D);
    }
}

// genera archivo grafo.txt y trata de generar grafo.png
void imprimir_grafo(const vector<vector<int>>& M, const vector<char>& V, const string& fname = "grafo.txt") {
    int n = (int)M.size();
    FILE *fp = fopen(fname.c_str(), "w");
    if (!fp) {
        cerr << "No se pudo crear " << fname << "\n";
        return;
    }
    fprintf(fp, "digraph G {\n");
    fprintf(fp, "  graph [rankdir=LR]\n");
    fprintf(fp, "  node [style=filled fillcolor=yellow]\n");
    for (int i = 0; i < n; ++i) fprintf(fp, "  %c;\n", V[i]);
    for (int i = 0; i < n; ++i) {
        for (int j = 0; j < n; ++j) {
            if (i == j) continue;
            if (M[i][j] > 0) {
                fprintf(fp, "  %c -> %c [label=%d];\n", V[i], V[j], M[i][j]);
            }
        }
    }
    fprintf(fp, "}\n");
    fclose(fp);

    // intenta generar png (si dot existe)
    int rc = system("dot -Tpng -ografo.png grafo.txt 2>/dev/null");
    if (rc == 0) {
        cout << "Se generó 'grafo.png' a partir de 'grafo.txt'.\n";
        // abrir con eog (si existe) en background (no crítico)
        system("which eog >/dev/null 2>&1 && eog grafo.png &");
    } else {
        cout << "Se creó 'grafo.txt'. Para generar la imagen ejecute: dot -Tpng -ografo.png grafo.txt\n";
    }
}

int main(int argc, char** argv) {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    if (argc < 2) {
        cerr << "Uso: " << argv[0] << " N   (N entero > 2)\n";
        return EXIT_FAILURE;
    }
    int n = atoi(argv[1]);
    if (n <= 2) {
        cerr << "Error: N debe ser > 2.\n";
        return EXIT_FAILURE;
    }

    // vectores/caracteres
    vector<char> V(n), S(n), VS(n);
    vector<int> D(n);

    inicializar_vector_caracter(S);
    inicializar_vector_caracter(VS);
    leer_nodos_ascii(V);

    // pedir origen
    int origen_input;
    cout << "Ingrese vértice origen (1.." << n << "): ";
    if (!(cin >> origen_input)) {
        cerr << "Error leyendo origen.\n";
        return EXIT_FAILURE;
    }
    if (origen_input < 1 || origen_input > n) {
        cerr << "Origen fuera de rango.\n";
        return EXIT_FAILURE;
    }
    int origen_index = origen_input - 1;

    // leer matriz
    vector<vector<int>> M(n, vector<int>(n));
    cout << "Ingrese la matriz " << n << "x" << n << " (use -1 para no arista). Filas separadas por enter, valores por espacios:\n";
    for (int i = 0; i < n; ++i) {
        for (int j = 0; j < n; ++j) {
            if (!(cin >> M[i][j])) {
                cerr << "Error: lectura incompleta de la matriz.\n";
                return EXIT_FAILURE;
            }
        }
    }

    // validaciones y correcciones
    for (int i = 0; i < n; ++i) {
        if (M[i][i] != 0) {
            cerr << "Aviso: M[" << i << "][" << i << "] = " << M[i][i] << " corregido a 0.\n";
            M[i][i] = 0;
        }
        for (int j = 0; j < n; ++j) {
            if (M[i][j] < NO_ARISTA) {
                cerr << "Error: valor invalido en M[" << i << "][" << j << "] = " << M[i][j] << "\n";
                return EXIT_FAILURE;
            }
            if (M[i][j] < 0 && M[i][j] != NO_ARISTA) {
                cerr << "Error: peso negativo distinto de -1 en M[" << i << "][" << j << "]\n";
                return EXIT_FAILURE;
            }
        }
    }

    // aplicar dijkstra (paso a paso)
    aplicar_dijkstra(V, S, VS, D, M, origen_index);

    // generar grafo
    imprimir_grafo(M, V, "grafo.txt");

    cout << "\n--- FIN DEL ALGORITMO ---\n";

    return EXIT_SUCCESS;
}
