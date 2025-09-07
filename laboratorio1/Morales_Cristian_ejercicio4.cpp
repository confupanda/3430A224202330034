#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
using namespace std;

struct Paciente {
    string nombre;
    int edad;
    float peso;
    float altura;
    Paciente* siguiente;
};

void agregarPaciente(Paciente*& cabeza, string nombre, int edad, float peso, float altura) {
    Paciente* nuevo = new Paciente{nombre, edad, peso, altura, cabeza};
    cabeza = nuevo;
}

void imprimirPacientes(Paciente* cabeza) {
    for (Paciente* temp = cabeza; temp != nullptr; temp = temp->siguiente) {
        cout << temp->nombre << " | Edad: " << temp->edad
             << " | Peso: " << temp->peso
             << " | Altura: " << temp->altura << endl;
    }
}

void cargarDesdeArchivo(Paciente*& cabeza, string filename) {
    ifstream file(filename);
    if (!file.is_open()) {
        cout << "No se pudo abrir el archivo.\n";
        return;
    }

    string linea;
    while (getline(file, linea)) {
        string nombre;
        int edad;
        float peso, altura;

        stringstream ss(linea);
        getline(ss, nombre, ',');
        ss >> edad;
        ss.ignore();
        ss >> peso;
        ss.ignore();
        ss >> altura;

        agregarPaciente(cabeza, nombre, edad, peso, altura);
    }

    file.close();
}

void eliminarLista(Paciente*& cabeza) {
    for (Paciente* temp = cabeza; temp != nullptr; ) {
        Paciente* borrar = temp;
        temp = temp->siguiente;
        delete borrar;
    }
    cabeza = nullptr;
}

int main() {
    Paciente* lista = nullptr;
    int opcion;

    for (;;) {
        cout << "\n--- Menu de Gestion de Pacientes ---\n";
        cout << "1. Agregar paciente\n";
        cout << "2. Mostrar pacientes\n";
        cout << "3. Cargar desde archivo CSV\n";
        cout << "4. Salir\n";
        cout << "Opcion: ";
        cin >> opcion;

        if (opcion == 1) {
            string nombre;
            int edad;
            float peso, altura;

            cin.ignore(); // limpia el salto pendiente ANTES de getline

            cout << "Nombre: ";
            getline(cin, nombre);

            cout << "Edad: ";
            cin >> edad;
            cout << "Peso: ";
            cin >> peso;
            cout << "Altura: ";
            cin >> altura;

            // limpiar lo que quede en el buffer antes de volver al menú
            cin.ignore();

            agregarPaciente(lista, nombre, edad, peso, altura);
        } 
        else if (opcion == 2) {
            imprimirPacientes(lista);
        } 
        else if (opcion == 3) {
            string archivo;
            cout << "Nombre del archivo CSV: ";
            cin >> archivo;
            cargarDesdeArchivo(lista, archivo);
        } 
        else if (opcion == 4) {
            break;
        }
    }

    eliminarLista(lista);
    return 0;
}
