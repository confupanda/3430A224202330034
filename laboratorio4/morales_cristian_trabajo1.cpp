
#include <iostream>
#include <string>
#include <fstream>
#include <limits>
#include <cstdlib>   // system
#include <functional>

using namespace std;

struct Nodo {
    int dato;
    Nodo* izq;
    Nodo* der;
};

// ---------- Creación / destrucción ----------
Nodo* crearNodo(int valor) {
    Nodo* n = new Nodo();
    n->dato = valor;
    n->izq = nullptr;
    n->der = nullptr;
    return n;
}

void liberarArbol(Nodo* root) {
    if (!root) return;
    liberarArbol(root->izq);
    liberarArbol(root->der);
    delete root;
}

// ---------- Búsqueda ----------
bool existe(Nodo* root, int valor) {
    Nodo* cur = root;
    while (cur) {
        if (valor == cur->dato) return true;
        if (valor < cur->dato) cur = cur->izq;
        else cur = cur->der;
    }
    return false;
}

// ---------- Inserción (iterativa) ----------
Nodo* insertarNodo(Nodo* root, int valor) {
    if (root == nullptr) {
        return crearNodo(valor);
    }
    Nodo* cur = root;
    Nodo* padre = nullptr;
    while (cur) {
        padre = cur;
        if (valor == cur->dato) {
            // Duplicado: no insertar
            cout << "El valor " << valor << " ya existe en el árbol. No se inserta.\n";
            return root;
        }
        if (valor < cur->dato) cur = cur->izq;
        else cur = cur->der;
    }
    if (valor < padre->dato) padre->izq = crearNodo(valor);
    else padre->der = crearNodo(valor);
    return root;
}

// ---------- Utilitaria: mínimo en subárbol ----------
Nodo* minimo(Nodo* root) {
    if (!root) return nullptr;
    while (root->izq) root = root->izq;
    return root;
}

// ---------- Eliminación (recursiva) ----------
Nodo* eliminarNodo(Nodo* root, int valor) {
    if (!root) {
        cout << "No se encontró el valor " << valor << " para eliminar.\n";
        return nullptr;
    }
    if (valor < root->dato) {
        root->izq = eliminarNodo(root->izq, valor);
    } else if (valor > root->dato) {
        root->der = eliminarNodo(root->der, valor);
    } else {
        // Nodo encontrado
        if (!root->izq && !root->der) {
            delete root;
            return nullptr;
        } else if (!root->izq) {
            Nodo* tmp = root->der;
            delete root;
            return tmp;
        } else if (!root->der) {
            Nodo* tmp = root->izq;
            delete root;
            return tmp;
        } else {
            // Dos hijos: usar sucesor (mínimo del subárbol derecho)
            Nodo* suc = minimo(root->der);
            root->dato = suc->dato;
            root->der = eliminarNodo(root->der, suc->dato);
        }
    }
    return root;
}

// ---------- Modificación: eliminar viejo e insertar nuevo ----------
Nodo* modificarNodo(Nodo* root, int viejo, int nuevo) {
    if (!existe(root, viejo)) {
        cout << "No existe el valor " << viejo << " que desea modificar.\n";
        return root;
    }
    if (existe(root, nuevo)) {
        cout << "No se puede modificar a " << nuevo << " porque ya existe en el árbol.\n";
        return root;
    }
    // Confirmación
    cout << "Confirma modificar " << viejo << " -> " << nuevo << " ? (s/n): ";
    char r; cin >> r;
    if (!(r == 's' || r == 'S')) {
        cout << "Modificación cancelada.\n";
        return root;
    }
    root = eliminarNodo(root, viejo);
    root = insertarNodo(root, nuevo);
    cout << "Modificación completada.\n";
    return root;
}

// ---------- Recorridos ----------
void preorden(Nodo* root) {
    if (!root) return;
    cout << root->dato << " ";
    preorden(root->izq);
    preorden(root->der);
}

void inorden(Nodo* root) {
    if (!root) return;
    inorden(root->izq);
    cout << root->dato << " ";
    inorden(root->der);
}

void posorden(Nodo* root) {
    if (!root) return;
    posorden(root->izq);
    posorden(root->der);
    cout << root->dato << " ";
}

// ---------- Generación de grafo ---
// - raíz: orange
// - hijo izquierdo: green
// - hijo derecho: lightblue
// Para nodos nulos se crean "puntos" invisibles para mantener forma
void generarGrafoRec(Nodo* nodo, Nodo* padre, char lado, ofstream &fp) {
    if (!nodo) return;

    // Definir estilo del nodo según su rol
    string color = "white";
    if (!padre) color = "orange";            // raíz
    else if (lado == 'L') color = "green";   // hijo izquierdo
    else if (lado == 'R') color = "lightblue"; // hijo derecho

    // Definir nodo con label y color
    fp << "\"" << nodo->dato << "\""
       << " [style=filled, fillcolor=" << color << ", shape=circle];\n";

    // Hijos (izq)
    if (nodo->izq) {
        fp << "\"" << nodo->dato << "\" -> \"" << nodo->izq->dato << "\";\n";
    } else {
        string punto = to_string(nodo->dato) + "i";
        fp << "\"" << punto << "\" [shape=point];\n";
        fp << "\"" << nodo->dato << "\" -> \"" << punto << "\";\n";
    }

    // Hijos (der)
    if (nodo->der) {
        fp << "\"" << nodo->dato << "\" -> \"" << nodo->der->dato << "\";\n";
    } else {
        string punto = to_string(nodo->dato) + "d";
        fp << "\"" << punto << "\" [shape=point];\n";
        fp << "\"" << nodo->dato << "\" -> \"" << punto << "\";\n";
    }

    // Recursión pasando información de rol (lado)
    generarGrafoRec(nodo->izq, nodo, 'L', fp);
    generarGrafoRec(nodo->der, nodo, 'R', fp);
}

