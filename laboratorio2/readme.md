Laboratorio 2 – Ejercicio 1  
Cristian Morales  


Este programa implementa una **pila de enteros** utilizando arreglos en C++.  
Se incluyen las operaciones básicas:
- **Push**: agregar un elemento a la pila.  
- **Pop**: eliminar el último elemento agregado.  
- **Mostrar pila**: visualizar el estado de la pila.  

El usuario define el **tamaño máximo de la pila** al inicio de la ejecución.  
Se manejan los casos de **pila vacía** (subdesbordamiento) y **pila llena** (desbordamiento).  

---




Compilación

g++ Morales_Cristian_Laboratorio.cpp -o 1
./1

(ejemplo)
Ingrese tamaño máximo de la pila: 3

========= MENU PILA =========
1. Agregar (Push)
2. Remover (Pop)
3. Ver pila
0. Salir
=============================
Opción: 1
Ingrese valor: 10
Elemento 10 agregado a la pila.

Opción: 1
Ingrese valor: 20
 Elemento 20 agregado a la pila.

Opción: 3
 Estado actual de la pila:
| 20 |
| 10 |

___________________________________________________________



---


# Laboratorio 2 – Ejercicio 2  


## 📌 Descripción
Este programa simula la gestión de contenedores en un **puerto seco** con pilas:  
- Cada pila puede contener un máximo de **n contenedores**.  
- Existen en total **m pilas**.  
- Cada contenedor tiene un **identificador único**.  

### Funcionalidades
- **Ingreso manual**: el usuario elige en qué pila se coloca el contenedor.  
- **Ingreso automático**: el sistema busca la primera pila con espacio disponible.  
- **Retiro de contenedor**: para retirar uno, se deben mover los que están encima.  
- **Mostrar estado**: imprime una representación gráfica del puerto con sus pilas.  



 Compilación

g++ Morales_Cristian_Laboratorio2.cpp -o 2

./2


(ejemplo)

========= MENU PUERTO SECO =========
1. Ingresar contenedor manual
2. Ingresar contenedor automático
3. Retirar contenedor
4. Ver estado
0. Salir
===================================
Opción: 2
Ingrese ID del contenedor: EMP1
Contenedor EMP1 agregado automáticamente a pila 0

Opción: 2
Ingrese ID del contenedor: EMP2
Contenedor EMP2 agregado automáticamente a pila 0

Opción: 4
======= ESTADO DEL PUERTO =======
   EMP2   .    .    .    .
   EMP1   .    .    .    .
---------------------------------
   P0    P1   P2   P3   P4
=================================



