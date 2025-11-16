/*
 * morales_cristian.cpp
 * laboratorio 8 - métodos de ordenamiento interno
 *
 * compilar:
 *      g++ Morales_Cristian.cpp -O2 -std=c++11 -o Morales_Cristian
 *
 * ejecutar (mostrar vectores):
 *      ./Morales_Cristian 10 s
 *
 * ejecutar (no mostrar vectores):
 *      ./Morales_Cristian 20000 n
 */

#include <iostream>
#include <vector>
#include <cstdlib>
#include <ctime>
#include <chrono>
#include <iomanip>

using namespace std;
using clock_tiempo = chrono::high_resolution_clock;
using ms = chrono::duration<double, milli>;

// ======================
// funciones auxiliares
// ======================

// imprimir vector estilo pdf
void imprimirVector(const vector<int>& v, const string& titulo) {
    cout << "\n" << titulo << "\n";
    for (int i = 0; i < (int)v.size(); i++) {
        cout << "a[" << i << "]=" << v[i] << " ";
        if ((i+1) % 10 == 0) cout << "\n";
    }
    cout << "\n";
}

// copiar vector original → vector temporal
void copiar(const vector<int>& orig, vector<int>& copia) {
    copia = orig;
}

// ======================
// algoritmos de ordenamiento
// ======================

// burbuja
void burbuja(vector<int>& A) {
    int n = A.size();
    for (int i = 0; i < n - 1; i++) {
        bool cambio = false;
        for (int j = 0; j < n - 1 - i; j++) {
            if (A[j] > A[j + 1]) {
                swap(A[j], A[j + 1]);
                cambio = true;
            }
        }
        if (!cambio) break;
    }
}

// inserción
void insercion(vector<int>& A) {
    for (int i = 1; i < (int)A.size(); i++) {
        int key = A[i];
        int j = i - 1;
        while (j >= 0 && A[j] > key) {
            A[j + 1] = A[j];
            j--;
        }
        A[j + 1] = key;
    }
}

// selección
void seleccion(vector<int>& A) {
    for (int i = 0; i < (int)A.size() - 1; i++) {
        int minIndex = i;
        for (int j = i + 1; j < (int)A.size(); j++) {
            if (A[j] < A[minIndex]) minIndex = j;
        }
        swap(A[i], A[minIndex]);
    }
}

// shellsort
void shellsort(vector<int>& A) {
    int n = A.size();
    for (int gap = n / 2; gap > 0; gap /= 2) {
        for (int i = gap; i < n; i++) {
            int temp = A[i];
            int j = i;
            while (j >= gap && A[j - gap] > temp) {
                A[j] = A[j - gap];
                j -= gap;
            }
            A[j] = temp;
        }
    }
}

// quicksort (partición)
int particion(vector<int>& A, int low, int high) {
    int piv = A[high];
    int i = low - 1;
    for (int j = low; j <= high - 1; j++) {
        if (A[j] <= piv) {
            i++;
            swap(A[i], A[j]);
        }
    }
    swap(A[i + 1], A[high]);
    return i + 1;
}

// quicksort recursivo
void quickRec(vector<int>& A, int low, int high) {
    if (low < high) {
        int pi = particion(A, low, high);
        quickRec(A, low, pi - 1);
        quickRec(A, pi + 1, high);
    }
}

void quicksort(vector<int>& A) {
    if (!A.empty()) quickRec(A, 0, A.size() - 1);
}

// ======================
// medición de tiempos
// ======================
double medir(void (*f)(vector<int>&), vector<int> A) {
    auto t0 = clock_tiempo::now();
    f(A);
    auto t1 = clock_tiempo::now();
    return ms(t1 - t0).count();
}

// ======================
// programa principal
// ======================
int main(int argc, char** argv) {

    // validación de parámetros
    if (argc != 3) {
        cout << "uso: ./Morales_Cristian N {s|n}\n";
        return 1;
    }

    int N = atoi(argv[1]);
    char VER = argv[2][0];

    if (N <= 0) {
        cout << "n debe ser positivo.\n";
        return 1;
    }

    bool mostrar = (VER == 's' || VER == 'S');

    // generar vector aleatorio
    srand(time(NULL));
    vector<int> original(N);
    for (int i = 0; i < N; i++)
        original[i] = rand() % 10000;

    if (mostrar) imprimirVector(original, "vector original:");

    // tiempos
    double t_burbuja, t_insercion, t_seleccion, t_shell, t_quick;

    // burbuja
    {
        vector<int> A;
        copiar(original, A);
        t_burbuja = medir(burbuja, A);
        if (mostrar) { burbuja(A); imprimirVector(A, "burbuja:"); }
    }

    // inserción
    {
        vector<int> A;
        copiar(original, A);
        t_insercion = medir(insercion, A);
        if (mostrar) { insercion(A); imprimirVector(A, "insercion:"); }
    }

    // selección
    {
        vector<int> A;
        copiar(original, A);
        t_seleccion = medir(seleccion, A);
        if (mostrar) { seleccion(A); imprimirVector(A, "seleccion:"); }
    }

    // shellsort
    {
        vector<int> A;
        copiar(original, A);
        t_shell = medir(shellsort, A);
        if (mostrar) { shellsort(A); imprimirVector(A, "shellsort:"); }
    }

    // quicksort
    {
        vector<int> A;
        copiar(original, A);
        t_quick = medir(quicksort, A);
        if (mostrar) { quicksort(A); imprimirVector(A, "quicksort:"); }
    }

    // tabla de tiempos
    cout << "\n---------- tiempos de ejecución (ms) ----------\n";
    cout << left << setw(12) << "metodo" << " | tiempo (ms)\n";
    cout << "-----------------------------------------------\n";
    cout << setw(12) << "burbuja"    << " | " << fixed << setprecision(3) << t_burbuja    << "\n";
    cout << setw(12) << "insercion"  << " | " << t_insercion  << "\n";
    cout << setw(12) << "seleccion"  << " | " << t_seleccion  << "\n";
    cout << setw(12) << "shellsort"  << " | " << t_shell      << "\n";
    cout << setw(12) << "quicksort"  << " | " << t_quick      << "\n";
    cout << "-----------------------------------------------\n";

    return 0;
}
