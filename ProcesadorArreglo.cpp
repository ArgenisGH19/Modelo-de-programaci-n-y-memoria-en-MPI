#include "ProcesadorArreglo.h"
#include <iostream>
#include <cstdlib>
#include <ctime>
#include <omp.h>
#include <mpi.h>
#include <sstream>
#include <unistd.h>


ProcesadorArreglo::ProcesadorArreglo(int tam, const std::string& equipo)
    : tamano(tam), nombreEquipo(equipo), arreglo(nullptr), rank(0), size(0) {

    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);

    // Reserva de memoria para el arreglo dinámico (sin std::vector)
    if (tamano > 0) {
        arreglo = new int[tamano];
    }
}

ProcesadorArreglo::~ProcesadorArreglo() {
    // Liberación adecuada del arreglo dinámico
    if (arreglo != nullptr) {
        delete[] arreglo;
        arreglo = nullptr;
    }
}
void ProcesadorArreglo::llenarYMostrarAvance() {
    if (tamano <= 0 || arreglo == nullptr) return;

    // 1. Paralelización estricta del llenado con OpenMP dentro del nodo
    #pragma omp parallel
    {
        int idHilo = omp_get_thread_num();
        std::srand(static_cast<unsigned int>(std::time(nullptr)) + rank * 1000 + idHilo);

        #pragma omp for schedule(static)
        for (int i = 0; i < tamano; ++i) {
            arreglo[i] = std::rand();
        }
    }

    // 2. Reporte de avance ordenado y secuencial (del 10% al 100%)
    // Se recorre proceso por proceso MPI de forma ordenada para evitar colisiones en la consola
    for (int p = 0; p < size; ++p) {
        if (rank == p) {
            for (int pct = 10; pct <= 100; pct += 10) {
                // Seleccionar un hilo OpenMP representativo para el mensaje de avance
                int hiloAsignado = (pct / 10) % omp_get_max_threads();

                std::cout << "Equipo: " << nombreEquipo
                          << " | Nodo/Proceso MPI: " << rank
                          << " | Hilo OpenMP: " << hiloAsignado
                          << " | Avance: " << pct << "%" << std::endl;
            }
        }
        // Sincronizar todos los procesos MPI para que imprima un proceso a la vez
        MPI_Barrier(MPI_COMM_WORLD);
    }
}
void ProcesadorArreglo::imprimirArreglo() const {
    if (arreglo == nullptr) return;

    int limite = (tamano <= 50) ? tamano : 10;

    std::ostringstream ss;
    ss << "\n==========================================\n"
       << "Datos del Arreglo - Equipo: " << nombreEquipo
       << " | Proceso/Nodo MPI: " << rank << "\n"
       << "Tamaño total: " << tamano << " elementos.\n"
       << "Valores: [ ";

    for (int i = 0; i < limite; ++i) {
        ss << arreglo[i] << " ";
    }

    if (tamano > 50) {
        ss << "... ";
        for (int i = tamano - 5; i < tamano; ++i) {
            ss << arreglo[i] << " ";
        }
    }
    ss << "]\n==========================================\n\n";

    std::cout << ss.str() << std::flush;
}
