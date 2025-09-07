#include <iostream>
#include <string>
using namespace std;

struct Paciente {
    string nombre;
    int edad;
    float peso;
    float altura;
};

int main() {
    Paciente pacientes[3] = {
        {"Ana Torres", 25, 60.5, 1.65},
        {"Carlos Díaz", 40, 80.0, 1.75},
        {"María López", 32, 55.3, 1.60}
    };

    for (int i = 0; i < 3; i++) {
        cout << "Paciente " << i+1 << ": " << pacientes[i].nombre
             << ", Edad: " << pacientes[i].edad
             << ", Peso: " << pacientes[i].peso << "kg"
             << ", Altura: " << pacientes[i].altura << "m\n";
    }

    return 0;
}
