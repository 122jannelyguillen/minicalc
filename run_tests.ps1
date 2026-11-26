# Ejecuta todos los casos de prueba y compara la salida con el archivo .esperado
# Uso (Windows):  powershell -ExecutionPolicy Bypass -File run_tests.ps1

[Console]::OutputEncoding = [System.Text.Encoding]::UTF8
Set-Location $PSScriptRoot

$exe = Join-Path $PSScriptRoot "minicalc.exe"
if (-not (Test-Path $exe)) {
    Write-Host "No se encontró minicalc.exe. Compilando..."
    g++ -std=c++17 -o minicalc src/main.cpp src/lexer.cpp src/parser.cpp src/token.cpp
    if (-not (Test-Path $exe)) {
        Write-Host "Error: no se pudo compilar. Revisá la guía INSTALACION.md"
        exit 1
    }
}

$archivos = @(Get-ChildItem "tests\validos\*.calc") + @(Get-ChildItem "tests\errores\*.calc")
$total = 0
$ok = 0

foreach ($f in $archivos) {
    $total++
    $esperadoPath = [System.IO.Path]::ChangeExtension($f.FullName, ".esperado")
    $obtenido = ((& $exe $f.FullName) -join "`n").Trim()
    $esperado = ((Get-Content $esperadoPath -Encoding UTF8) -join "`n").Trim()
    $nombre = $f.Directory.Name + "\" + $f.Name

    if ($obtenido -eq $esperado) {
        Write-Host "[OK]    $nombre" -ForegroundColor Green
        $ok++
    } else {
        Write-Host "[FALLO] $nombre" -ForegroundColor Red
        Write-Host "  Esperado: $(($esperado -split "`n")[-1])"
        Write-Host "  Obtenido: $(($obtenido -split "`n")[-1])"
    }
}

Write-Host ""
Write-Host "Resultado: $ok de $total pruebas correctas"
if ($ok -ne $total) { exit 1 }
