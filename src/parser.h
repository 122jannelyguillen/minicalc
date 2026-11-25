// parser.h
// Analizador sintáctico descendente recursivo de Mini-Calc.
// Una función por cada no terminal de la gramática SIN recursión por la izquierda:
//
//   programa          -> lista_sentencias EOF
//   lista_sentencias  -> sentencia lista_sentencias | ε
//   sentencia         -> ID OP_ASSIGN expresion PUNCT_SEMI
//                      | KW_PRINT expresion PUNCT_SEMI
//   expresion         -> termino expresion'
//   expresion'        -> OP_ADD termino expresion' | OP_SUB termino expresion' | ε
//   termino           -> factor termino'
//   termino'          -> OP_MUL factor termino' | OP_DIV factor termino' | ε
//   factor            -> PUNCT_LPAREN expresion PUNCT_RPAREN | ID | NUM
#ifndef PARSER_H
#define PARSER_H

#include <vector>
#include "token.h"
using namespace std;

class Parser {
public:
    explicit Parser(const vector<Token>& tokens);

    // Verifica la estructura completa del programa.
    // Ante un error sintáctico lanza ErrorCompilacion con la ubicación exacta.
    void analizar();

private:
    vector<Token> toks;
    size_t actual;   // índice del token de preanálisis (lookahead)

    // Utilidades
    const Token& la() const;          // token de preanálisis
    const Token& anterior() const;    // último token consumido
    void consumir();
    string describir(const Token& t) const;

    // Una función por no terminal
    void programa();
    void listaSentencias();
    void sentencia();
    void expresion();
    void expresionPrima();
    void termino();
    void terminoPrima();
    void factor();
};

#endif
