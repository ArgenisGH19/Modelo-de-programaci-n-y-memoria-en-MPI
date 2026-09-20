#ifndef PROCESADOR_ARREGLO_H
#define PROCESADOR_ARREGLO_H

#include <string>

class ProcesadorArreglo {
private:
    int tamano;
    std::string nombreEquipo;
    int* arreglo;
    int rank;
    int size;

public:

    ProcesadorArreglo(int tam, const std::string& equipo);
    ~ProcesadorArreglo();

    void llenarYMostrarAvance();
    void imprimirArreglo() const;
};

#endif // PROCESADOR_ARREGLO_H
