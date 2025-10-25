
#include <bits/stdc++.h>
using namespace std;

// -----------------------------
// ESTRUCTURA DE DATOS
// -----------------------------
struct Node {
    string go;        // clave (ej. GO:0000001)
    string func;      // descripción
    double score;     // puntaje
    Node* left;
    Node* right;
    int height;
};

// -----------------------------
// UTILIDADES (trim, escape, split)
// -----------------------------
static inline string trim(const string &s) {
    size_t a = s.find_first_not_of(" \t\r\n");
    if (a == string::npos) return "";
    size_t b = s.find_last_not_of(" \t\r\n");
    return s.substr(a, b - a + 1);
}

// Escapa comillas y barras para labels en Graphviz
static inline string gv_escape(const string &s) {
    string r;
    for (char c : s) {
        if (c == '"' || c == '\\') { r.push_back('\\'); r.push_back(c); }
        else if (c == '\n' || c == '\r') { r.push_back(' '); }
        else r.push_back(c);
    }
    return r;
}

// -----------------------------
// PARSEO CSV (manejo de comillas, comas dentro de campos)
// -----------------------------
bool parse_csv_line(const string &line, vector<string> &fields) {
    fields.clear();
    string cur;
    bool inQuotes = false;
    for (size_t i = 0; i < line.size(); ++i) {
        char c = line[i];
        if (inQuotes) {
            if (c == '"') {
                if (i + 1 < line.size() && line[i+1] == '"') { cur.push_back('"'); ++i; }
                else inQuotes = false;
            } else cur.push_back(c);
        } else {
            if (c == '"') inQuotes = true;
            else if (c == ',') { fields.push_back(trim(cur)); cur.clear(); }
            else cur.push_back(c);
        }
    }
    fields.push_back(trim(cur));
    return true;
}

// Carga CSV con formato: GO,Function,Score (header opcional). Devuelve vector de tuplas.
bool load_csv_file(const string &fname, vector<tuple<string,string,double>> &outTerms) {
    ifstream in(fname.c_str());
    if (!in.is_open()) {
        cerr << "[ERROR] No se pudo abrir archivo CSV: " << fname << "\n";
        return false;
    }
    string line;
    bool firstLine = true;
    size_t lineno = 0;
    while (getline(in, line)) {
        ++lineno;
        if (line.empty()) continue;
        replace(line.begin(), line.end(), ';', ',');
        vector<string> cols;
        parse_csv_line(line, cols);
        if (cols.empty()) continue;
        if (firstLine) {
            string lower = cols[0];
            for (char &ch : lower) ch = tolower(ch);
            // detectar header heurísticamente
            if (lower.find("go") != string::npos || lower.find("function") != string::npos || lower.find("score") != string::npos) {
                firstLine = false;
                continue; // salta header
            }
        }
        firstLine = false;
        if (cols.size() < 3) {
            cerr << "[WARN] Línea " << lineno << " ignorada (menos de 3 columnas): '" << line << "'\n";
            continue;
        }
        string go = trim(cols[0]);
        string func = trim(cols[1]);
        double score = 0.0;
        try { score = stod(cols[2]); } catch (...) { score = 0.0; }
        if (go.empty()) {
            cerr << "[WARN] Línea " << lineno << " ignorada (GO vacío)\n";
            continue;
        }
        outTerms.emplace_back(go, func, score);
    }
    in.close();
    return true;
}

// -----------------------------
// FUNCIONES DEL AVL (sin clases)
// -----------------------------
int heightNode(Node* n) { return n ? n->height : 0; }
int maxInt(int a, int b) { return (a > b) ? a : b; }

// FE con convención configurable:
// FE = altura(derecha) - altura(izquierda)  (default 'right-left')
// o FE = altura(izquierda) - altura(derecha) ('left-right')
enum FEConvention { RIGHT_MINUS_LEFT, LEFT_MINUS_RIGHT };
FEConvention GLOBAL_FE_CONV = RIGHT_MINUS_LEFT;

int getBalance(Node* n) {
    if (!n) return 0;
    if (GLOBAL_FE_CONV == RIGHT_MINUS_LEFT)
        return heightNode(n->right) - heightNode(n->left);
    else
        return heightNode(n->left) - heightNode(n->right);
}

// Crear nodo
Node* create_node(const string &go, const string &func, double score) {
    Node* n = new Node();
    n->go = go; n->func = func; n->score = score;
    n->left = n->right = nullptr;
    n->height = 1;
    return n;
}

