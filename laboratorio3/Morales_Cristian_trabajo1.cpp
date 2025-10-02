#include <iostream>
#include <string>
using namespace std;

struct Nodo {
    string nombre;
    string carrera;
    float promedio;
    Nodo* siguiente;
};

// Insertar Cola: encolar estudiante al final
void insertarCola(Nodo*& frente, Nodo*& fin, string nombre, string carrera, float promedio) {
    Nodo* nuevo = new Nodo();
    nuevo->nombre = nombre;
    nuevo->carrera = carrera;
    nuevo->promedio = promedio;
    nuevo->siguiente = nullptr;

    if (frente == nullptr) { 
        frente = nuevo;
        fin = nuevo;
    } else {
        fin->siguiente = nuevo;
        fin = nuevo;
    }

    cout << "Estudiante " << nombre << " agregado a la cola.\n";
}

// Eliminar Cola: desencolar estudiante al frente
bool eliminarCola(Nodo*& frente, Nodo*& fin, string& nombre, string& carrera, float& promedio) {
    if (frente == nullptr) return false;

    Nodo* aux = frente;
    nombre = aux->nombre;
    carrera = aux->carrera;
    promedio = aux->promedio;

    frente = frente->siguiente;
    if (frente == nullptr) fin = nullptr; 

    delete aux;
    return true;
}

// Verificar si la cola está vacía
bool colaVacia(Nodo* frente) {
    return (frente == nullptr);
}

// Cola llena (siempre falso en dinámica)
bool colaLlena() {
    return false;
}

// Mostrar toda la cola con for
void mostrarCola(Nodo* frente) {
    if (frente == nullptr) {
        cout << "La cola está vacía.\n";
        return;
    }

    cout << "\nContenido actual de la cola:\n";
    int pos = 1;
    for (Nodo* actual = frente; actual != nullptr; actual = actual->siguiente, pos++) {
        cout << pos << ". " << actual->nombre 
             << " | " << actual->carrera 
             << " | Promedio: " << actual->promedio << endl;
    }
}

int main() {
    Nodo* frente = nullptr;
    Nodo* fin = nullptr;
    int opcion;
    string nombre, carrera;
    float promedio;

    do {
        cout << "\n=== MENU COLA DE ESTUDIANTES ===\n";
        cout << "1. Insertar estudiante (encolar)\n";
        cout << "2. Eliminar estudiante (desencolar)\n";
        cout << "3. Mostrar cola\n";
        cout << "4. Salir\n";
        cout << "Seleccione una opción: ";
        cin >> opcion;
        cin.ignore();

        switch (opcion) {
            case 1:
                cout << "Nombre: ";
                getline(cin, nombre);
                cout << "Carrera: ";
                getline(cin, carrera);
                cout << "Promedio: ";
                cin >> promedio;
                cin.ignore();
                insertarCola(frente, fin, nombre, carrera, promedio);
                break;
            case 2:
                if (eliminarCola(frente, fin, nombre, carrera, promedio)) {
                    cout << "Se desencoló a: " 
                         << nombre << " | " << carrera 
                         << " | Promedio: " << promedio << endl;
                } else {
                    cout << "La cola está vacía.\n";
                }
                break;
            case 3:
                mostrarCola(frente);
                break;
            case 4:
                cout << "Saliendo...\n";
                break;
            default:
                cout << "Opción inválida.\n";
        }
    } while (opcion != 4);

    return 0;
}
