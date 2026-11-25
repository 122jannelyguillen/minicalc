// parser.cpp
// Descenso recursivo: cada función reconoce un no terminal y decide qué
// producción aplicar mirando un solo token de preanálisis (gramática LL(1)).
#include "parser.h"
using namespace std;

Parser::Parser(const vector<Token>& tokens) : toks(tokens), actual(0) {}

const Token& Parser::la() const { return toks[actual]; }

const Token& Parser::anterior() const { return toks[actual - 1]; }

void Parser::consumir() {
    if (la().tipo != FIN_ARCHIVO) actual++;
}

string Parser::describir(const Token& t) const {
    if (t.tipo == FIN_ARCHIVO) return "el fin del archivo";
    return "'" + t.lexema + "'";
}

void Parser::analizar() {
    programa();
}

// programa -> lista_sentencias EOF
void Parser::programa() {
    listaSentencias();
    if (la().tipo != FIN_ARCHIVO) {
        throw ErrorCompilacion{la().linea, la().columna,
            "Error sintáctico - se esperaba el inicio de una sentencia, se encontró " + describir(la())};
    }
}

// lista_sentencias -> sentencia lista_sentencias | ε
// PRIMERO(sentencia) = { ID, KW_PRINT }   SIGUIENTE(lista_sentencias) = { EOF }
void Parser::listaSentencias() {
    if (la().tipo == ID || la().tipo == KW_PRINT) {
        sentencia();
        listaSentencias();
    }
    // si no, se aplica la producción ε
}

// sentencia -> ID OP_ASSIGN expresion PUNCT_SEMI
//            | KW_PRINT expresion PUNCT_SEMI
void Parser::sentencia() {
    if (la().tipo == ID) {
        consumir();                                   // ID
        if (la().tipo != OP_ASSIGN) {
            throw ErrorCompilacion{la().linea, la().columna,
                "Error sintáctico - se esperaba '=' después de la variable '" +
                anterior().lexema + "', se encontró " + describir(la())};
        }
        consumir();                                   // OP_ASSIGN
        expresion();
    } else {
        consumir();                                   // KW_PRINT
        expresion();
    }

    // PUNCT_SEMI obligatorio al final de cada sentencia
    if (la().tipo == PUNCT_SEMI) {
        consumir();
        return;
    }
    if (la().tipo == PUNCT_RPAREN) {
        throw ErrorCompilacion{la().linea, la().columna,
            "Error sintáctico - paréntesis ')' sin su '(' correspondiente"};
    }
    // Falta ';': se reporta justo después del último token de la sentencia
    const Token& ult = anterior();
    throw ErrorCompilacion{ult.linea, ult.columna + (int)ult.lexema.size(),
        "Error sintáctico - falta ';' al final de la sentencia (se encontró " + describir(la()) + ")"};
}

// expresion -> termino expresion'
void Parser::expresion() {
    termino();
    expresionPrima();
}

// expresion' -> OP_ADD termino expresion' | OP_SUB termino expresion' | ε
// SIGUIENTE(expresion') = { PUNCT_RPAREN, PUNCT_SEMI }
void Parser::expresionPrima() {
    if (la().tipo == OP_ADD || la().tipo == OP_SUB) {
        consumir();          // + o -
        termino();
        expresionPrima();
    }
    // si no, ε
}

// termino -> factor termino'
void Parser::termino() {
    factor();
    terminoPrima();
}

// termino' -> OP_MUL factor termino' | OP_DIV factor termino' | ε
// SIGUIENTE(termino') = { OP_ADD, OP_SUB, PUNCT_RPAREN, PUNCT_SEMI }
void Parser::terminoPrima() {
    if (la().tipo == OP_MUL || la().tipo == OP_DIV) {
        consumir();          // * o /
        factor();
        terminoPrima();
    }
    // si no, ε
}

// factor -> PUNCT_LPAREN expresion PUNCT_RPAREN | ID | NUM
void Parser::factor() {
    if (la().tipo == ID || la().tipo == NUM) {
        consumir();
        return;
    }
    if (la().tipo == PUNCT_LPAREN) {
        Token abre = la();
        consumir();          // (
        expresion();
        if (la().tipo != PUNCT_RPAREN) {
            throw ErrorCompilacion{la().linea, la().columna,
                "Error sintáctico - se esperaba ')' para cerrar el paréntesis abierto en línea " +
                to_string(abre.linea) + ", columna " + to_string(abre.columna) +
                " (se encontró " + describir(la()) + ")"};
        }
        consumir();          // )
        return;
    }
    throw ErrorCompilacion{la().linea, la().columna,
        "Error sintáctico - se esperaba un número, una variable o '(', se encontró " + describir(la())};
}
