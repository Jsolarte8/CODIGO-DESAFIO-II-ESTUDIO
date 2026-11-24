// COPIA DE ESTUDIO: lógica original; comentarios didácticos.
#ifndef MONITOR_RECURSOS_H
#define MONITOR_RECURSOS_H

#include <cstddef>

class MonitorRecursos {
private:
    unsigned long long iteracionesAcumuladasDesdeElInicio;
    std::size_t memoriaAcumuladaDesdeElInicio;

public:
    MonitorRecursos();

    // ESTUDIO: Suma iteraciones y snapshots de memoria; no mide RAM pico.
    void mostrarReporte(const char* const nombresDeModulos[],
                         const unsigned long long iteracionesPorModulo[],
                         const std::size_t memoriaPorModulo[],
                         int cantidadDeModulosReportados);

    void limpiarPantalla();

    unsigned long long getIteracionesAcumuladasDesdeElInicio() const;
    std::size_t getMemoriaAcumuladaDesdeElInicio() const;
};

#endif
