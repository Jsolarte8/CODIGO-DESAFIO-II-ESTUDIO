// COPIA DE ESTUDIO: comportamiento original conservado.
// Gestor posee Mensaje**, cada Mensaje posee texto y Fecha por valor. IDs representan asociación lógica.
#ifndef GESTOR_MENSAJES_H
#define GESTOR_MENSAJES_H

#include "admin_amistades.h"
#include <cstddef>
#include <iosfwd>

class Fecha {
public:
    Fecha(int dia = 1, int mes = 1, int anio = 2000);
    int getDia() const;
    int getMes() const;
    int getAnio() const;
    // ESTUDIO: Sobrecarga de comparación: compara YYYYMMDD; const& evita copia y const final protege el objeto.
    bool operator==(const Fecha& otra) const;
    // ESTUDIO: Sobrecarga de comparación: compara YYYYMMDD; const& evita copia y const final protege el objeto.
    bool operator<=(const Fecha& otra) const;
private:
    int valor_;
};

class Mensaje {
public:
    Mensaje(const Mensaje& otro);
    ~Mensaje();
    // ESTUDIO: Impide asignación superficial de punteros propietarios.
    Mensaje& operator=(const Mensaje&) = delete;
    int getId() const;
    int getEmisor() const;
    int getReceptor() const;
    const Fecha& getFecha() const;
    const char* getTexto() const;
    bool estaLeido() const;
private:
    // ESTUDIO: Permite al gestor acceder a miembros privados de Mensaje.
    friend class GestorMensajes;
    Mensaje(int id, int emisor, int receptor, const Fecha& fecha,
            const char* texto, std::size_t bytes, bool leido);
    int id_, emisor_, receptor_;
    // ESTUDIO: Composición por valor: la fecha vive dentro del mensaje.
    Fecha fecha_;
    char* texto_;
    std::size_t bytes_;
    bool leido_;
};

class GestorMensajes {
public:
    GestorMensajes();
    ~GestorMensajes();
    GestorMensajes(const GestorMensajes&) = delete;
    // ESTUDIO: Impide asignación superficial de punteros propietarios.
    GestorMensajes& operator=(const GestorMensajes&) = delete;

    // ESTUDIO: Valida usuarios/texto y escanea todos los IDs previos; carga global O(M²). El gestor adquiere propiedad del nuevo mensaje.
    bool cargarMensaje(const AdminAmistades& red, int id, int emisor, int receptor,
                       const Fecha& fecha, const char* texto, bool leido);
    // ESTUDIO: Resuelve destinatario, comprueba amistad, genera ID y delega carga.
    bool enviarMensaje(const AdminAmistades& red, int emisor,
                       const char* destinatario, const char* texto, const Fecha& fecha);
    // ESTUDIO: Dos pasadas O(M): mostrar primero, marcar leídos sólo si salida válida.
    int leerNuevos(const AdminAmistades& red, int receptor, std::ostream& salida);
    // ESTUDIO: Búsqueda binaria del límite inferior y recorrido hasta fin inclusivo; filtra receptor.
    int buscarPorRangoDeFechas(const AdminAmistades& red, int receptor,
        const Fecha& inicio, const Fecha& fin, std::ostream& salida) const;
    // ESTUDIO: Submenú que acumula iteraciones de varias acciones.
    void mostrarMenu(const AdminAmistades& red, int usuario, const Fecha& fechaActual,
                     std::istream& entrada, std::ostream& salida);
    int getCantidad() const;
    const Mensaje* getMensaje(int posicion) const;
    void reiniciarIteraciones() const;
    unsigned long long getIteraciones() const;
    std::size_t getMemoriaReservada() const;

private:
    // ESTUDIO: Arreglo propietario de punteros a objetos Mensaje.
    Mensaje** mensajes_;
    int cantidad_, capacidad_, ultimoId_;
    std::size_t bytesMensajes_;
    // ESTUDIO: mutable permite contar trabajo desde métodos const.
    mutable unsigned long long iteraciones_;
    // ESTUDIO: Valida UTF-8 por rangos hexadecimales y limita a 512 puntos de código; no usa máscaras.
    bool validarTexto(const char* texto, std::size_t& bytes) const;
    // ESTUDIO: Amplía arreglo de punteros, desplaza para ordenar por fecha/ID. O(M) peor caso.
    void insertar(Mensaje* mensaje);
    // ESTUDIO: Busca nombre del emisor en red, luego imprime fecha y texto.
    void imprimir(const AdminAmistades& red, const Mensaje& mensaje, std::ostream& salida) const;
};

#endif
