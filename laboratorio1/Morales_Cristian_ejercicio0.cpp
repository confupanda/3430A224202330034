#include <iostream>
#include <string>
using namespace std;

// Definición de estructura Paciente
struct Paciente {
    string nombre;
    int edad;
    float peso;
    float altura;
};

int main() {
    // Ejemplos de pacientes
    Paciente p1 = {"Ana Torres", 25, 60.5, 1.65};
    Paciente p2 = {"Carlos Díaz", 40, 80.0, 1.75};
    Paciente p3 = {"María López", 32, 55.3, 1.60};

    // Mostrar información
    cout << "Paciente 1: " << p1.nombre << ", Edad: " << p1.edad 
         << ", Peso: " << p1.peso << "kg, Altura: " << p1.altura << "m\n";
    cout << "Paciente 2: " << p2.nombre << ", Edad: " << p2.edad 
         << ", Peso: " << p2.peso << "kg, Altura: " << p2.altura << "m\n";
    cout << "Paciente 3: " << p3.nombre << ", Edad: " << p3.edad 
         << ", Peso: " << p3.peso << "kg, Altura: " << p3.altura << "m\n";

    return 0;
}