// Rotaciones (las condiciones de rotación están escritas considerando la forma general
// de FE; funcionan con ambas convenciones si las condiciones comparan claves)
Node* rightRotate(Node* y) {
    Node* x = y->left;
    Node* T2 = x->right;
    x->right = y;
    y->left = T2;
    y->height = 1 + maxInt(heightNode(y->left), heightNode(y->right));
    x->height = 1 + maxInt(heightNode(x->left), heightNode(x->right));
    return x;
}

Node* leftRotate(Node* x) {
    Node* y = x->right;
    Node* T2 = y->left;
    y->left = x;
    x->right = T2;
    x->height = 1 + maxInt(heightNode(x->left), heightNode(x->right));
    y->height = 1 + maxInt(heightNode(y->left), heightNode(y->right));
    return y;
}

// Insertar (sin duplicados). Si existe clave, inserted=false y no se duplica.
Node* insertNode(Node* node, const string &go, const string &func, double score, bool &inserted) {
    if (!node) { inserted = true; return create_node(go, func, score); }

    if (go == node->go) {
        inserted = false;
        return node;
    }
    if (go < node->go) node->left = insertNode(node->left, go, func, score, inserted);
    else node->right = insertNode(node->right, go, func, score, inserted);

    node->height = 1 + maxInt(heightNode(node->left), heightNode(node->right));
    int balance = getBalance(node);

    // Rotaciones: debemos usar comparaciones de claves (go) para detectar caso LL/LR/RR/RL.
    // Caso Left-Left
    if (balance < -1 && go < node->left->go) return rightRotate(node);
    // Caso Right-Right
    if (balance > 1 && go > node->right->go) return leftRotate(node);
    // Left-Right
    if (balance < -1 && go > node->left->go) {
        node->left = leftRotate(node->left);
        return rightRotate(node);
    }
    // Right-Left
    if (balance > 1 && go < node->right->go) {
        node->right = rightRotate(node->right);
        return leftRotate(node);
    }
    return node;
}

// Buscar por clave
Node* searchNode(Node* root, const string &go) {
    if (!root) return nullptr;
    if (go == root->go) return root;
    if (go < root->go) return searchNode(root->left, go);
    return searchNode(root->right, go);
}

// Recorridos
void inorderPrint(Node* root) {
    if (!root) return;
    inorderPrint(root->left);
    int fe = getBalance(root);
    cout << root->go << " | " << root->func << " | Score: " << fixed << setprecision(2)
         << root->score << " | FE: " << fe << " | H: " << root->height << "\n";
    inorderPrint(root->right);
}

void preorderCollect(Node* root, vector<Node*> &out) {
    if (!root) return;
    out.push_back(root);
    preorderCollect(root->left, out);
    preorderCollect(root->right, out);
}

// -----------------------------
// GENERAR GRAPHVIZ (DOT)

// Escribe el grafo Graphviz con nodos y puntos nulos
void write_graphviz(Node* root, const string &filename) {
    ofstream out(filename.c_str());
    if (!out.is_open()) {
        cerr << "[ERROR] No se pudo crear archivo DOT: " << filename << "\n";
        return;
    }

    out << "digraph AVL {\n";
    out << "  node [style=filled, shape=box, fillcolor=lightyellow];\n\n";

    vector<Node*> nodes;
    preorderCollect(root, nodes);
    unordered_map<Node*, string> id;
    for (size_t i = 0; i < nodes.size(); ++i)
        id[nodes[i]] = "n" + to_string(i);

    // Nodos con etiquetas
    for (auto *n : nodes) {
        int fe = getBalance(n);
        string label = gv_escape(n->func) + "\\nScore: " + to_string(n->score)
                     + "\\nFE: " + to_string(fe);
        out << "  " << id[n] << " [label=\"" << label << "\"];\n";
    }
    out << "\n";

    // Enlaces
    int nullCount = 0;
    for (auto *n : nodes) {
        if (n->left) {
            out << "  " << id[n] << " -> " << id[n->left] << ";\n";
        } else {
            string nid = "null" + to_string(nullCount++);
            out << "  " << nid << " [shape=point];\n";
            out << "  " << id[n] << " -> " << nid << ";\n";
        }

        if (n->right) {
            out << "  " << id[n] << " -> " << id[n->right] << ";\n";
        } else {
            string nid = "null" + to_string(nullCount++);
            out << "  " << nid << " [shape=point];\n";
            out << "  " << id[n] << " -> " << nid << ";\n";
        }
    }

    out << "}\n";
    out.close();
    cout << "[OK] Grafo escrito en: " << filename << "\n";
    cout << "    Usa: dot -Tpng -ografo.png " << filename << " && xdg-open grafo.png\n";
}

// Liberar memoria
void free_tree(Node* root) {
    if (!root) return;
    free_tree(root->left);
    free_tree(root->right);
    delete root;
}

