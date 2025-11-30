#!/bin/bash

# directorio base donde realmente tienes el laboratorio
BASE_DIR="/home/cristian/Escritorio/Codigos de c++ y aprendiendo weas/3430A224202330034/ayed_bash"

LAB_DIR="$BASE_DIR/Lab_10"
OUTPUT_DIR="$BASE_DIR/ayed_bash_salida"

echo "1) manejo de carpetas"

echo "verificando directorio: $LAB_DIR"

# validar existencia de Lab_10
if [ ! -d "$LAB_DIR" ]; then
    echo "error: no existe el directorio $LAB_DIR"
    echo "revisa que la carpeta se llame exactamente Lab_10"
    exit 1
fi

echo "directorio encontrado "

# crear carpeta de salida
if [ ! -d "$OUTPUT_DIR" ]; then
    echo "creando carpeta de salida: $OUTPUT_DIR"
    mkdir -p "$OUTPUT_DIR"
fi

echo ""
echo "2) listando contenido del laboratorio"
ls -l "$LAB_DIR"

echo ""
echo "3) copiando archivos a la carpeta de salida"
cp -r "$LAB_DIR"/* "$OUTPUT_DIR"/

echo "archivos copiados correctamente "

echo ""
echo "4) script finalizado correctamente "

