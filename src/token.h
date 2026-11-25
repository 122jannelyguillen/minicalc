// token.h
// Definición de las categorías de tokens de Mini-Calc (Sección 2.b del enunciado)
// y de la estructura Token que produce el analizador léxico.
#ifndef TOKEN_H
#define TOKEN_H

#include <string>
using namespace std;

enum TipoToken {
    KW_PRINT,       // print
    ID,             // [a-z]
    NUM,            // [0-9]+
    OP_ASSIGN,      // =
    OP_ADD,         // +
    OP_SUB,         // -
    OP_MUL,         // *
    OP_DIV,         // /
    PUNCT_LPAREN,   // (
    PUNCT_RPAREN,   // )
    PUNCT_SEMI,     // ;
    FIN_ARCHIVO     // marca de fin de entrada (EOF), la agrega el lexer
};

struct Token {
    TipoToken tipo;
    string lexema;
    int linea;
    int columna;
};

// Devuelve el nombre del tipo de token tal como se imprime en la salida.
string nombreToken(TipoToken t);

// Excepción única para errores léxicos y sintácticos.
// Lleva la ubicación exacta (línea y columna) del error.
struct ErrorCompilacion {
    int linea;
    int columna;
    string mensaje;
};

#endif