// -----------------------------
// MENÚ INTERACTIVO
// -----------------------------
void show_menu() {
    cout << "\n==============================\n";
    cout << "     MENÚ ÁRBOL AVL GO\n";
    cout << "==============================\n";
    cout << "1) Insertar término manualmente\n";
    cout << "2) Buscar por código GO\n";
    cout << "3) Mostrar recorrido inorder\n";
    cout << "4) Generar archivo DOT (grafo.txt)\n";
    cout << "5) Generar PNG con Graphviz (si instalado)\n";
    cout << "6) Salir\n";
    cout << "------------------------------\n";
    cout << "Seleccione opción: ";
}

void menu_loop(Node*& root) {
    while (true) {
        show_menu();
        int opt;
        if (!(cin >> opt)) {
            cin.clear();
            string tmp;
            getline(cin, tmp);
            cout << "[!] Entrada no válida\n";
            continue;
        }
        cout << "\n";

        if (opt == 1) {
            string go, func; double score;
            cout << "Código GO: "; cin >> ws; getline(cin, go);
            cout << "Función biológica: "; getline(cin, func);
            cout << "Score: "; cin >> score;
            bool inserted = false;
            root = insertNode(root, go, func, score, inserted);
            if (inserted) cout << "[OK] Insertado: " << go << "\n";
            else cout << "[WARN] Clave duplicada, no insertada.\n";
        }

        else if (opt == 2) {
            string go; cout << "Código GO a buscar: "; cin >> ws; getline(cin, go);
            Node* f = searchNode(root, go);
            if (f) cout << "[OK] Encontrado: " << f->go << " | " << f->func << " | Score: "
                        << f->score << " | FE: " << getBalance(f) << "\n";
            else cout << "[X] No se encontró ese GO.\n";
        }

        else if (opt == 3) {
            cout << "Recorrido inorder:\n";
            inorderPrint(root);
        }

        else if (opt == 4) {
            write_graphviz(root, "grafo.txt");
        }

        else if (opt == 5) {
            write_graphviz(root, "grafo.txt");
            int r = system("dot -Tpng -ografo.png grafo.txt");
            if (r == 0) system("xdg-open grafo.png &");
            else cerr << "[ERROR] No se pudo generar PNG. ¿Graphviz instalado?\n";
        }

        else if (opt == 6) {
            cout << "Saliendo...\n";
            break;
        }

        else cout << "[!] Opción no válida.\n";
    }
}

// -----------------------------
// MAIN Y ARGUMENTOS
// -----------------------------
void show_help(const string &prog) {
    cout << "Uso: " << prog << " [--file archivo.csv] [--output grafo.txt] [--fe-convention left|right]\n";
    cout << "Ejemplo: " << prog << " --file Funciones_GO_minimal.csv --fe-convention right\n";
    cout << "Sin parámetros, puedes insertar manualmente desde el menú.\n";
}

int main(int argc, char** argv) {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    string file = "";
    string outdot = "grafo.txt";
    string feconv = "right";  // default

    // parseo simple de argumentos
    for (int i = 1; i < argc; ++i) {
        string arg = argv[i];
        if (arg == "--file" && i + 1 < argc) file = argv[++i];
        else if (arg == "--output" && i + 1 < argc) outdot = argv[++i];
        else if (arg == "--fe-convention" && i + 1 < argc) feconv = argv[++i];
        else if (arg == "-h" || arg == "--help") {
            show_help(argv[0]);
            return 0;
        } else {
            cerr << "[WARN] Argumento desconocido: " << arg << "\n";
        }
    }

    // configurar convención FE
    if (feconv == "left" || feconv == "izquierda") GLOBAL_FE_CONV = LEFT_MINUS_RIGHT;
    else GLOBAL_FE_CONV = RIGHT_MINUS_LEFT;

    Node* root = nullptr;

    // Cargar desde CSV si se especificó
    if (!file.empty()) {
        vector<tuple<string,string,double>> datos;
        if (load_csv_file(file, datos)) {
            cout << "[OK] Archivo cargado: " << file << " (" << datos.size() << " términos)\n";
            for (auto &t : datos) {
                string go, func; double score;
                tie(go, func, score) = t;
                bool inserted = false;
                root = insertNode(root, go, func, score, inserted);
                if (!inserted) {
                    Node* n = searchNode(root, go);
                    if (n) { n->func = func; n->score = score; }
                }
            }
        } else {
            cerr << "[ERROR] No se pudo cargar archivo, comenzando con árbol vacío.\n";
        }
    } else {
        cout << "[INFO] No se indicó archivo CSV. Puedes insertar manualmente.\n";
    }

    // Bucle principal
    menu_loop(root);

    // Limpieza
    free_tree(root);
    return 0;
}
