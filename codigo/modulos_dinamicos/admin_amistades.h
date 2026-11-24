// COPIA DE ESTUDIO: comportamiento original conservado.
// Grafo matricial: C*C unsigned char, 0 sin relación, 1 amistad, 2 solicitud dirigida. No empaqueta bits.
#ifndef ADMIN_AMISTADES_H
#define ADMIN_AMISTADES_H

#include <cstddef>
#include <iosfwd>

class AdminAmistades {
public:
    explicit AdminAmistades(int capacidad = 8);
    ~AdminAmistades();
    AdminAmistades(const AdminAmistades&) = delete;
    // ESTUDIO: Impide asignación superficial de punteros propietarios.
    AdminAmistades& operator=(const AdminAmistades&) = delete;

    // ESTUDIO: Valida unicidad, amplía si hace falta y copia nombre a memoria propia.
    bool cargarUsuario(int id, const char* nombre);
    // ESTUDIO: Escribe estado 1 en ambas direcciones: amistad simétrica.
    bool cargarAmistad(int primero, int segundo);
    bool cargarSolicitud(int emisor, int receptor);
    // ESTUDIO: Búsqueda lineal por ID o nombre; retorna índice o -1. O(N) por ID.
    int buscarUsuario(int id) const;
    // ESTUDIO: Búsqueda lineal por ID o nombre; retorna índice o -1. O(N) por ID.
    int buscarUsuario(const char* nombre) const;
    int getCantidadUsuarios() const;
    int getIdEnPosicion(int posicion) const;
    const char* getNombre(int id) const;
    // ESTUDIO: Consulta celda O(1) tras dos búsquedas lineales: total O(N).
    bool sonAmigos(int primero, int segundo) const;
    // ESTUDIO: Consulta estado 2 en dirección emisor->receptor.
    bool haySolicitud(int emisor, int receptor) const;
    // ESTUDIO: Sobrecarga de función por tipo del destinatario, ID o nombre.
    bool envioDeSolicitud(int emisor, int receptor);
    // ESTUDIO: Sobrecarga de función por tipo del destinatario, ID o nombre.
    bool envioDeSolicitud(int emisor, const char* nombreReceptor);
    // ESTUDIO: Convierte solicitud dirigida en amistad simétrica.
    bool aceptarSolicitud(int receptor, int emisor);
    // ESTUDIO: Restablece a cero solicitud dirigida.
    bool rechazarSolicitud(int receptor, int emisor);
    // ESTUDIO: Busca posiciones y recorre N columnas, contando coincidencias de estado 1.
    int contarAmigosComunes(int primero, int segundo) const;

    // ESTUDIO: Cuenta caminos de longitud dos y mantiene top K, excluyendo amigos y pendientes. O(N+d*N+N*K).
    int sugerirAmigos(int usuario, int k, int* ids, int* comunes) const;
    // ESTUDIO: Escanea columna del receptor para identificar emisores pendientes.
    void listarSolicitudes(int receptor, std::ostream& salida) const;
    // ESTUDIO: Submenú que acumula iteraciones de varias acciones.
    void mostrarMenu(int usuario, std::istream& entrada, std::ostream& salida);
    // ESTUDIO: Presenta top K y permite seleccionar destinatario.
    void mostrarMenuSugerencias(int usuario, std::istream& entrada, std::ostream& salida);

    void reiniciarIteraciones() const;
    unsigned long long getIteraciones() const;
    std::size_t getMemoriaReservada() const;

private:
    int capacidad_;
    int cantidad_;
    int* ids_;
    char** nombres_;
    // ESTUDIO: Matriz contigua con capacidad al cuadrado.
    unsigned char* matriz_;
    std::size_t bytesNombres_;
    // ESTUDIO: mutable permite contar trabajo desde métodos const.
    mutable unsigned long long iteraciones_;
    // ESTUDIO: Traduce fila y columna a posición lineal fila*capacidad+columna, O(1).
    std::size_t celda(int fila, int columna) const;
    bool posicionesValidas(int primero, int segundo) const;
    // ESTUDIO: Compara bytes del nombre y cuenta cada comparación.
    bool nombresIguales(const char* primero, const char* segundo) const;
    // ESTUDIO: Verifica ambos sentidos libres; escribe 2 sólo emisor->receptor.
    bool registrarSolicitud(int emisor, int receptor);
    // ESTUDIO: Duplica capacidad y copia matriz N*N; transfiere punteros de nombres.
    void ampliar();
};

#endif
