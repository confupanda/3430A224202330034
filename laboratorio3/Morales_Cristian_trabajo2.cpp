#include <iostream>
#include <fstream>
#include <string>
#include <cstdlib>
#include <set>
#include <algorithm>
using namespace std;

struct Nodo {
    string resn;   // nombre del residuo (ej: ASP, MET…)
    string resid;  // número del residuo (ej: 157, 158…)
    Nodo* siguiente;
};

// Insertar al final
void insertarResiduo(Nodo*& cabeza, string resn, string resid) {
    Nodo* nuevo = new Nodo();
    nuevo->resn = resn;
    nuevo->resid = resid;
    nuevo->siguiente = nullptr;

    if (cabeza == nullptr) {
        cabeza = nuevo;
    } else {
        Nodo* aux;
        for (aux = cabeza; aux->siguiente != nullptr; aux = aux->siguiente);
        aux->siguiente = nuevo;
    }
}

// Mostrar lista
void mostrarLista(Nodo* cabeza) {
    if (cabeza == nullptr) {
        cout << "La lista está vacía.\n";
        return;
    }
    cout << "\nSecuencia de residuos:\n";
    int pos = 1;
    for (Nodo* aux = cabeza; aux != nullptr; aux = aux->siguiente, pos++) {
        cout << pos << ". " << aux->resn << aux->resid << endl;
    }
}

// Modificar resn (mutación)
bool modificarResiduo(Nodo* cabeza, int posicion, string nuevoResn) {
    int contador = 1;
    for (Nodo* aux = cabeza; aux != nullptr; aux = aux->siguiente, contador++) {
        if (contador == posicion) {
            aux->resn = nuevoResn;
            return true;
        }
    }
    return false;
}

// Eliminar residuo en posición
bool eliminarResiduo(Nodo*& cabeza, int posicion) {
    if (cabeza == nullptr || posicion < 1) return false;

    if (posicion == 1) {
        Nodo* aux = cabeza;
        cabeza = aux->siguiente;
        delete aux;
        return true;
    }

    Nodo* anterior = nullptr;
    Nodo* actual = cabeza;
    int contador = 1;
    for (; actual != nullptr && contador < posicion; contador++) {
        anterior = actual;
        actual = actual->siguiente;
    }

    if (actual == nullptr) return false;

    anterior->siguiente = actual->siguiente;
    delete actual;
    return true;
}

// Exportar a Graphviz
void exportarGraphviz(Nodo* cabeza, string archivoDot, string archivoPng) {
    ofstream outfile(archivoDot);
    if (!outfile.is_open()) {
        cerr << "No se pudo crear el archivo " << archivoDot << "\n";
        return;
    }

    outfile << "digraph G {\n";
    outfile << "rankdir=LR;\n";
    outfile << "node [style=filled, fillcolor=yellow];\n";

    for (Nodo* aux = cabeza; aux != nullptr && aux->siguiente != nullptr; aux = aux->siguiente) {
        outfile << aux->resn << aux->resid << " -> "
                << aux->siguiente->resn << aux->siguiente->resid << ";\n";
    }

    outfile << "}\n";
    outfile.close();

    cout << "+ Archivo graphviz generado: " << archivoDot << "\n";

    string comando = "dot -Tpng " + archivoDot + " -o " + archivoPng;
    int resultado = system(comando.c_str());
    if (resultado == 0) {
        cout << "+ Imagen graphviz generada: " << archivoPng << "\n";
    } else {
        cout << "+ [ERROR]: Revise que esté instalado Graphviz.\n";
    }
}

// Leer residuos de un archivo PDB según cadena y rango
void cargarProteina(Nodo*& cabeza, string archivoPdb, char cadena, int inicio = -1, int fin = -1) {
    ifstream file(archivoPdb);
    if (!file.is_open()) {
        cerr << "No se pudo abrir el archivo PDB.\n";
        return;
    }

    set<string> vistos;
    string line;
    while (getline(file, line)) {
        if (line.rfind("ATOM", 0) == 0) {
            string resn = line.substr(17, 3);
            string chain = line.substr(21, 1);
            string resid = line.substr(22, 4);

            // limpiar espacios
            resn.erase(remove(resn.begin(), resn.end(), ' '), resn.end());
            resid.erase(remove(resid.begin(), resid.end(), ' '), resid.end());

            if (chain.size() > 0 && chain[0] == cadena) {
                int residNum = stoi(resid);

                // Si se definió un rango, filtrar
                if ((inicio != -1 && residNum < inicio) || (fin != -1 && residNum > fin))
                    continue;

                string clave = resn + resid;
                if (vistos.find(clave) == vistos.end()) {
                    insertarResiduo(cabeza, resn, resid);
                    vistos.insert(clave);
                }
            }
        }
    }
    file.close();
    cout << "+ Se cargó la proteína desde " << archivoPdb 
         << " (cadena " << cadena << ") correctamente.\n";
}

int main() {
    Nodo* cabeza = nullptr;
    int opcion;
    string archivo;
    char cadena;
    string nuevoResn;
    int pos, inicio, fin;
    bool cargada = false;

    cout << "=== CARGA DE PROTEINA PDB ===\n";
    cout << "Ingrese nombre del archivo .pdb (ej: 9L4L.pdb): ";
    getline(cin, archivo);

    cout << "Ingrese la cadena que desea cargar (ej: A): ";
    cin >> cadena;

    cout << "Desea cargar:\n";
    cout << "1. Toda la cadena\n";
    cout << "2. Solo un rango de residuos\n";
    cout << "Seleccione: ";
    int tipoCarga;
    cin >> tipoCarga;

    if (tipoCarga == 1) {
        cargarProteina(cabeza, archivo, cadena);
    } else {
        cout << "Ingrese residuo inicial: ";
        cin >> inicio;
        cout << "Ingrese residuo final: ";
        cin >> fin;
        cargarProteina(cabeza, archivo, cadena, inicio, fin);
    }
    cargada = true;

    do {
        cout << "\n=== MENU LISTA DE PROTEINA ===\n";
        cout << "1. Mostrar lista de residuos\n";
        cout << "2. Modificar residuo (mutación)\n";
        cout << "3. Eliminar residuo en posición\n";
        cout << "4. Exportar a Graphviz y generar PNG\n";
        cout << "5. Salir\n";
        cout << "Seleccione una opción: ";
        cin >> opcion;
        cin.ignore();

        switch (opcion) {
            case 1:
                mostrarLista(cabeza);
                break;
            case 2:
                cout << "Posición a modificar: ";
                cin >> pos;
                cin.ignore();
                cout << "Nuevo nombre de residuo: ";
                getline(cin, nuevoResn);
                if (modificarResiduo(cabeza, pos, nuevoResn))
                    cout << "Residuo modificado.\n";
                else
                    cout << "Posición inválida.\n";
                break;
            case 3:
                cout << "Posición a eliminar: ";
                cin >> pos;
                if (eliminarResiduo(cabeza, pos))
                    cout << "Residuo eliminado.\n";
                else
                    cout << "Posición inválida.\n";
                break;
            case 4:
                exportarGraphviz(cabeza, "Estructura_primaria.dot", "Estructura_primaria_.png");
                break;
            case 5:
                cout << "Saliendo...\n";
                break;
            default:
                cout << "Opción inválida.\n";
        }
    } while (opcion != 5);

    return 0;
}
