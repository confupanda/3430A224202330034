# Laboratorio 6 — Algoritmo de Dijkstra
# ----------------------------------------

# Archivo
# Código fuente: Morales_Cristian.cpp
# Compilado: ./1

# Compilación
# En la terminal, dentro de la carpeta donde está el archivo:
g++ -std=c++17 -O2 -o 1 Morales_Cristian.cpp

# ▶Ejecución
# Ejecutar el programa indicando la cantidad de nodos (N):
./1 N

# Ejemplo:
./1 5

# Luego el programa pedirá:
# 1. El vértice origen (entre 1 y N)
# 2. La matriz de adyacencia (valores separados por espacios, usa -1 para indicar que no hay conexión)

#  Ejemplo completo:
./1 5
# Ingrese vértice origen (1..5): 1
# Ingrese la matriz 5x5 (use -1 para no arista):
# 0 4 11 -1 -1
# -1 0 -1 6 2
# -1 3 0 6 -1
# -1 -1 -1 0 -1
# -1 -1 5 3 0
