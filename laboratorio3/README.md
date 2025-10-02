## 🔹 Actividad 1: Cola de Estudiantes con Listas Dinámicas

### Archivo fuente:
`Morales_Cristian_trabajo1.cpp`

### Descripción:
Se implementa una **cola (queque)** utilizando listas dinámicas para gestionar información de estudiantes de un curso universitario.  

Cada estudiante contiene:
- `string nombre`
- `string carrera`
- `float promedio`

### Funcionalidades:
- **Insertar estudiante** (encolar al final de la cola).  
- **Eliminar estudiante** (desencolar desde el frente).  
- **Mostrar la cola completa**.  
- **Salir del programa**.  

### Compilación y ejecución en Linux:
```bash
g++ Morales_Cristian_trabajo1.cpp -o trabajo1
./trabajo1


Actividad 2: Lista Dinámica de Residuos de Proteína (con carga automática desde PDB)
Archivo fuente:

Morales_Cristian_trabajo2.cpp

Se implementa una lista dinámica que representa la estructura primaria de una proteína.
Los residuos se extraen automáticamente desde un archivo PDB, permitiendo cargar toda la cadena o un rango de posiciones específicas.

Cada nodo contiene:

string resn (nombre del residuo, ej: MET, ASP, GLY)

string resid (número del residuo, ej: 157, 158, 159…)

Funcionalidades:

Cargar automáticamente residuos desde un archivo .pdb:

Elegir la cadena (A, B, etc.).

O bien cargar solo un rango de posiciones (ej: del residuo 157 al 186).

Mostrar lista de residuos en consola.

Modificar un residuo (emulación de mutación).

Eliminar un residuo en una posición determinada.

Exportar la lista a formato Graphviz (.dot).

Generar automáticamente una imagen .png con la estructura primaria (usando dot -Tpng).

Ejemplo de salida Graphviz:

digraph G {
    rankdir=LR;
    node [style=filled, fillcolor=yellow];
    ASP157 -> ASN158;
    ASN158 -> ARG159;
    ...
}
Compilación y ejecución en Linux:

g++ Morales_Cristian_trabajo2.cpp -o trabajo2
./trabajo2
