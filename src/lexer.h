// lexer.h
// Analizador léxico de Mini-Calc implementado como un AFD dirigido por tabla.
#ifndef LEXER_H
#define LEXER_H

#include <string>
#include <vector>
#include <iostream>
#include "token.h"
using namespace std;

class Lexer {
public:
    explicit Lexer(const string& fuente);

    // Recorre toda la fuente y devuelve la lista de tokens (terminada en FIN_ARCHIVO).
    // Cada token reconocido se escribe en 'salida' con el formato
    // <TIPO_TOKEN, "LEXEMA", LINEA, COLUMNA>.
    // Ante un error léxico lanza ErrorCompilacion con la línea y columna exactas.
    vector<Token> analizar(ostream& salida);

private:
    string src;     // contenido completo del archivo .calc
    size_t pos;     // índice del carácter actual
    int linea;      // línea actual (empieza en 1)
    int columna;    // columna actual (empieza en 1)

    void avanzar();
    void saltarEspacios();
    Token siguienteToken();
    string caracterEn(size_t p) const;  // devuelve el carácter (UTF-8 completo) en p
};

#endif
