#include <iostream>
#include <cstdlib>
#include <ctime>
#include <mpi.h>
#include <omp.h>
#include <cstdio>
#include "ProcesadorArreglo.h"

int main(int argc, char** argv) {
    MPI_Init(&argc, &argv);
    std::setvbuf(stdout, NULL, _IONBF, 0);

    int rank, size;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);

    std::string nombreEquipo = "Equipo_Alpha";
    int tamanoArreglo = 0;
    int opcionModo = 1;


    if (argc > 1) {
        opcionModo = std::atoi(argv[1]);
    }

    switch (opcionModo) {
        case 1:
            std::srand(static_cast<unsigned int>(std::time(nullptr)) + rank);
            tamanoArreglo = 20 + (std::rand() % 31); // Rango [20, 50]
            break;

        case 2:
            tamanoArreglo = 10000000;
            break;

        case 3:
            tamanoArreglo = 10000000;
            break;

        default:
            tamanoArreglo = 30;
            break;
    }

    MPI_Barrier(MPI_COMM_WORLD);

    if (rank == 0) {
        std::cout << "=================================================" << std::endl;
        std::cout << "  INICIANDO PRACTICA HIBRIDA MPI + OPENMP" << std::endl;
        std::cout << "  Procesos MPI totales: " << size << std::endl;
        std::cout << "  Hilos OpenMP por nodo: " << omp_get_max_threads() << std::endl;
        std::cout << "  Modo de ejecución: " << opcionModo << std::endl;
        std::cout << "=================================================\n" << std::endl;
    }

    MPI_Barrier(MPI_COMM_WORLD);

    ProcesadorArreglo procesador(tamanoArreglo, nombreEquipo);
    procesador.llenarYMostrarAvance();

    MPI_Barrier(MPI_COMM_WORLD);

    procesador.imprimirArreglo();

    MPI_Finalize();

    return 0;
}
