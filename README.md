# Modelo-de-programacion-y-memoria-en-MPI
Desarrollar un programa que utilice clases para llenar un arreglo dinámico con valores aleatorios y mostrar su porcentaje de avance, utilizando MPI para distribuir la ejecución entre procesos y nodos, y OpenMP para paralelizar el procesamiento dentro de cada nodo.
# Actividad 1.1: Modelo de programación y memoria en MPI.
Integrantes: Gutiérrez Hurtado Argenis Adán, Rubio Calderon  Gerardo Josue

## Descripción de la Solución
Esta práctica implementa un sistema híbrido de alto rendimiento combinando **MPI (Message Passing Interface)** para la computación distribuida entre nodos en red y **OpenMP (Open Multi-Processing)** para el procesamiento multihilo en memoria compartida.

El programa administra arreglos dinámico masivos de enteros de forma paralela en tres modos de ejecución configurables por línea de comandos:
1. **Modo 1 (Prueba Local):** Inicialización de arreglos con tamaño aleatorio (entre 20 y 50 elementos) en cada proceso MPI.
2. **Modo 2 (Carga Masiva Local):** Generación de 10,000,000 de elementos enteros por proceso MPI dentro de una sola máquina.
3. **Modo 3 (Ejecución Distribuida):** Procesamiento de 10,000,000 de elementos distribuidos a través de una red local conectando **al menos 3 computadoras físicas**.

Cada nodo reporta progresivamente su porcentaje de avance (del 10% al 100%) en incrementos estrictos de 10 en 10 sin entrelazado de consola, utilizando punteros crudos (`int*`) para garantizar una gestión eficiente de la memoria sin depender de contenedores como `std::vector`.

---

## Caracteres Utilizados
Para el formateo, estructura e interfaz visual por consola, se emplean los siguientes caracteres y estándares:
* **Caracteres de estructura y banners ASCII:** Uso de `=`, `-`, `|` y `[]` para la delimitación clara de encabezados, reportes de avance y formato de arreglos en terminal.
* **Separadores de cadena:** Carácter pipeline (`|`) para la segmentación de datos en el log (`Equipo | Nodo/Proceso MPI | Hilo OpenMP | Avance`).
* **Formatos de escape C++:** `\n` y `std::endl` coordinados con `std::ostringstream` y barreras MPI para la gestión del búfer de salida sin colisiones de texto.

---

## Algoritmos Implementados
1. **Algoritmo de Generación Paralela de Datos Pseudoaleatorios:**
   Sincronización de semillas independientes para evitar colisiones entre hilos y nodos mediante la fórmula:
   $$\text{semilla} = \text{std::time(nullptr)} + (\text{rank} \times 1000) + \text{idHilo}$$
2. **Algoritmo de Sincronización y Control de Progreso (10% - 100%):**
   Modelado secuencial mediante barreras de red (`MPI_Barrier`) y turnos por rank para asegurar que cada proceso imprima su progreso en la terminal de forma coordinada.
3. **Algoritmo de Manejo Dinámico de Memoria:**
   Reserva explícita mediante `new int[tamano]` y posterior liberación secuencial con `delete[]` para evitar fugas de memoria (*memory leaks*).
4. **Algoritmo de Truncado de Salida:**
   Para arreglos masivos de 10,000,000 de elementos, se muestran los primeros 10 y los últimos 5 elementos para mantener la legibilidad de la salida.

---

##  Directivas OpenMP y Funciones MPI Utilizadas

### Directivas OpenMP
* `#pragma omp parallel`: Crea la región paralela en memoria compartida e instancía el equipo de hilos locales.
* `#pragma omp for schedule(static)`: Distribuye las iteraciones del bucle de llenado del arreglo de forma equitativa entre los hilos OpenMP disponibles.
* `omp_get_thread_num()`: Obtiene el identificador único del hilo que ejecuta la iteración.
* `omp_get_max_threads()`: Devuelve el número total de hilos habilitados en el procesador del nodo.

### Funciones MPI (MS-MPI)
* `MPI_Init(...)`: Inicializa el entorno de ejecución MPI en todos los procesos.
* `MPI_Comm_rank(...)`: Asigna el ID o rango (`rank`) correspondiente a cada nodo.
* `MPI_Comm_size(...)`: Determina el número total de procesos MPI activos en la ejecución.
* `MPI_Barrier(MPI_COMM_WORLD)`: Fuerza una barrera de sincronización global de red para ordenar el flujo de impresión en consola.
* `MPI_Finalize()`: Libera los recursos del entorno de comunicación MPI antes de cerrar el programa.

---

##  Instrucciones Básicas de Compilación y Ejecución

### Prerrequisitos
* Compilador de C++ (MinGW / GCC o MSVC) compatible con OpenMP (`-fopenmp`).
* **Microsoft MPI (MS-MPI)** v10.0 o superior instalado en el sistema.

### Compilación (desde CMD o Code::Blocks)
```cmd
g++ -O2 -fopenmp main.cpp ProcesadorArreglo.cpp -o Actividad_1.1.exe -I"C:\Program Files (x86)\Microsoft SDKs\MPI\Include" -L"C:\Program Files (x86)\Microsoft SDKs\MPI\Lib\x64" -lmsmpi