void generarGrafo(Nodo* root, const string &archivo = "grafo.txt") {
    ofstream fp(archivo);
    if (!fp.is_open()) {
        cerr << "Error: no se pudo abrir " << archivo << " para escritura.\n";
        return;
    }
    fp << "digraph G {\n";
    fp << "rankdir=TB; // top to bottom\n";
    fp << "node [fontname=\"Helvetica\"];\n";

    if (!root) {
        fp << "// Arbol vacío\n";
    } else {
        generarGrafoRec(root, nullptr, 'X', fp);
    }

    fp << "}\n";
    fp.close();

    cout << "Archivo de grafo generado: " << archivo << "\n";

    // Intentar generar PNG y abrirlo (si dot está instalado)
    string png = "grafo.png";
    string cmd = "dot -Tpng -o " + png + " " + archivo;
    int rc = system(cmd.c_str());
    if (rc != 0) {
        cout << "Aviso: comando 'dot' devolvió código " << rc << ". Asegúrate de tener Graphviz instalado.\n";
    } else {
        // Abrir con visor común (intenta eog y xdg-open)
        system(("eog " + png + " >/dev/null 2>&1 &").c_str());
        system(("xdg-open " + png + " >/dev/null 2>&1 &").c_str());
    }
}

// ---------- Utilidades de lectura y menú ----------
int leerEntero(const string &mensaje) {
    int x;
    while (true) {
        cout << mensaje;
        if (cin >> x) return x;
        cout << "Entrada inválida. Ingrese un número entero.\n";
        cin.clear();
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
    }
}

void pausa() {
    cout << "\nPresiona Enter para continuar...";
    cin.ignore(numeric_limits<streamsize>::max(), '\n');
    cin.get();
}

void mostrarMenu() {
    cout << "\n\n";
    cout << "     ARBOL BINARIO DE BÚSQUEDA\n";
    cout << "\n";
    cout << "1) Insertar número\n";
    cout << "2) Eliminar número\n";
    cout << "3) Modificar número (viejo -> nuevo)\n";
    cout << "4) Mostrar recorridos (Pre, In, Pos)\n";
    cout << "5) Generar grafo (Graphviz)\n";
    cout << "6) Cargar ejemplo (datos del enunciado)\n";
    cout << "0) Salir\n";
    cout << "------------------------------------\n";
}

// ---------- Ejemplo predeterminado (del PDF) ----------
void cargarEjemplo(Nodo* &root) {
    int datos[] = {120, 87, 140, 43, 99, 130, 22, 65, 93, 135, 56};
    int n = sizeof(datos) / sizeof(datos[0]);
    for (int i = 0; i < n; ++i) {
        if (!existe(root, datos[i])) root = insertarNodo(root, datos[i]);
    }
    cout << "Ejemplo cargado (" << n << " valores).\n";
}

// ---------- Programa principal ----------
int main() {
    Nodo* raiz = nullptr;
    int opcion = -1;

    while (true) {
        mostrarMenu();
        opcion = leerEntero("Ingrese opción: ");

        if (opcion == 0) {
            cout << "Saliendo y liberando memoria...\n";
            break;
        }

        switch (opcion) {
            case 1: {
                int v = leerEntero("Ingrese número a insertar: ");
                if (!existe(raiz, v)) raiz = insertarNodo(raiz, v);
                else cout << "No se permiten duplicados. El valor ya existe.\n";
                break;
            }
            case 2: {
                int v = leerEntero("Ingrese número a eliminar: ");
                if (!existe(raiz, v)) {
                    cout << "Valor no encontrado.\n";
                } else {
                    cout << "Confirma eliminación de " << v << " ? (s/n): ";
                    char r; cin >> r;
                    if (r == 's' || r == 'S') {
                        raiz = eliminarNodo(raiz, v);
                        cout << "Eliminación realizada.\n";
                    } else {
                        cout << "Eliminación cancelada.\n";
                    }
                }
                break;
            }
            case 3: {
                int viejo = leerEntero("Ingrese valor antiguo a modificar: ");
                int nuevo = leerEntero("Ingrese nuevo valor: ");
                raiz = modificarNodo(raiz, viejo, nuevo);
                break;
            }
            case 4: {
                cout << "\n-- Preorden --\n";
                preorden(raiz); cout << "\n";
                cout << "-- Inorden --\n";
                inorden(raiz); cout << "\n";
                cout << "-- Posorden --\n";
                posorden(raiz); cout << "\n";
                break;
            }
            case 5: {
                if (!raiz) cout << "Árbol vacío. Nada que graficar.\n";
                else generarGrafo(raiz, "grafo.txt");
                break;
            }
            case 6: {
                cargarEjemplo(raiz);
                break;
            }
            default:
                cout << "Opción inválida. Intente nuevamente.\n";
        }

        // limpiar buffer y pausa
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
        cout << "\n";
    }

    liberarArbol(raiz);
    return 0;
}
