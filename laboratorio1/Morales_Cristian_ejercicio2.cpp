#include <iostream>
#include <string>
using namespace std;

struct Paciente {
    string nombre;
    int edad;
    float peso;
    float altura;
    Paciente* siguiente; // Puntero al siguiente nodo
};

// Función para agregar paciente al inicio de la lista
void agregarPaciente(Paciente*& cabeza, string nombre, int edad, float peso, float altura) {
    Paciente* nuevo = new Paciente{nombre, edad, peso, altura, cabeza};
    cabeza = nuevo;
}

// Función para imprimir lista de pacientes
void imprimirPacientes(Paciente* cabeza) {
    while (cabeza != nullptr) {
        cout << cabeza->nombre << " | Edad: " << cabeza->edad 
             << " | Peso: " << cabeza->peso 
             << " | Altura: " << cabeza->altura << endl;
        cabeza = cabeza->siguiente;
    }
}

// Función para eliminar toda la lista
void eliminarLista(Paciente*& cabeza) {
    while (cabeza != nullptr) {
        Paciente* temp = cabeza;
        cabeza = cabeza->siguiente;
        delete temp;
    }
}

int main() {
    Paciente* lista = nullptr;

    // Agregar pacientes
    agregarPaciente(lista, "Ana Torres", 25, 60.5, 1.65);
    agregarPaciente(lista, "Carlos Díaz", 40, 80.0, 1.75);
    agregarPaciente(lista, "María López", 32, 55.3, 1.60);

    cout << "Lista de pacientes:\n";
    imprimirPacientes(lista);

    eliminarLista(lista);
    return 0;
}
