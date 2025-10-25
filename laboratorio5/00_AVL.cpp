#include <iostream>
#include <fstream>
#include <cstdlib>
#include <cstring>

using namespace std;

/* estructura del nodo */
typedef struct NODO {
    NODO* izq;
    NODO* der;
    int info;
    int FE;
} NODO;

/* prototipos */
void InsercionBalanceado(NODO** nodocabeza, bool* BO, int infor);
void Busqueda(NODO* nodo, int infor);
void Restructura1(NODO** nodocabeza, bool* BO);
void Restructura2(NODO** nodocabeza, bool* BO);
void Borra(NODO** aux1, NODO** otro1, bool* BO);
void EliminacionBalanceado(NODO** nodocabeza, bool* BO, int infor);
int Menu();
void GenerarGrafo(NODO* p);
void PreOrden(NODO*, ofstream& fp);

/* Función principal */
int main() {
    int opcion;
    int elemento;
    NODO* raiz = nullptr;
    
    // system("clear");
    opcion = Menu();
    bool inicio;

    while (opcion) {
        switch (opcion) {
            case 1:
                cout << "Ingresar elemento: ";
                cin >> elemento;
                inicio = false;
                InsercionBalanceado(&raiz, &inicio, elemento);
                GenerarGrafo(raiz);
                break;

            case 2:
                cout << "Buscar elemento: ";
                cin >> elemento;
                Busqueda(raiz, elemento);
                break;

            case 3:
                cout << "Eliminar elemento: ";
                cin >> elemento;
                inicio = false;
                EliminacionBalanceado(&raiz, &inicio, elemento);
                GenerarGrafo(raiz);
                break;

            case 4:
                GenerarGrafo(raiz);
                break;

            case 0:
                return 0;
        }
        opcion = Menu();
    }
    return 0;
}

/* Menú */
int Menu() {
    int Op;
    do {
        cout << "\n--------------------\n";
        cout << "1) Insertar\n";
        cout << "2) Buscar\n";
        cout << "3) Eliminacion\n";
        cout << "4) Grafo\n";
        cout << "0) Salir\n\n";
        cout << "Opción: ";
        cin >> Op;
    } while (Op < 0 || Op > 4);
    return Op;
}

/* Inserción balanceada AVL */
void InsercionBalanceado(NODO** nodocabeza, bool* BO, int infor) {
    NODO* nodo = *nodocabeza;
    NODO* nodo1;
    NODO* nodo2;

    if (nodo != nullptr) {
        if (infor < nodo->info) {
            InsercionBalanceado(&(nodo->izq), BO, infor);
            if (*BO) {
                switch (nodo->FE) {
                    case 1:
                        nodo->FE = 0;
                        *BO = false;
                        break;
                    case 0:
                        nodo->FE = -1;
                        break;
                    case -1:
                        nodo1 = nodo->izq;
                        if (nodo1->FE <= 0) { // Rotación II
                            nodo->izq = nodo1->der;
                            nodo1->der = nodo;
                            nodo->FE = 0;
                            nodo = nodo1;
                        } else { // Rotación ID
                            nodo2 = nodo1->der;
                            nodo->izq = nodo2->der;
                            nodo2->der = nodo;
                            nodo1->der = nodo2->izq;
                            nodo2->izq = nodo1;

                            if (nodo2->FE == -1)
                                nodo->FE = 1;
                            else
                                nodo->FE = 0;

                            if (nodo2->FE == 1)
                                nodo1->FE = -1;
                            else
                                nodo1->FE = 0;
                            nodo = nodo2;
                        }
                        nodo->FE = 0;
                        *BO = false;
                        break;
                }
            }
        } else if (infor > nodo->info) {
            InsercionBalanceado(&(nodo->der), BO, infor);
            if (*BO) {
                switch (nodo->FE) {
                    case -1:
                        nodo->FE = 0;
                        *BO = false;
                        break;
                    case 0:
                        nodo->FE = 1;
                        break;
                    case 1:
                        nodo1 = nodo->der;
                        if (nodo1->FE >= 0) { // Rotación DD
                            nodo->der = nodo1->izq;
                            nodo1->izq = nodo;
                            nodo->FE = 0;
                            nodo = nodo1;
                        } else { // Rotación DI
                            nodo2 = nodo1->izq;
                            nodo->der = nodo2->izq;
                            nodo2->izq = nodo;
                            nodo1->izq = nodo2->der;
                            nodo2->der = nodo1;

                            if (nodo2->FE == 1)
                                nodo->FE = -1;
                            else
                                nodo->FE = 0;

                            if (nodo2->FE == -1)
                                nodo1->FE = 1;
                            else
                                nodo1->FE = 0;

                            nodo = nodo2;
                        }
                        nodo->FE = 0;
                        *BO = false;
                        break;
                }
            }
        } else {
            cout << "El nodo ya se encuentra en el árbol\n";
        }
    } else {
        nodo = new NODO();
        nodo->izq = nullptr;
        nodo->der = nullptr;
        nodo->info = infor;
        nodo->FE = 0;
        *BO = true;
    }

    *nodocabeza = nodo;
}

/* Búsqueda */
void Busqueda(NODO* nodo, int infor) {
    if (nodo != nullptr) {
        if (infor < nodo->info)
            Busqueda(nodo->izq, infor);
        else if (infor > nodo->info)
            Busqueda(nodo->der, infor);
        else
            cout << "El nodo SÍ se encuentra en el árbol\n";
    } else {
        cout << "El nodo NO se encuentra en el árbol\n";
    }
}

