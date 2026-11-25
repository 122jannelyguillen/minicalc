// lexer.cpp
// Implementación del AFD unificado de Mini-Calc.
// La función de transición delta: Q x Σ -> Q está codificada en la matriz TABLA,
// que es la misma "Tabla de transición" del informe técnico (Sección 3).
#include "lexer.h"
using namespace std;

// ---------------------------------------------------------------------------
// 1. Estados del autómata (Q)
// ---------------------------------------------------------------------------
enum Estado {
    Q0,   // inicial
    Q1,   // ID: una letra distinta de 'p'                   (final -> ID)
    Q2,   // leyó "p"                                        (final -> ID)
    Q3,   // leyó "pr"                                       (no final)
    Q4,   // leyó "pri"                                      (no final)
    Q5,   // leyó "prin"                                     (no final)
    Q6,   // leyó "print"                                    (final -> KW_PRINT)
    Q7,   // dos o más letras que no forman "print"          (no final, error)
    Q8,   // uno o más dígitos                               (final -> NUM)
    Q9,   // =                                               (final -> OP_ASSIGN)
    Q10,  // +                                               (final -> OP_ADD)
    Q11,  // -                                               (final -> OP_SUB)
    Q12,  // *                                               (final -> OP_MUL)
    Q13,  // /                                               (final -> OP_DIV)
    Q14,  // (                                               (final -> PUNCT_LPAREN)
    Q15,  // )                                               (final -> PUNCT_RPAREN)
    Q16,  // ;                                               (final -> PUNCT_SEMI)
    NUM_ESTADOS,
    ERR = -1  // "sin transición": el autómata se detiene
};

// ---------------------------------------------------------------------------
// 2. Alfabeto agrupado en clases de caracteres (columnas de la tabla)
// ---------------------------------------------------------------------------
enum Clase {
    C_P, C_R, C_I, C_N, C_T,   // letras de la palabra reservada "print"
    C_LETRA,                   // resto de letras [a-z] - {p,r,i,n,t}
    C_DIGITO,                  // [0-9]
    C_IGUAL, C_MAS, C_MENOS, C_POR, C_DIV,
    C_LPAREN, C_RPAREN, C_PYC,
    C_ESPACIO,                 // espacio, tabulación, salto de línea
    C_OTRO,                    // cualquier otro carácter (no pertenece a Σ)
    NUM_CLASES
};

static Clase clasificar(char c) {
    switch (c) {
        case 'p': return C_P;
        case 'r': return C_R;
        case 'i': return C_I;
        case 'n': return C_N;
        case 't': return C_T;
        case '=': return C_IGUAL;
        case '+': return C_MAS;
        case '-': return C_MENOS;
        case '*': return C_POR;
        case '/': return C_DIV;
        case '(': return C_LPAREN;
        case ')': return C_RPAREN;
        case ';': return C_PYC;
        case ' ': case '\t': case '\n': case '\r': return C_ESPACIO;
    }
    if (c >= 'a' && c <= 'z') return C_LETRA;
    if (c >= '0' && c <= '9') return C_DIGITO;
    return C_OTRO;
}

// ---------------------------------------------------------------------------
// 3. Tabla de transición  delta(estado, clase) -> estado
// ---------------------------------------------------------------------------
//  Columnas:  p    r    i    n    t    L    D    =    +    -    *    /    (    )    ;    ws   otro
static const int TABLA[NUM_ESTADOS][NUM_CLASES] = {
/* Q0  */ {  Q2,  Q1,  Q1,  Q1,  Q1,  Q1,  Q8,  Q9, Q10, Q11, Q12, Q13, Q14, Q15, Q16,  Q0, ERR },
/* Q1  */ {  Q7,  Q7,  Q7,  Q7,  Q7,  Q7, ERR, ERR, ERR, ERR, ERR, ERR, ERR, ERR, ERR, ERR, ERR },
/* Q2  */ {  Q7,  Q3,  Q7,  Q7,  Q7,  Q7, ERR, ERR, ERR, ERR, ERR, ERR, ERR, ERR, ERR, ERR, ERR },
/* Q3  */ {  Q7,  Q7,  Q4,  Q7,  Q7,  Q7, ERR, ERR, ERR, ERR, ERR, ERR, ERR, ERR, ERR, ERR, ERR },
/* Q4  */ {  Q7,  Q7,  Q7,  Q5,  Q7,  Q7, ERR, ERR, ERR, ERR, ERR, ERR, ERR, ERR, ERR, ERR, ERR },
/* Q5  */ {  Q7,  Q7,  Q7,  Q7,  Q6,  Q7, ERR, ERR, ERR, ERR, ERR, ERR, ERR, ERR, ERR, ERR, ERR },
/* Q6  */ {  Q7,  Q7,  Q7,  Q7,  Q7,  Q7, ERR, ERR, ERR, ERR, ERR, ERR, ERR, ERR, ERR, ERR, ERR },
/* Q7  */ {  Q7,  Q7,  Q7,  Q7,  Q7,  Q7, ERR, ERR, ERR, ERR, ERR, ERR, ERR, ERR, ERR, ERR, ERR },
/* Q8  */ { ERR, ERR, ERR, ERR, ERR, ERR,  Q8, ERR, ERR, ERR, ERR, ERR, ERR, ERR, ERR, ERR, ERR },
/* Q9  */ { ERR, ERR, ERR, ERR, ERR, ERR, ERR, ERR, ERR, ERR, ERR, ERR, ERR, ERR, ERR, ERR, ERR },
/* Q10 */ { ERR, ERR, ERR, ERR, ERR, ERR, ERR, ERR, ERR, ERR, ERR, ERR, ERR, ERR, ERR, ERR, ERR },
/* Q11 */ { ERR, ERR, ERR, ERR, ERR, ERR, ERR, ERR, ERR, ERR, ERR, ERR, ERR, ERR, ERR, ERR, ERR },
/* Q12 */ { ERR, ERR, ERR, ERR, ERR, ERR, ERR, ERR, ERR, ERR, ERR, ERR, ERR, ERR, ERR, ERR, ERR },
/* Q13 */ { ERR, ERR, ERR, ERR, ERR, ERR, ERR, ERR, ERR, ERR, ERR, ERR, ERR, ERR, ERR, ERR, ERR },
/* Q14 */ { ERR, ERR, ERR, ERR, ERR, ERR, ERR, ERR, ERR, ERR, ERR, ERR, ERR, ERR, ERR, ERR, ERR },
/* Q15 */ { ERR, ERR, ERR, ERR, ERR, ERR, ERR, ERR, ERR, ERR, ERR, ERR, ERR, ERR, ERR, ERR, ERR },
/* Q16 */ { ERR, ERR, ERR, ERR, ERR, ERR, ERR, ERR, ERR, ERR, ERR, ERR, ERR, ERR, ERR, ERR, ERR },
};

