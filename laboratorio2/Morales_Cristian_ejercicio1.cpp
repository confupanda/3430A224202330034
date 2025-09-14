#include <iostream>
#include <cstdlib>
using namespace std;

class Pila {
private:
    int* elementos;   // arreglo dinámico
    int tope;
    int max;

public:
    Pila(int capacidad) {
        max = capacidad;
        tope = 0;
        elementos = new int[max + 1]; // arreglo de 1..max
    }

    ~Pila() {
        delete[] elementos;
    }

    bool pilaVacia() {
        return (tope == 0);
    }

    bool pilaLlena() {
        return (tope == max);
    }

    void push(int dato) {
        if (pilaLlena()) {
            cout << "\n Error: Desbordamiento, la pila está llena.\n";
        } else {
            tope++;
            elementos[tope] = dato;
            cout << " Elemento " << dato << " agregado a la pila.\n";
        }
    }

    void pop() {
        if (pilaVacia()) {
            cout << "\n  Error: Subdesbordamiento, la pila está vacía.\n";
        } else {
            cout << "  Elemento eliminado: " << elementos[tope] << endl;
            tope--;
        }
    }

    void mostrar() {
        if (pilaVacia()) {
            cout << "\n La pila está vacía.\n";
            return;
        }
        cout << "\nEstado actual de la pila:\n";
        for (int i = tope; i >= 1; i--) {
            cout << "| " << elementos[i] << " |" << endl;
        }
    }
};

int main() {
    int opcion, valor, capacidad;
    cout << "Ingrese tamaño máximo de la pila: ";
    cin >> capacidad;

    Pila pila(capacidad);

    do {
        cout << "\n========= MENU PILA =========\n";
        cout << "1. Agregar (Push)\n";
        cout << "2. Remover (Pop)\n";
        cout << "3. Ver pila\n";
        cout << "0. Salir\n";
        cout << "=============================\n";
        cout << "Opción: ";
        cin >> opcion;

        switch (opcion) {
            case 1:
                cout << "Ingrese valor: ";
                cin >> valor;
                pila.push(valor);
                break;
            case 2:
                pila.pop();
                break;
            case 3:
                pila.mostrar();
                break;
            case 0:
                cout << "Saliendo del programa...\n";
                break;
            default:
                cout << " Opción inválida.\n";
        }
    } while (opcion != 0);

    return 0;
}