/* Reestructuras y eliminación (idénticas al código original, solo cambiando sintaxis) */
void Restructura1(NODO** nodocabeza, bool* BO) {
    NODO *nodo, *nodo1, *nodo2;
    nodo = *nodocabeza;
    if (*BO) {
        switch (nodo->FE) {
            case -1:
                nodo->FE = 0;
                break;
            case 0:
                nodo->FE = 1;
                *BO = false;
                break;
            case 1:
                nodo1 = nodo->der;
                if (nodo1->FE >= 0) { // Rotación DD
                    nodo->der = nodo1->izq;
                    nodo1->izq = nodo;
                    switch (nodo1->FE) {
                        case 0:
                            nodo->FE = 1;
                            nodo1->FE = -1;
                            *BO = false;
                            break;
                        case 1:
                            nodo->FE = 0;
                            nodo1->FE = 0;
                            *BO = false;
                            break;
                    }
                    nodo = nodo1;
                } else { // Rotación DI
                    nodo2 = nodo1->izq;
                    nodo->der = nodo2->izq;
                    nodo2->izq = nodo;
                    nodo1->izq = nodo2->der;
                    nodo2->der = nodo1;
                    if (nodo2->FE == 1)
                        nodo->FE = -1;
                    else
                        nodo->FE = 0;
                    if (nodo2->FE == -1)
                        nodo1->FE = 1;
                    else
                        nodo1->FE = 0;
                    nodo = nodo2;
                    nodo2->FE = 0;
                }
                break;
        }
    }
    *nodocabeza = nodo;
}

void Restructura2(NODO** nodocabeza, bool* BO) {
    NODO *nodo, *nodo1, *nodo2;
    nodo = *nodocabeza;
    if (*BO) {
        switch (nodo->FE) {
            case 1:
                nodo->FE = 0;
                break;
            case 0:
                nodo->FE = -1;
                *BO = false;
                break;
            case -1:
                nodo1 = nodo->izq;
                if (nodo1->FE <= 0) { // Rotación II
                    nodo->izq = nodo1->der;
                    nodo1->der = nodo;
                    switch (nodo1->FE) {
                        case 0:
                            nodo->FE = -1;
                            nodo1->FE = 1;
                            *BO = false;
                            break;
                        case -1:
                            nodo->FE = 0;
                            nodo1->FE = 0;
                            *BO = false;
                            break;
                    }
                    nodo = nodo1;
                } else { // Rotación ID
                    nodo2 = nodo1->der;
                    nodo->izq = nodo2->der;
                    nodo2->der = nodo;
                    nodo1->der = nodo2->izq;
                    nodo2->izq = nodo1;
                    if (nodo2->FE == -1)
                        nodo->FE = 1;
                    else
                        nodo->FE = 0;
                    if (nodo2->FE == 1)
                        nodo1->FE = -1;
                    else
                        nodo1->FE = 0;
                    nodo = nodo2;
                    nodo2->FE = 0;
                }
                break;
        }
    }
    *nodocabeza = nodo;
}

void Borra(NODO** aux1, NODO** otro1, bool* BO) {
    NODO *aux, *otro;
    aux = *aux1;
    otro = *otro1;
    if (aux->der != nullptr) {
        Borra(&(aux->der), &otro, BO);
        Restructura2(&aux, BO);
    } else {
        otro->info = aux->info;
        aux = aux->izq;
        *BO = true;
    }
    *aux1 = aux;
    *otro1 = otro;
}

void EliminacionBalanceado(NODO** nodocabeza, bool* BO, int infor) {
    NODO *nodo, *otro;
    nodo = *nodocabeza;
    if (nodo != nullptr) {
        if (infor < nodo->info) {
            EliminacionBalanceado(&(nodo->izq), BO, infor);
            Restructura1(&nodo, BO);
        } else if (infor > nodo->info) {
            EliminacionBalanceado(&(nodo->der), BO, infor);
            Restructura2(&nodo, BO);
        } else {
            otro = nodo;
            if (otro->der == nullptr) {
                nodo = otro->izq;
                *BO = true;
                delete otro;
            } else if (otro->izq == nullptr) {
                nodo = otro->der;
                *BO = true;
                delete otro;
            } else {
                Borra(&(otro->izq), &otro, BO);
                Restructura1(&nodo, BO);
                delete otro;
            }
        }
    } else {
        cout << "El nodo NO se encuentra en el árbol\n";
    }
    *nodocabeza = nodo;
}

/* Generar grafo con Graphviz */
void GenerarGrafo(NODO* ArbolInt) {
    if (!ArbolInt) return;
    ofstream fp("grafo.txt");
    fp << "digraph G {\n";
    fp << "node [style=filled fillcolor=yellow];\n";
    fp << "nullraiz [shape=point];\n";
    fp << "nullraiz -> " << ArbolInt->info << ";\n";
    PreOrden(ArbolInt, fp);
    fp << "}\n";
    fp.close();
    system("dot -Tpng -ografo.png grafo.txt");
    // system("eog grafo.png &");
}

/* Recorrido preorden para generar el grafo */
void PreOrden(NODO* a, ofstream& fp) {
    if (a != nullptr) {
        if (a->izq != nullptr)
            fp << a->info << " -> " << a->izq->info << ";\n";
        else
            fp << a->info << " -> " << "\"" << a->info << "i" << "\""<< " [shape=point];\n";
        if (a->der != nullptr)
            fp << a->info << " -> " << a->der->info << ";\n";
        else
            fp << a->info << " -> " << "\"" << a->info << "d" << "\""<< " [shape=point];\n";

        PreOrden(a->izq, fp);
        PreOrden(a->der, fp);
    }
}

