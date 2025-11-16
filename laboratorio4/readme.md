#  Árbol Binario de Búsqueda (ABB)

**Archivo fuente:** `cristian_morales_trabajo1.cpp`  
**Compilado:** `1`

##  Descripción general
Este programa implementa un **Árbol Binario de Búsqueda (ABB)** en C++ con operaciones básicas de gestión, recorridos y visualización mediante Graphviz.  
Permite al usuario insertar, eliminar, modificar y recorrer los elementos del árbol, así como generar un grafo visual que representa su estructura.

## Funcionalidades principales

1. **Insertar número:**  
   Agrega un nuevo valor al árbol siguiendo la propiedad del ABB (izquierda < raíz < derecha).  
   - No permite valores duplicados.

2. **Eliminar número:**  
   Elimina un nodo del árbol según las reglas de:
   - Nodo hoja.
   - Nodo con un hijo.
   - Nodo con dos hijos (usa el **sucesor inorden** para reemplazarlo).

3. **Modificar número:**  
   Reemplaza un valor existente por uno nuevo (elimina el antiguo y agrega el nuevo).  
   - Pide confirmación antes de realizar el cambio.

4. **Mostrar recorridos:**  
   Imprime en pantalla los tres recorridos clásicos:
   - **Preorden (raíz – izquierda – derecha)**
   - **Inorden (izquierda – raíz – derecha)**
   - **Postorden (izquierda – derecha – raíz)**

5. **Generar grafo (Graphviz):**  
   Crea un archivo `grafo.txt` con la descripción del árbol en formato DOT.  
   Luego genera una imagen `grafo.png` automáticamente si `Graphviz` está instalado.  
   - Raíz → color **naranjo**  
   - Hijo izquierdo → **verde**  
   - Hijo derecho → **celeste**

6. **Cargar ejemplo automático:**  
   Inserta un conjunto de valores predefinidos para pruebas:
