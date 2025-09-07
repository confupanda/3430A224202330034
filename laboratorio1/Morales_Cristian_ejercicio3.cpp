#include <iostream>
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
        float imc = temp->peso / (temp->altura * temp->altura);
        cout << temp->nombre << " | Edad: " << temp->edad
             << " | Peso: " << temp->peso 
             << " | Altura: " << temp->altura 
             << " | IMC: " << imc << endl;
    }
}

void estadisticas(Paciente* cabeza) {
    int total = 0, sumaEdad = 0;
    float sumaPeso = 0;

    for (Paciente* temp = cabeza; temp != nullptr; temp = temp->siguiente) {
        total++;
        sumaEdad += temp->edad;
        sumaPeso += temp->peso;
    }

    if (total > 0) {
        cout << "\nPromedio edad: " << (float)sumaEdad / total << endl;
        cout << "Promedio peso: " << sumaPeso / total << "kg\n";
    }
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

    agregarPaciente(lista, "nicole perez", 25, 60.5, 1.65);
    agregarPaciente(lista, "Cristian morales", 40, 80.0, 1.75);
    agregarPaciente(lista, "juan alcachofa", 32, 55.3, 1.60);

    cout << "Pacientes:\n";
    imprimirPacientes(lista);

    estadisticas(lista);

    eliminarLista(lista);
    return 0;
}
