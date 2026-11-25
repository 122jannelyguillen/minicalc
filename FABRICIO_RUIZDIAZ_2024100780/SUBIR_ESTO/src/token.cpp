// token.cpp
#include "token.h"

string nombreToken(TipoToken t) {
    switch (t) {
        case KW_PRINT:     return "KW_PRINT";
        case ID:           return "ID";
        case NUM:          return "NUM";
        case OP_ASSIGN:    return "OP_ASSIGN";
        case OP_ADD:       return "OP_ADD";
        case OP_SUB:       return "OP_SUB";
        case OP_MUL:       return "OP_MUL";
        case OP_DIV:       return "OP_DIV";
        case PUNCT_LPAREN: return "PUNCT_LPAREN";
        case PUNCT_RPAREN: return "PUNCT_RPAREN";
        case PUNCT_SEMI:   return "PUNCT_SEMI";
        case FIN_ARCHIVO:  return "EOF";
    }
    return "DESCONOCIDO";
}
