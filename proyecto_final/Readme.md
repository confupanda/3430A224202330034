proyecto: alineamiento global con needleman–wunsch
1. descripcion general

este proyecto implementa una herramienta completa para realizar alineamientos globales de secuencias de adn usando el algoritmo needleman–wunsch, incorporando:

seleccion interactiva de secuencias dentro de archivos fasta

soporte para puntajes personalizados mediante matriz csv

generacion de informe del alineamiento en alineamiento.txt

generacion opcional de graficos usando archivos .dot (graphviz)

vista del alineamiento en pantalla

inspeccion de matrices internas del algoritmo

2. estructura recomendada del proyecto
proyecto_final/
│
├── needleman_complete.cpp
│
├── gene.fna
├── dataset_catalog.json
├── data_report.jsonl
│
└── hbb_datasets/


solo el archivo fasta es obligatorio para la ejecucion.

3. compilacion

utilizar un compilador con soporte para c++17:

g++ -std=c++17 -O2 -o needleman_complete needleman_complete.cpp


el ejecutable generado se llamara needleman_complete.

4. modos de ejecucion
4.1 modo -w

alinear dos secuencias dentro del mismo archivo fasta:

./needleman_complete -w archivo.fasta


flujo del modo -w:

lectura de cabeceras

eleccion interactiva de 2 indices

extraccion de secuencias

alineamiento global

generacion de salida

4.2 modo -c

comparar secuencias de dos archivos fasta distintos:

./needleman_complete -c archivo1.fasta archivo2.fasta


flujo del modo -c:

lectura de cabeceras de ambos archivos

seleccion de un indice por archivo

alineamiento global entre secuencias

generacion de salida

5. parametros opcionales

se puede incluir una matriz de puntuacion en formato csv:

./needleman_complete -w archivo.fasta matriz.csv


o especificar penalidad de gap:

./needleman_complete -w archivo.fasta matriz.csv -3


valores por defecto si no se especifica matriz:

match: +1

mismatch: -1

gap: -2

6. salidas generadas
6.1 archivo principal: alineamiento.txt

contiene el alineamiento completo

dividido en bloques de 60 caracteres

incluye linea intermedia marcando coincidencias con |

ejemplo:

secuencia 1: ATGCTAGCTAGC
             || | |  | ||
secuencia 2: AT-CTGGCTAGC

6.2 archivo graphviz: alineamiento.dot

generado solo si se selecciona en el menu.

permite exportar un segmento del alineamiento (maximo 100 posiciones).

archivo generado mediante:

dot -Tpng alineamiento.dot -o alineamiento.png


si graphviz esta instalado, se obtiene tambien alineamiento.png.

6.3 matrices internas (opcional)

el menu permite imprimir:

matriz dp (matriz de puntuacion)

matriz de direcciones

estas matrices ayudan a comprender el funcionamiento interno del algoritmo.

7. menu interactivo

tras finalizar el alineamiento, aparece un menu como el siguiente:

opciones:
  1) generar imagen graphviz de un intervalo
  2) imprimir matrices en pantalla
  3) mostrar alineamiento en pantalla
  4) salir

8. requisitos
8.1 obligatorios

compilador c++ con soporte c++17

8.2 opcionales

graphviz para generar imagen .png:

sudo apt-get install graphviz

9. consideraciones importantes

el archivo fasta debe contener cabeceras válidas comenzando con >
los archivos csv deben tener una primera fila y columna con encabezados
la memoria utilizada por needleman–wunsch crece proporcionalmente a n × m