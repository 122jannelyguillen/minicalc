# Mini-Calc — Compilador minimalista

Trabajo Práctico Final de **Teoría de Autómatas** — Ingeniería en Informática, UNIDA.

Analizador léxico (AFD dirigido por tabla) y analizador sintáctico (descenso recursivo) para el lenguaje **Mini-Calc**, escritos en C++ sin bibliotecas de terceros ni generadores (sin Flex, Bison, ANTLR, etc.).

## Estructura

```
minicalc/
├── src/
│   ├── token.h / token.cpp     Tipos de token y estructura de error
│   ├── lexer.h / lexer.cpp     AFD: estados, clases de caracteres, tabla de transición
│   ├── parser.h / parser.cpp   Descenso recursivo: una función por no terminal
│   └── main.cpp                Lee el .calc y ejecuta las dos fases
├── tests/
│   ├── validos/                Programas correctos (+ salida .esperado)
│   └── errores/                Errores léxicos (lex_*) y sintácticos (sin_*)
├── INSTALACION.md              Guía de instalación, ejecución y prueba
├── run_tests.ps1               Pruebas automáticas (Windows / PowerShell)
├── run_tests.sh                Pruebas automáticas (Linux, macOS, Git Bash)
├── demo.ps1                    Demostración guiada paso a paso
└── Makefile
```

> **Guía completa de instalación y prueba:** ver [INSTALACION.md](INSTALACION.md).

## Compilar

Con g++ (Linux, macOS o Windows con MinGW):

```
g++ -std=c++17 -o minicalc src/main.cpp src/lexer.cpp src/parser.cpp src/token.cpp
```

o simplemente `make`.

## Ejecutar

```
./minicalc tests/validos/ejemplo.calc
```

En Windows (PowerShell): `.\minicalc.exe tests\validos\ejemplo.calc`. Si no se pasa el archivo, el programa lo pide por teclado.

## Pruebas

```
powershell -ExecutionPolicy Bypass -File run_tests.ps1     # Windows
bash run_tests.sh                                          # Linux, macOS, Git Bash
```

Corre los 15 casos de `tests/` y compara cada salida con su archivo `.esperado`.

## Gramática usada por el parser (sin recursión por la izquierda)

```
programa          -> lista_sentencias EOF
lista_sentencias  -> sentencia lista_sentencias | ε
sentencia         -> ID OP_ASSIGN expresion PUNCT_SEMI | KW_PRINT expresion PUNCT_SEMI
expresion         -> termino expresion'
expresion'        -> OP_ADD termino expresion' | OP_SUB termino expresion' | ε
termino           -> factor termino'
termino'          -> OP_MUL factor termino' | OP_DIV factor termino' | ε
factor            -> PUNCT_LPAREN expresion PUNCT_RPAREN | ID | NUM
```

## Integrantes

| Nombre | Aporte |
|---|---|
| Carlos Eduardo Báez Mendoza | Analizador sintáctico descendente recursivo |
| Alan Josué Chamorro Cabral | Casos de prueba y script de pruebas |
| Thiago Alejandro Da Silva Domínguez | Programa principal y reporte de errores |
| Jannely Magalí Guillén Capdevila | Estructura del proyecto y analizador léxico (AFD y tabla de transición) |
| Sergio Luis Marín Escobar | Guía de instalación, pruebas para Windows y script de demo |
| Fabricio Nicolás Ruiz Díaz Escobar | Definición de tokens y estructura de errores |
