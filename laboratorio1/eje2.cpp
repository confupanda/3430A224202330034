#include <iostream>              // Permite usar cout y cin
#include <string>                // Permite usar std::string
using namespace std;             // Evita escribir std:: en cada uso

bool esPalindromo(const string& s) { // Devuelve true si se lee igual al revés
    int i = 0;                       // Índice al inicio de la cadena
    int j = (int)s.size() - 1;       // Índice al final de la cadena

    while (i < j) {                  // Compara simétricamente desde los extremos
        if (s[i] != s[j])            // Si alguna pareja de caracteres no coincide
            return false;            // Entonces NO es palíndromo
        i++;                         // Avanza desde la izquierda
        j--;                         // Retrocede desde la derecha
    }
    return true;                     // Si todas las parejas coinciden, SÍ es palíndromo
}

int main() {                         // Programa principal
    string sec;                      // Variable para la secuencia
    cout << "Secuencia: ";           // Pide la secuencia
    cin >> sec;                      // Lee la secuencia

    if (esPalindromo(sec))           // Llama a la función
        cout << "ES palindroma\n";   // Imprime resultado positivo
    else
        cout << "NO es palindroma\n";// Imprime resultado negativo

    return 0;                        // Fin del programa
}
