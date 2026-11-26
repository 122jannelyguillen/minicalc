# Demostración guiada de Mini-Calc (pensada para grabar el video)
# Uso (Windows):  powershell -ExecutionPolicy Bypass -File demo.ps1
# Avanza con Enter entre cada paso.

[Console]::OutputEncoding = [System.Text.Encoding]::UTF8
Set-Location $PSScriptRoot
$exe = Join-Path $PSScriptRoot "minicalc.exe"

function Paso($titulo) {
    Write-Host ""
    Write-Host "============================================================" -ForegroundColor Cyan
    Write-Host " $titulo" -ForegroundColor Cyan
    Write-Host "============================================================" -ForegroundColor Cyan
}

function Pausa {
    Write-Host ""
    Read-Host "Presione Enter para continuar" | Out-Null
}

function Mostrar($archivo) {
    Write-Host "--- Contenido de $archivo ---" -ForegroundColor Yellow
    Get-Content $archivo -Encoding UTF8 | ForEach-Object { Write-Host "    $_" }
    Write-Host "--- Salida del compilador ---" -ForegroundColor Yellow
    & $exe $archivo
}

Paso "1. Compilación del proyecto"
Write-Host "g++ -std=c++17 -o minicalc src/main.cpp src/lexer.cpp src/parser.cpp src/token.cpp"
g++ -std=c++17 -o minicalc src/main.cpp src/lexer.cpp src/parser.cpp src/token.cpp
if (Test-Path $exe) { Write-Host "Compilación correcta: se generó minicalc.exe" -ForegroundColor Green }
else { Write-Host "Error de compilación" -ForegroundColor Red; exit 1 }
Pausa

Paso "2. Programa válido: ejemplo del enunciado"
Mostrar "tests\validos\ejemplo.calc"
Pausa

Paso "3. Precedencia de operadores y paréntesis"
Mostrar "tests\validos\precedencia.calc"
Pausa

Paso "4. Error léxico: carácter no válido"
Mostrar "tests\errores\lex_dolar.calc"
Pausa

Paso "5. Error léxico: identificador de más de una letra"
Mostrar "tests\errores\lex_id_largo.calc"
Pausa

Paso "6. Error sintáctico: falta el punto y coma"
Mostrar "tests\errores\sin_falta_pyc.calc"
Pausa

Paso "7. Error sintáctico: paréntesis sin cerrar"
Mostrar "tests\errores\sin_falta_rparen.calc"
Pausa

Paso "8. Ejecución de las 15 pruebas automáticas"
& (Join-Path $PSScriptRoot "run_tests.ps1")

Write-Host ""
Write-Host "Fin de la demostración." -ForegroundColor Cyan
