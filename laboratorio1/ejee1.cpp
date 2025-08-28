#include <iostream>              // Permite usar cout y cin
#include <string>                // Permite usar std::string
using namespace std;             // Evita escribir std:: en cada uso

char complBase(char b) {         // Función que devuelve la base complementaria
    switch (b) {                 // Selecciona según el carácter recibido
        case 'A': return 'T';    // A complementa con T
        case 'T': return 'A';    // T complementa con A
        case 'C': return 'G';    // C complementa con G
        case 'G': return 'C';    // G complementa con C
        default:  return '?';    // Si viene algo distinto a ATCG, marcamos como inválido
    }
}

bool esComplementaria(const string& s) { // Recibe la secuencia y responde true/false
    int i = 0;                           // Índice desde la izquierda (primer carácter)
    int j = (int)s.size() - 1;           // Índice desde la derecha (último carácter)

    while (i < j) {                      // Recorre comparando pares (1° con último, 2° con penúltimo, etc.)
        char izq = s[i];                 // Toma la base de la izquierda
        char der = s[j];                 // Toma la base de la derecha
        char debeSer = complBase(izq);   // Calcula cuál debería ser su complemento

        if (debeSer == '?' || der != debeSer) // Si la base es inválida o no coincide el complemento
            return false;                // Entonces NO es complementaria de sí misma

        i++;                             // Avanza desde la izquierda
        j--;                             // Retrocede desde la derecha
    }
    return true;                         // Si todos los pares coinciden, SÍ es complementaria
}

int main() {                             // Programa principal
    string sec;                          // Variable para guardar la secuencia
    cout << "Secuencia: ";               // Pide la secuencia
    cin >> sec;                          // Lee la secuencia por teclado

    if (esComplementaria(sec))           // Llama a la función
        cout << "ES complementaria de si misma\n";   // Mensaje si es complementaria
    else
        cout << "NO es complementaria de si misma\n"; // Mensaje si no lo es

    return 0;                            // Fin del programa
}
