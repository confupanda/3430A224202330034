
set -euo pipefail


# configuraciones

LABDIR="$(pwd)"                      # ruta actual (Lab_10)
OUTPUT_LOG="${LABDIR}/lab10_salida.log"
GO_OUTPUT="GO proteinas de trabajo.csv"

PDB_FILES=("HEM_A.pdb" "GFP_A.pdb" "ACE2_A.pdb")
AMINOACIDOS=(ALA ARG ASN ASP CYS GLN GLU GLY HIS ILE LEU LYS MET PHE PRO SER THR TRP TYR VAL)

echo "inicio del script lab 10" > "$OUTPUT_LOG"
echo "fecha: $(date)" >> "$OUTPUT_LOG"

# 1) manejo de carpetas

echo -e "\n1) manejo de carpetas" | tee -a "$OUTPUT_LOG"

SALIDA="${LABDIR}/Lab10_salida"
echo "creando carpeta de salida: $SALIDA" | tee -a "$OUTPUT_LOG"
mkdir -p "$SALIDA"


# 2) listado de archivos

echo -e "\n2) listado de archivos" | tee -a "$OUTPUT_LOG"

echo -e "\nlista simple:" | tee -a "$OUTPUT_LOG"
ls -1 | tee -a "$OUTPUT_LOG"

echo -e "\narchivos *.txt:" | tee -a "$OUTPUT_LOG"
ls -1 *.txt 2>/dev/null | tee -a "$OUTPUT_LOG" || true

echo -e "\narchivos que comienzan con G o g:" | tee -a "$OUTPUT_LOG"
ls -1 [Gg]* 2>/dev/null | tee -a "$OUTPUT_LOG" || true

echo -e "\narchivos que contienen ' A':" | tee -a "$OUTPUT_LOG"
ls -1 | grep -i " A" || echo "ninguno" | tee -a "$OUTPUT_LOG"

# listado dentro de uniprot_info
if [ -d "uniprot_info" ]; then
    echo -e "\nentrando en uniprot_info:" | tee -a "$OUTPUT_LOG"
    cd uniprot_info
    ls -la | tee -a "../$OUTPUT_LOG"

    COUNT=$(ls -A | wc -l)
    echo "cantidad de archivos: $COUNT" | tee -a "../$OUTPUT_LOG"

    echo -e "\narchivos ocultos:" | tee -a "../$OUTPUT_LOG"
    ls -d .[!.]* 2>/dev/null || echo "ninguno"

    cd ..
else
    echo "no existe uniprot_info" | tee -a "$OUTPUT_LOG"
fi

echo -e "\norden alfabetico:" | tee -a "$OUTPUT_LOG"
ls -1 | sort | tee -a "$OUTPUT_LOG"

echo -e "\norden por fecha:" | tee -a "$OUTPUT_LOG"
ls -lt | tee -a "$OUTPUT_LOG"


# 3) mostrar contenido de HEM.txt

echo -e "\n3) mostrar contenido de HEM.txt" | tee -a "$OUTPUT_LOG"

if [ -d "uniprot_info" ]; then
    cd uniprot_info
    if [ -f "HEM.txt" ]; then
        echo -e "\ncontenido completo HEM.txt:" | tee -a "../$OUTPUT_LOG"
        cat HEM.txt | tee -a "../$OUTPUT_LOG"

        echo -e "\nprimeras 10 lineas:" | tee -a "../$OUTPUT_LOG"
        head -n 10 HEM.txt | tee -a "../$OUTPUT_LOG"

        echo -e "\nultimas 10 lineas:" | tee -a "../$OUTPUT_LOG"
        tail -n 10 HEM.txt | tee -a "../$OUTPUT_LOG"

        echo -e "\nbuscar 'NUCLEOTIDE SEQUENCE':" | tee -a "../$OUTPUT_LOG"
        grep -n "NUCLEOTIDE SEQUENCE" HEM.txt | tee -a "../$OUTPUT_LOG" || true
    fi
    cd ..
fi


# 4) busquedas GO

