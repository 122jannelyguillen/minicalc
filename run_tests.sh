#!/bin/bash
# Ejecuta todos los casos de prueba y compara la salida con el archivo .esperado
# Uso:  ./run_tests.sh     (en Windows, desde Git Bash)

EXE=./minicalc
[ -f ./minicalc.exe ] && EXE=./minicalc.exe

total=0
ok=0
for f in tests/validos/*.calc tests/errores/*.calc; do
    total=$((total + 1))
    esperado="${f%.calc}.esperado"
    obtenido=$($EXE "$f" | tr -d '\r')
    if [ "$obtenido" == "$(tr -d '\r' < "$esperado")" ]; then
        echo "[OK]    $f"
        ok=$((ok + 1))
    else
        echo "[FALLO] $f"
        diff <(echo "$obtenido") <(tr -d '\r' < "$esperado")
    fi
done
echo
echo "Resultado: $ok de $total pruebas correctas"
[ $ok -eq $total ]
