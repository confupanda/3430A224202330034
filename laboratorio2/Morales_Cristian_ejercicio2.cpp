#include <iostream>
#include <stack>
#include <vector>
#include <iomanip>
#include <cstdlib> // atoi
using namespace std;

class PuertoSeco {
private:
    int n; // capacidad por pila
    int m; // cantidad de pilas
    vector<stack<string>> pilas;

public:
    PuertoSeco(int n_, int m_) {
        n = n_;
        m = m_;
        pilas.resize(m);
    }

    void ingresarManual(string id, int indice) {
        if (indice < 0 || indice >= m) {
            cout << "Índice de pila inválido.\n";
            return;
        }
        if ((int)pilas[indice].size() >= n) {
            cout << "La pila " << indice << " está llena.\n";
        } else {
            pilas[indice].push(id);
            cout << "Contenedor " << id << " agregado a pila " << indice << endl;
        }
    }

    void ingresarAutomatico(string id) {
        for (int i = 0; i < m; i++) {
            if ((int)pilas[i].size() < n) {
                pilas[i].push(id);
                cout << " Contenedor " << id << " agregado automáticamente a pila " << i << endl;
                return;
            }
        }
        cout << "No hay espacio disponible para el contenedor " << id << endl;
    }

    void mostrarEstado() {
        cout << "\n======= ESTADO DEL PUERTO =======\n";
        for (int nivel = n; nivel > 0; nivel--) {
            for (int j = 0; j < m; j++) {
                if ((int)pilas[j].size() >= nivel) {
                    stack<string> aux = pilas[j];
                    for (int k = 0; k < nivel - 1; k++) aux.pop();
                    cout << setw(6) << aux.top();
                } else {
                    cout << setw(6) << ".";
                }
            }
            cout << endl;
        }
        cout << "---------------------------------\n";
        for (int j = 0; j < m; j++) {
            cout << setw(6) << "P" + to_string(j);
        }
        cout << "\n=================================\n";
    }

    void retirarContenedor(string id) {
        for (int i = 0; i < m; i++) {
            stack<string> aux;
            bool encontrado = false;

            while (!pilas[i].empty()) {
                string cima = pilas[i].top();
                pilas[i].pop();

                if (cima == id) {
                    cout << " Retirando contenedor " << id << " desde pila " << i << endl;
                    encontrado = true;
                    break;
                } else {
                    cout << " Moviendo contenedor " << cima << " temporalmente." << endl;
                    aux.push(cima);
                }
            }

            // Devolver los contenedores
            while (!aux.empty()) {
                pilas[i].push(aux.top());
                aux.pop();
            }

            if (encontrado) return;
        }
        cout << "Contenedor " << id << " no encontrado.\n";
    }
};

int main(int argc, char* argv[]) {
    if (argc < 3) {
        cout << "Uso: " << argv[0] << " <capacidad_n> <cantidad_m>\n";
        return 1;
    }

    int n = atoi(argv[1]);
    int m = atoi(argv[2]);
    PuertoSeco puerto(n, m);

    int opcion;
    string id;
    int indice;

    do {
        cout << "\n========= MENU PUERTO SECO =========\n";
        cout << "1. Ingresar contenedor manual\n";
        cout << "2. Ingresar contenedor automático\n";
        cout << "3. Retirar contenedor\n";
        cout << "4. Ver estado\n";
        cout << "0. Salir\n";
        cout << "===================================\n";
        cout << "Opción: ";
        cin >> opcion;

        switch (opcion) {
            case 1:
                cout << "Ingrese ID del contenedor: ";
                cin >> id;
                cout << "Ingrese pila destino (0 a " << m-1 << "): ";
                cin >> indice;
                puerto.ingresarManual(id, indice);
                break;
            case 2:
                cout << "Ingrese ID del contenedor: ";
                cin >> id;
                puerto.ingresarAutomatico(id);
                break;
            case 3:
                cout << "Ingrese ID del contenedor a retirar: ";
                cin >> id;
                puerto.retirarContenedor(id);
                break;
            case 4:
                puerto.mostrarEstado();
                break;
            case 0:
                cout << " Saliendo del programa...\n";
                break;
            default:
                cout << " Opción inválida.\n";
        }
    } while (opcion != 0);

    return 0;
}
