// main.cpp
// Compilador minimalista Mini-Calc: análisis léxico + análisis sintáctico.
// Uso:  minicalc archivo.calc

// En Windows, windows.h tiene que ir ANTES que cualquier "using namespace std;".
// Si no, su tipo 'byte' choca con std::byte de C++17 ("reference to 'byte' is ambiguous").
#ifdef _WIN32
#define WIN32_LEAN_AND_MEAN
#include <windows.h>
#endif

#include <iostream>
#include <fstream>
#include <sstream>
#include "lexer.h"
#include "parser.h"
using namespace std;

static void reportarError(const ErrorCompilacion& e) {
    cout << "Error [Línea " << e.linea << ", Columna " << e.columna << "]: "
         << e.mensaje << endl;
}

int main(int argc, char* argv[]) {
#ifdef _WIN32
    SetConsoleOutputCP(CP_UTF8);   // para ver bien las tildes en la consola de Windows
#endif

    string ruta;
    if (argc >= 2) {
        ruta = argv[1];
    } else {
        cout << "Archivo de entrada (.calc): ";
        cin >> ruta;
    }

    if (ruta.size() < 5 || ruta.substr(ruta.size() - 5) != ".calc") {
        cout << "Error: el archivo de entrada debe tener extensión .calc" << endl;
        return 1;
    }

    ifstream archivo(ruta, ios::binary);
    if (!archivo) {
        cout << "Error: no se pudo abrir el archivo '" << ruta << "'" << endl;
        return 1;
    }
    stringstream buffer;
    buffer << archivo.rdbuf();
    string fuente = buffer.str();

    // ---------------- Fase 1: análisis léxico ----------------
    cout << "=== INICIO DE ANÁLISIS LÉXICO ===" << endl;
    vector<Token> tokens;
    try {
        Lexer lexer(fuente);
        tokens = lexer.analizar(cout);
    } catch (const ErrorCompilacion& e) {
        reportarError(e);
        return 1;
    }

    // ---------------- Fase 2: análisis sintáctico ----------------
    cout << endl << "=== INICIO DE ANÁLISIS SINTÁCTICO ===" << endl;
    try {
        Parser parser(tokens);
        parser.analizar();
    } catch (const ErrorCompilacion& e) {
        reportarError(e);
        return 1;
    }

    cout << "El análisis sintáctico finalizó exitosamente sin errores." << endl;
    return 0;
}