// ---------------------------------------------------------------------------
// 4. Estados finales (F) y el token que reconoce cada uno
// ---------------------------------------------------------------------------
static bool esFinal(int q) {
    return q == Q1 || q == Q2 || q == Q6 || (q >= Q8 && q <= Q16);
}

static TipoToken tokenDeEstado(int q) {
    switch (q) {
        case Q1: case Q2: return ID;
        case Q6:  return KW_PRINT;
        case Q8:  return NUM;
        case Q9:  return OP_ASSIGN;
        case Q10: return OP_ADD;
        case Q11: return OP_SUB;
        case Q12: return OP_MUL;
        case Q13: return OP_DIV;
        case Q14: return PUNCT_LPAREN;
        case Q15: return PUNCT_RPAREN;
        default:  return PUNCT_SEMI;   // Q16
    }
}

// ---------------------------------------------------------------------------
// 5. Motor del autómata
// ---------------------------------------------------------------------------
Lexer::Lexer(const string& fuente) : src(fuente), pos(0), linea(1), columna(1) {}

void Lexer::avanzar() {
    char c = src[pos];
    pos++;
    if (c == '\n') {
        linea++;
        columna = 1;
    } else if ((c & 0xC0) != 0x80) {
        // Solo cuenta columnas al inicio de un carácter UTF-8
        // (así 'ñ' o 'á' ocupan una sola columna).
        columna++;
    }
}

string Lexer::caracterEn(size_t p) const {
    unsigned char c = src[p];
    size_t largo = 1;
    if (c >= 0xF0) largo = 4;
    else if (c >= 0xE0) largo = 3;
    else if (c >= 0xC0) largo = 2;
    return src.substr(p, largo);
}

Token Lexer::siguienteToken() {
    int estado = Q0;
    string lexema = "";
    int linIni = linea, colIni = columna;

    while (pos < src.size()) {
        int clase = clasificar(src[pos]);
        int sig = TABLA[estado][clase];
        if (sig == ERR) break;               // no hay transición: el AFD se detiene

        if (estado == Q0 && sig == Q0) {     // espacio en blanco: se ignora
            avanzar();
            continue;
        }
        if (lexema.empty()) {                // primer carácter del token
            linIni = linea;
            colIni = columna;
        }
        lexema += src[pos];
        avanzar();
        estado = sig;
    }

    // El autómata se detuvo: decidir según el estado alcanzado.
    if (estado == Q0) {
        if (pos >= src.size()) {
            return Token{FIN_ARCHIVO, "EOF", linea, columna};
        }
        throw ErrorCompilacion{linea, columna,
            "Error léxico - carácter no válido '" + caracterEn(pos) + "'"};
    }

    if (esFinal(estado)) {
        return Token{tokenDeEstado(estado), lexema, linIni, colIni};
    }

    // Estados no finales Q3, Q4, Q5 (prefijo de "print") o Q7 (palabra inválida)
    if (estado == Q3 || estado == Q4 || estado == Q5) {
        throw ErrorCompilacion{linIni, colIni,
            "Error léxico - palabra incompleta '" + lexema + "' (¿quiso escribir 'print'?)"};
    }
    throw ErrorCompilacion{linIni, colIni,
        "Error léxico - identificador inválido '" + lexema +
        "': las variables deben ser una sola letra minúscula (a-z)"};
}

vector<Token> Lexer::analizar(ostream& salida) {
    vector<Token> tokens;
    while (true) {
        Token t = siguienteToken();
        tokens.push_back(t);
        if (t.tipo == FIN_ARCHIVO) break;
        salida << "<" << nombreToken(t.tipo) << ", \"" << t.lexema << "\", "
               << t.linea << ", " << t.columna << ">" << endl;
    }
    return tokens;
}
