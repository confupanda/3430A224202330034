Este programa implementa cinco métodos de ordenamiento interno:

burbuja
inserción
selección
shellsort
quicksort

El programa genera un vector de números aleatorios y aplica cada algoritmo, midiendo su tiempo de ejecución en milisegundos.
Opcionalmente, puede mostrar los vectores antes y después del ordenamiento.

compilación

g++ Morales_Cristian.cpp -O2 -std=c++11 -o Morales_Cristian

ejecución

Formato:

./Morales_Cristian N VER

N: cantidad de elementos

VER: s para mostrar vectores, n para no mostrar

./Morales_Cristian 10 s
./Morales_Cristian 20000 n

salida

El programa muestra:

tiempos de ejecución de cada método

si VER = s, muestra también los vectores originales y ordenados