echo -e "\n4) busquedas GO" | tee -a "$OUTPUT_LOG"

cd uniprot_info

for f in HEM.txt GFP.txt ACE2.txt; do
    if [ -f "$f" ]; then
        echo -e "\nbuscando GO en $f:" | tee -a "../$OUTPUT_LOG"
        grep --color=always -n "GO;" "$f" | tee -a "../$OUTPUT_LOG" || true
    fi
done

cd ..


# 5) renombrar ocultos

echo -e "\n5) renombrar ocultos" | tee -a "$OUTPUT_LOG"

CANDIDATOS=(".GFP.txt" ".ACE2.txt" ".borrar.txt")

for f in "${CANDIDATOS[@]}"; do
    if [ -f "$f" ]; then
        nuevo="${f#.}"
        echo "renombrando $f -> $nuevo" | tee -a "$OUTPUT_LOG"
        mv "$f" "$nuevo"
    else
        echo "$f no existe" | tee -a "$OUTPUT_LOG"
    fi
done

if [ -f "borrar.txt" ]; then
    echo -e "\ncontenido borrar.txt:" | tee -a "$OUTPUT_LOG"
    cat borrar.txt | tee -a "$OUTPUT_LOG"
    echo "borrando borrar.txt" | tee -a "$OUTPUT_LOG"
    rm borrar.txt
fi


# 6) generar archivos GO *.csv

echo -e "\n6) generar archivos GO *.csv" | tee -a "$OUTPUT_LOG"

cd uniprot_info

for prot in HEM GFP ACE2; do
    in="${prot}.txt"
    out="../GO ${prot}.csv"
    if [ -f "$in" ]; then
        echo "extrayendo GO de $in -> $out" | tee -a "../$OUTPUT_LOG"
        grep -n "GO;" "$in" > "$out" || true
    fi
done

cd ..

echo "concatenando GO *.csv" | tee -a "$OUTPUT_LOG"
cat "GO HEM.csv" "GO GFP.csv" "GO ACE2.csv" > "$GO_OUTPUT" || true

echo "wc del archivo concatenado:" | tee -a "$OUTPUT_LOG"
wc "$GO_OUTPUT" | tee -a "$OUTPUT_LOG"

echo "eliminando archivo concatenado" | tee -a "$OUTPUT_LOG"
rm "$GO_OUTPUT"


# 7) ultima columna de GO HEM
echo -e "\n7) ultima columna funciones biologicas" | tee -a "$OUTPUT_LOG"

if [ -f "GO HEM.csv" ]; then
    awk -F';' '{print $NF}' "GO HEM.csv" > "FuncionesBiologica HEM.csv"
    awk -F';' '{print $NF}' "GO GFP.csv" > "FuncionesBiologica GFP.csv"
    awk -F';' '{print $NF}' "GO ACE2.csv" > "FuncionesBiologica ACE2.csv"
fi


# 8) obtención de aminoácidos

echo -e "\n8) obtencion aminoacidos" | tee -a "$OUTPUT_LOG"

mkdir -p "Obtencion_Aminoacidos"
cp -v *.pdb "Obtencion_Aminoacidos/" | tee -a "$OUTPUT_LOG"

cd "Obtencion_Aminoacidos"

count_residues() {
    local file="$1"
    local base="${file%.pdb}"

    awk 'substr($0,18,3)=="LYS" && substr($0,13,2)=="CA" {print}' "$file" > "${base}-LYS-CA.pdb"

    lys_count=$(wc -l < "${base}-LYS-CA.pdb")

    echo "aas;${base}" > "${base}-Residues-All.csv"
    echo "LYS;${lys_count}" >> "${base}-Residues-All.csv"
}

for f in "${PDB_FILES[@]}"; do
    if [ -f "$f" ]; then
        count_residues "$f"
    fi
done

cd "$LABDIR"


# FIN

echo -e "\nscript finalizado correctamente." | tee -a "$OUTPUT_LOG"
echo "fin." >> "$OUTPUT_LOG"
