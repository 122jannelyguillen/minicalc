# Guía de instalación y prueba — Mini-Calc

Compilador minimalista (analizador léxico + analizador sintáctico) para el lenguaje Mini-Calc, escrito en C++17 sin bibliotecas externas.

## 1. Requisitos

| Requisito | Detalle |
|---|---|
| Compilador C++ | `g++` con soporte de C++17 (versión 7 o superior) |
| Sistema operativo | Windows 10/11, Linux o macOS |
| Opcional | Git (para clonar el repositorio) y VS Code |

No hace falta instalar ninguna biblioteca adicional.

## 2. Instalar el compilador

### Windows

Opción A, con **winget** (viene con Windows 10/11). Abrir PowerShell y ejecutar:

```
winget install BrechtSanders.WinLibs.POSIX.UCRT
```

Opción B: instalar **MSYS2** desde https://www.msys2.org y, dentro de la terminal de MSYS2, ejecutar `pacman -S mingw-w64-ucrt-x86_64-gcc`. Después agregar `C:\msys64\ucrt64\bin` a la variable de entorno `PATH`.

Cerrar y volver a abrir la terminal, y comprobar que quedó instalado:

```
g++ --version
```

### Linux (Debian/Ubuntu)

```
sudo apt install g++ make
```

### macOS

```
xcode-select --install
```

## 3. Descargar el proyecto

Con Git:

```
git clone https://github.com/USUARIO/minicalc.git
cd minicalc
```

Sin Git: en la página del repositorio, **Code → Download ZIP**, descomprimir y abrir una terminal dentro de la carpeta `minicalc`.

## 4. Compilar

Desde la carpeta `minicalc`:

```
g++ -std=c++17 -o minicalc src/main.cpp src/lexer.cpp src/parser.cpp src/token.cpp
```

En Linux y macOS también se puede usar `make`. Si el comando no muestra ningún mensaje, la compilación fue correcta y se genera `minicalc.exe` (Windows) o `minicalc` (Linux/macOS).

## 5. Ejecutar

**Windows (PowerShell):**

```
.\minicalc.exe tests\validos\ejemplo.calc
```

**Linux / macOS:**

```
./minicalc tests/validos/ejemplo.calc
```

Si se ejecuta sin argumentos, el programa pide el nombre del archivo `.calc` por teclado.

Salida esperada:

```
=== INICIO DE ANÁLISIS LÉXICO ===
<ID, "x", 1, 1>
<OP_ASSIGN, "=", 1, 3>
<NUM, "10", 1, 5>
...
<PUNCT_SEMI, ";", 3, 8>

=== INICIO DE ANÁLISIS SINTÁCTICO ===
El análisis sintáctico finalizó exitosamente sin errores.
```

Ejemplo con error léxico:

```
.\minicalc.exe tests\errores\lex_dolar.calc
```

```
Error [Línea 2, Columna 7]: Error léxico - carácter no válido '$'
```

## 6. Probar todos los casos

La carpeta `tests/` tiene 15 casos: 4 programas válidos (`tests/validos`) y 11 con errores léxicos y sintácticos (`tests/errores`). Cada `.calc` tiene al lado su salida correcta en un archivo `.esperado`.

**Windows (PowerShell):**

```
powershell -ExecutionPolicy Bypass -File run_tests.ps1
```

**Linux / macOS / Git Bash:**

```
bash run_tests.sh
```

Al final debe mostrar: `Resultado: 15 de 15 pruebas correctas`.

## 7. Escribir un programa propio

Crear un archivo con extensión `.calc`, por ejemplo `prueba.calc`:

```
a = 3 * (4 + 2);
b = a / 2 - 1;
print b;
```

Y ejecutarlo:

```
.\minicalc.exe prueba.calc
```

Reglas del lenguaje: variables de una sola letra minúscula (`a`–`z`), números enteros positivos, operadores `+ - * / =`, paréntesis, `;` al final de cada sentencia y la palabra reservada `print`.

## 8. Demostración guiada

Para ver en orden la compilación, un programa válido, varios errores y el resumen de las pruebas:

```
powershell -ExecutionPolicy Bypass -File demo.ps1
```

El script se detiene entre cada paso; se avanza con Enter.

## 9. Problemas frecuentes

| Problema | Solución |
|---|---|
| `g++ no se reconoce como un comando` | Cerrar y volver a abrir la terminal después de instalar. Si sigue, agregar la carpeta `bin` del compilador al `PATH`. |
| `.\minicalc.exe no se reconoce` | La compilación falló o no se está en la carpeta `minicalc`. Volver al paso 4. |
| Tildes raras (`AnÃ¡lisis`, `L├¡nea`) | Ejecutar `[Console]::OutputEncoding = [System.Text.Encoding]::UTF8` en esa terminal. |
| `la ejecución de scripts está deshabilitada` | Usar `powershell -ExecutionPolicy Bypass -File ...` como en los pasos 6 y 8. |
| `Error: el archivo de entrada debe tener extensión .calc` | El programa solo acepta archivos `.calc`. |
