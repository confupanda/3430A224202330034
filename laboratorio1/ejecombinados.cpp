#include <iostream>              // Librería para entrada y salida
#include <string>                // Librería para usar string
using namespace std;             // Para no escribir std:: en todo

// --------------------- FUNCIONES ---------------------

// Función que devuelve la base complementaria
char complBase(char b) {
    switch (b) {                 // Según la base ingresada
        case 'A': return 'T';    // A complementa con T
        case 'T': return 'A';    // T complementa con A
        case 'C': return 'G';    // C complementa con G
        case 'G': return 'C';    // G complementa con C
        default:  return '?';    // Si no es válida, devolvemos '?'
    }
}

// Verifica si la secuencia es complementaria de sí misma
bool esComplementaria(const string& s) {
    int i = 0;                       // Índice desde el inicio
    int j = (int)s.size() - 1;       // Índice desde el final

    while (i < j) {                  // Recorremos mientras no crucemos los índices
        char izq = s[i];             // Base izquierda
        char der = s[j];             // Base derecha
        char debeSer = complBase(izq); // Complemento esperado de la base izquierda

        if (debeSer == '?' || der != debeSer) // Si no coincide el complemento
            return false;            // No es complementaria

        i++;                         // Avanzamos hacia el centro
        j--;
    }
    return true;                     // Si todas coincidieron, sí es complementaria
}

// Verifica si la secuencia es palíndroma
bool esPalindromo(const string& s) {
    int i = 0;                       // Índice desde el inicio
    int j = (int)s.size() - 1;       // Índice desde el final

    while (i < j) {                  // Comparación simétrica
        if (s[i] != s[j])            // Si algún par no coincide
            return false;            // No es palíndroma
        i++;
        j--;
    }
    return true;                     // Si todas coincidieron, sí es palíndroma
}

// --------------------- PROGRAMA PRINCIPAL ---------------------

int main() {
    string secuencia;                // Variable para guardar la secuencia
    int opcion;                      // Variable para guardar la elección del usuario

    cout << "Ingrese una secuencia de nucleotidos: ";
    cin >> secuencia;                // Leemos la secuencia desde teclado

    cout << "\nSeleccione una opcion:\n";
    cout << "1. Verificar si es complementaria de si misma\n";
    cout << "2. Verificar si es palindroma\n";
    cout << "Opcion: ";
    cin >> opcion;                   // Leemos la opción del usuario

    if (opcion == 1) {               // Si elige la opción 1
        if (esComplementaria(secuencia))
            cout << "✔ La secuencia ES complementaria de si misma.\n";
        else
            cout << "✘ La secuencia NO es complementaria de si misma.\n";
    }
    else if (opcion == 2) {          // Si elige la opción 2
        if (esPalindromo(secuencia))
            cout << "✔ La secuencia ES palindroma.\n";
        else
            cout << "✘ La secuencia NO es palindroma.\n";
    }
    else {
        cout << "Opcion invalida.\n"; // Si pone otro número
    }

    return 0;                        // Fin del programa
}
