README

Este es mi proyecto de implementación del algoritmo Needleman–Wunsch en C++. El objetivo es alinear dos secuencias desde un archivo FASTA y obtener el alineamiento global junto con el porcentaje de identidad. También se pueden ver las matrices usadas en el proceso y guardar el resultado final.

Uso una puntuación simple:

match = +1

mismatch = -1

gap negativo (lo ingresa el usuario)

Quise dejar el programa lo más claro posible, sin matrices PAM/BLOSUM ni cosas externas. Solo utilizo la matriz de programación dinámica y la matriz de direcciones para reconstruir el alineamiento.

Compilar

// compilar:
// g++ -std=c++17 -O2 -o needleman_complete needleman_complete.cpp

Ejecutar

// ejemplos:
// ./needleman_complete -c gene.fna gene2.fna
// ./needleman_complete -w gene.fna
// ./needleman_complete -c gene.fna gene2.fna -5

Al ejecutar el programa, puedo seleccionar las secuencias del FASTA, indicar la penalidad de gap (siempre negativa), correr Needleman–Wunsch, ver las matrices, ver el alineamiento final, guardarlo y exportar la matriz de direcciones en formato Graphviz.

Este proyecto lo hice yo, Cristian Morales, para aprender y entender mejor cómo funciona el alineamiento global de secuencias en bioinformática.