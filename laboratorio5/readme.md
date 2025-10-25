#  Árbol AVL de términos Gene Ontology (GO)

Este programa en **C++** implementa un **árbol AVL** para almacenar, buscar y visualizar términos de la **Gene Ontology (GO)** junto con su descripción y puntaje.  
Permite insertar datos manualmente o cargarlos desde un archivo CSV, realizar búsquedas, mostrar recorridos inorder, y generar visualizaciones gráficas en **Graphviz**.

---

##  Características principales

- Implementación de un **árbol AVL** balanceado (sin clases, usando estructuras).
- Carga de datos desde **archivos CSV** (con o sin cabecera).
- Inserción manual de nuevos términos.
- Búsqueda por código GO.
- Recorrido **inorder** del árbol.
- Generación automática de archivo **.dot** (para Graphviz).
- Exportación a imagen **PNG** del grafo (si Graphviz está instalado).
- Menú interactivo con limpieza de pantalla y validación de entradas.

---

##  Requisitos

- **Sistema operativo:** Linux, macOS o Windows (con pequeñas adaptaciones).
- **Compilador:** `g++` (C++11 o superior).
- **Opcional:** [Graphviz](https://graphviz.org/download/) para generar imágenes del árbol.

---

##  Compilación

En la terminal, dentro del directorio del proyecto:

```bash
g++ Morales_Cristian.cpp -o 2

./2 --file Funciones_GO.csv