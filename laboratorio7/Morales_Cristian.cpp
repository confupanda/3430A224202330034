/*
 * Morales_Cristian.cpp
 * laboratorio 7 - Algoritmo de Prim 
 * Compilar: g++ Morales_Cristian.cpp -o prim
 * Ejecutar: ./1
 */

#include <iostream>
#include <fstream>
#include <iomanip>
#include <climits>
#include <string>
#include <cstdlib> 
using namespace std;

void inicializar_matriz(int **matriz, int n) {
    for (int i = 0; i < n; i++)
        for (int j = 0; j < n; j++)
            matriz[i][j] = 0;
}

void imprimir_matriz(int **matriz, string *nodos, int n) {
    cout << "\nMatriz de Adyacencia:\n    ";
    for (int i = 0; i < n; i++) cout << setw(4) << nodos[i];
    cout << "\n   ";
    for (int i = 0; i < n; i++) cout << "----";
    cout << endl;
    for (int i = 0; i < n; i++) {
        cout << setw(3) << nodos[i] << "|";
        for (int j = 0; j < n; j++)
            cout << setw(4) << matriz[i][j];
        cout << endl;
    }
}

void leer_datos(string *nodos, int **matriz, int n) {
    cout << "\n--- Ingreso de Nodos ---\n";
    for (int i = 0; i < n; i++) {
        cout << "Nombre del nodo " << i + 1 << ": ";
        cin >> nodos[i];
    }
    cout << "\n--- Ingreso de Pesos (0 = sin conexión) ---\n";
    for (int i = 0; i < n; i++) {
        for (int j = i + 1; j < n; j++) {
            cout << "Peso entre " << nodos[i] << " y " << nodos[j] << ": ";
            int w; cin >> w;
            matriz[i][j] = matriz[j][i] = w;
        }
    }
}

void generar_dot(int **matriz, string *nodos, int n, string nombre) {
    ofstream f(nombre + ".dot");
    f << "graph G {\n  node [style=filled fillcolor=\"#00ff005f\"];\n";
    for (int i = 0; i < n; i++)
        for (int j = i + 1; j < n; j++)
            if (matriz[i][j] > 0)
                f << "  " << nodos[i] << " -- " << nodos[j]
                  << " [label=" << matriz[i][j] << "];\n";
    f << "}\n"; f.close();

    string comando_png = "dot -Tpng " + nombre + ".dot -o " + nombre + ".png";
    system(comando_png.c_str()); // genera imagen
    cout << "Archivo '" << nombre << ".png' generado.\n";
}

int minimo(int *key, bool *vis, int n) {
    int minVal = INT_MAX, idx = -1;
    for (int i = 0; i < n; i++)
        if (!vis[i] && key[i] < minVal)
            minVal = key[i], idx = i;
    return idx;
}

void prim(int **G, int **MST, int n) {
    int parent[n], key[n];
    bool vis[n];
    for (int i = 0; i < n; i++) {
        key[i] = INT_MAX;
        vis[i] = false;
        parent[i] = -1;
    }
    key[0] = 0;

    for (int c = 0; c < n - 1; c++) {
        int u = minimo(key, vis, n);
        vis[u] = true;
        for (int v = 0; v < n; v++)
            if (G[u][v] && !vis[v] && G[u][v] < key[v])
                parent[v] = u, key[v] = G[u][v];
    }

    for (int i = 1; i < n; i++)
        if (parent[i] != -1)
            MST[i][parent[i]] = MST[parent[i]][i] = G[i][parent[i]];
}

void mostrar_L(int **MST, string *nodos, int n) {
    int total = 0;
    cout << "\nConjunto L (Aristas del MST):\nL = { ";
    bool first = true;
    for (int i = 0; i < n; i++)
        for (int j = i + 1; j < n; j++)
            if (MST[i][j] > 0) {
                if (!first) cout << ", ";
                cout << "(" << nodos[i] << ", " << nodos[j]
                     << ", " << MST[i][j] << ")";
                total += MST[i][j];
                first = false;
            }
    cout << " }\nCosto total: " << total << endl;
}

int main(int argc, char **argv) {
    if (argc < 2) {
        cout << "uso: ./1 N\n";
        return -1;
    }
    int n = atoi(argv[1]);
    if (n < 2) {
        cout << "Error: N debe ser mayor a 1.\n";
        return -1;
    }

    string *nodos = new string[n];
    int **G = new int*[n], **MST = new int*[n];
    for (int i = 0; i < n; i++) {
        G[i] = new int[n];
        MST[i] = new int[n];
    }

    inicializar_matriz(G, n);
    inicializar_matriz(MST, n);
    leer_datos(nodos, G, n);
    imprimir_matriz(G, nodos, n);
    generar_dot(G, nodos, n, "grafo_original");
    prim(G, MST, n);
    generar_dot(MST, nodos, n, "grafo_minimo");
    mostrar_L(MST, nodos, n);

    cout << "\nAbriendo imagen del grafo mínimo...\n";
    system("xdg-open grafo_minimo.png"); // abre la imagen 

    for (int i = 0; i < n; i++) { delete[] G[i]; delete[] MST[i]; }
    delete[] G; delete[] MST; delete[] nodos;

    return 0;
}
