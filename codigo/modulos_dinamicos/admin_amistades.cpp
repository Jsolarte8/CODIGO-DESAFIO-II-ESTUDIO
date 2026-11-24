// COPIA DE ESTUDIO: comportamiento original conservado.
// Grafo matricial: C*C unsigned char, 0 sin relación, 1 amistad, 2 solicitud dirigida. No empaqueta bits.
#include "admin_amistades.h"
#include <istream>
#include <ostream>
#include <limits>
#include <stdexcept>

namespace {
bool leerEntero(std::istream& entrada, int& valor) {
    int leido = 0;
    if (entrada >> leido) {
        valor = leido;
        entrada.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
        return true;
    }
    if (!entrada.eof()) {
        entrada.clear();
        entrada.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
    }
    return false;
}
}

AdminAmistades::AdminAmistades(int capacidad)
    : capacidad_(capacidad), cantidad_(0), ids_(nullptr), nombres_(nullptr),
      matriz_(nullptr), bytesNombres_(0), iteraciones_(0) {
    if (capacidad <= 0 || static_cast<std::size_t>(capacidad) >
        std::numeric_limits<std::size_t>::max() / static_cast<std::size_t>(capacidad))
        throw std::invalid_argument("Capacidad de usuarios invalida");
    try {
        // ESTUDIO: Reserva dinámica; el dueño debe liberar al terminar.
        ids_ = new int[capacidad_]();
        // ESTUDIO: Reserva dinámica; el dueño debe liberar al terminar.
        nombres_ = new char*[capacidad_]();
        // ESTUDIO: Reserva dinámica; el dueño debe liberar al terminar.
        matriz_ = new unsigned char[static_cast<std::size_t>(capacidad_) * capacidad_]();
    } catch (...) {
        // ESTUDIO: Libera memoria previamente reservada.
        delete[] ids_;
        // ESTUDIO: Libera memoria previamente reservada.
        delete[] nombres_;
        // ESTUDIO: Libera memoria previamente reservada.
        delete[] matriz_;
        throw;
    }
}
AdminAmistades::~AdminAmistades() {
    // ESTUDIO: Recorre elementos ocupados; considerar costo N o M según módulo.
    for (int i = 0; i < cantidad_; ++i) delete[] nombres_[i];
    // ESTUDIO: Libera memoria previamente reservada.
    delete[] nombres_;
    // ESTUDIO: Libera memoria previamente reservada.
    delete[] ids_;
    // ESTUDIO: Libera memoria previamente reservada.
    delete[] matriz_;
}
// ESTUDIO: Traduce fila y columna a posición lineal fila*capacidad+columna, O(1).
std::size_t AdminAmistades::celda(int fila, int columna) const {
    return static_cast<std::size_t>(fila) * capacidad_ + columna;
}
bool AdminAmistades::posicionesValidas(int primero, int segundo) const {
    return primero >= 0 && segundo >= 0 && primero != segundo;
}
// ESTUDIO: Compara bytes del nombre y cuenta cada comparación.
bool AdminAmistades::nombresIguales(const char* primero, const char* segundo) const {
    if (!primero || !segundo) return false;
    int i = 0;
    do {
        // ESTUDIO: Instrumentación manual; no equivale a instrucciones CPU.
        ++iteraciones_;
        if (primero[i] != segundo[i]) return false;
    } while (primero[i++] != '\0');
    return true;
}
// ESTUDIO: Búsqueda lineal por ID o nombre; retorna índice o -1. O(N) por ID.
int AdminAmistades::buscarUsuario(int id) const {
    // ESTUDIO: Recorre elementos ocupados; considerar costo N o M según módulo.
    for (int i = 0; i < cantidad_; ++i) {
        // ESTUDIO: Instrumentación manual; no equivale a instrucciones CPU.
        ++iteraciones_;
        if (ids_[i] == id) return i;
    }
    return -1;
}
// ESTUDIO: Búsqueda lineal por ID o nombre; retorna índice o -1. O(N) por ID.
int AdminAmistades::buscarUsuario(const char* nombre) const {
    if (!nombre) return -1;
    // ESTUDIO: Recorre elementos ocupados; considerar costo N o M según módulo.
    for (int i = 0; i < cantidad_; ++i) {
        // ESTUDIO: Instrumentación manual; no equivale a instrucciones CPU.
        ++iteraciones_;
        if (nombresIguales(nombres_[i], nombre)) return i;
    }
    return -1;
}
// ESTUDIO: Valida unicidad, amplía si hace falta y copia nombre a memoria propia.
bool AdminAmistades::cargarUsuario(int id, const char* nombre) {
    if (id <= 0 || !nombre || !nombre[0] ||
        buscarUsuario(id) >= 0 || buscarUsuario(nombre) >= 0) return false;
    std::size_t longitud = 0;
    bool contenido = false;
    // ESTUDIO: Recorrido controlado por condición; evaluar costo y contador.
    while (nombre[longitud]) {
        // ESTUDIO: Instrumentación manual; no equivale a instrucciones CPU.
        ++iteraciones_;
        if (nombre[longitud] != ' ' && nombre[longitud] != '\t' &&
            nombre[longitud] != '\r' && nombre[longitud] != '\n') contenido = true;
        ++longitud;
    }
    if (!contenido) return false;
    if (cantidad_ == capacidad_) ampliar();
    // ESTUDIO: Reserva dinámica; el dueño debe liberar al terminar.
    char* copia = new char[longitud + 1];
    // ESTUDIO: Recorrido controlado por condición; evaluar costo y contador.
    for (std::size_t i = 0; i <= longitud; ++i) {
        // ESTUDIO: Instrumentación manual; no equivale a instrucciones CPU.
        ++iteraciones_;
        copia[i] = nombre[i];
    }
    ids_[cantidad_] = id;
    nombres_[cantidad_++] = copia;
    bytesNombres_ += longitud + 1;
    return true;
}
// ESTUDIO: Duplica capacidad y copia matriz N*N; transfiere punteros de nombres.
void AdminAmistades::ampliar() {
    if (capacidad_ > std::numeric_limits<int>::max() / 2)
        throw std::length_error("Demasiados usuarios");
    int nueva = capacidad_ * 2;
    if (static_cast<std::size_t>(nueva) > std::numeric_limits<std::size_t>::max() / nueva)
        throw std::length_error("Matriz demasiado grande");
    int* ids = nullptr;
    char** nombres = nullptr;
    unsigned char* matriz = nullptr;
    try {
        // ESTUDIO: Reserva dinámica; el dueño debe liberar al terminar.
        ids = new int[nueva]();
        // ESTUDIO: Reserva dinámica; el dueño debe liberar al terminar.
        nombres = new char*[nueva]();
        // ESTUDIO: Reserva dinámica; el dueño debe liberar al terminar.
        matriz = new unsigned char[static_cast<std::size_t>(nueva) * nueva]();
    } catch (...) {
        // ESTUDIO: Libera memoria previamente reservada.
        delete[] ids; delete[] nombres; delete[] matriz;
        throw;
    }
    // ESTUDIO: Recorre elementos ocupados; considerar costo N o M según módulo.
    for (int i = 0; i < cantidad_; ++i) {
        // ESTUDIO: Instrumentación manual; no equivale a instrucciones CPU.
        ++iteraciones_;
        ids[i] = ids_[i]; nombres[i] = nombres_[i];
        // ESTUDIO: Recorre elementos ocupados; considerar costo N o M según módulo.
        for (int j = 0; j < cantidad_; ++j) {
            // ESTUDIO: Instrumentación manual; no equivale a instrucciones CPU.
            ++iteraciones_;
            matriz[static_cast<std::size_t>(i) * nueva + j] = matriz_[celda(i, j)];
        }
    }
    // ESTUDIO: Libera memoria previamente reservada.
    delete[] ids_; delete[] nombres_; delete[] matriz_;
    ids_ = ids; nombres_ = nombres; matriz_ = matriz; capacidad_ = nueva;
}
// ESTUDIO: Escribe estado 1 en ambas direcciones: amistad simétrica.
bool AdminAmistades::cargarAmistad(int primero, int segundo) {
    int a = buscarUsuario(primero), b = buscarUsuario(segundo);
    if (!posicionesValidas(a, b)) return false;
    matriz_[celda(a, b)] = matriz_[celda(b, a)] = 1;
    return true;
}
// ESTUDIO: Verifica ambos sentidos libres; escribe 2 sólo emisor->receptor.
bool AdminAmistades::registrarSolicitud(int emisor, int receptor) {
    int a = buscarUsuario(emisor), b = buscarUsuario(receptor);
    if (!posicionesValidas(a, b) || matriz_[celda(a, b)] != 0 ||
        matriz_[celda(b, a)] != 0) return false;
    matriz_[celda(a, b)] = 2;
    return true;
}
bool AdminAmistades::cargarSolicitud(int emisor, int receptor) {
    return registrarSolicitud(emisor, receptor);
}
int AdminAmistades::getCantidadUsuarios() const { return cantidad_; }
int AdminAmistades::getIdEnPosicion(int posicion) const {
    return posicion >= 0 && posicion < cantidad_ ? ids_[posicion] : -1;
}
const char* AdminAmistades::getNombre(int id) const {
    int posicion = buscarUsuario(id);
    return posicion >= 0 ? nombres_[posicion] : nullptr;
}
// ESTUDIO: Consulta celda O(1) tras dos búsquedas lineales: total O(N).
bool AdminAmistades::sonAmigos(int primero, int segundo) const {
    int a = buscarUsuario(primero), b = buscarUsuario(segundo);
    return posicionesValidas(a, b) && matriz_[celda(a, b)] == 1;
}
// ESTUDIO: Consulta estado 2 en dirección emisor->receptor.
bool AdminAmistades::haySolicitud(int emisor, int receptor) const {
    int a = buscarUsuario(emisor), b = buscarUsuario(receptor);
    return posicionesValidas(a, b) && matriz_[celda(a, b)] == 2;
}
// ESTUDIO: Sobrecarga de función por tipo del destinatario, ID o nombre.
bool AdminAmistades::envioDeSolicitud(int emisor, int receptor) {
    return registrarSolicitud(emisor, receptor);
}
// ESTUDIO: Sobrecarga de función por tipo del destinatario, ID o nombre.
bool AdminAmistades::envioDeSolicitud(int emisor, const char* nombreReceptor) {
    int posicion = buscarUsuario(nombreReceptor);
    return posicion >= 0 && registrarSolicitud(emisor, ids_[posicion]);
}
// ESTUDIO: Convierte solicitud dirigida en amistad simétrica.
bool AdminAmistades::aceptarSolicitud(int receptor, int emisor) {
    int a = buscarUsuario(emisor), b = buscarUsuario(receptor);
    if (!posicionesValidas(a, b) || matriz_[celda(a, b)] != 2) return false;
    matriz_[celda(a, b)] = matriz_[celda(b, a)] = 1;
    return true;
}
// ESTUDIO: Restablece a cero solicitud dirigida.
bool AdminAmistades::rechazarSolicitud(int receptor, int emisor) {
    int a = buscarUsuario(emisor), b = buscarUsuario(receptor);
    if (!posicionesValidas(a, b) || matriz_[celda(a, b)] != 2) return false;
    matriz_[celda(a, b)] = 0;
    return true;
}
// ESTUDIO: Busca posiciones y recorre N columnas, contando coincidencias de estado 1.
int AdminAmistades::contarAmigosComunes(int primero, int segundo) const {
    int a = buscarUsuario(primero), b = buscarUsuario(segundo), total = 0;
    if (!posicionesValidas(a, b)) return 0;
    // ESTUDIO: Recorre elementos ocupados; considerar costo N o M según módulo.
    for (int i = 0; i < cantidad_; ++i) {
        // ESTUDIO: Instrumentación manual; no equivale a instrucciones CPU.
        ++iteraciones_;
        if (matriz_[celda(a, i)] == 1 && matriz_[celda(b, i)] == 1) ++total;
    }
    return total;
}
// ESTUDIO: Cuenta caminos de longitud dos y mantiene top K, excluyendo amigos y pendientes. O(N+d*N+N*K).
int AdminAmistades::sugerirAmigos(int usuario, int k, int* ids, int* comunes) const {
    int activo = buscarUsuario(usuario);
    if (activo < 0 || k < 1 || k > 20 || !ids || !comunes || ids == comunes) return -1;
    // ESTUDIO: Reserva dinámica; el dueño debe liberar al terminar.
    int* conteos = new int[cantidad_]();
    // ESTUDIO: Recorre elementos ocupados; considerar costo N o M según módulo.
    for (int amigo = 0; amigo < cantidad_; ++amigo) {
        // ESTUDIO: Instrumentación manual; no equivale a instrucciones CPU.
        ++iteraciones_;
        if (matriz_[celda(activo, amigo)] != 1) continue;
        // ESTUDIO: Recorre elementos ocupados; considerar costo N o M según módulo.
        for (int candidato = 0; candidato < cantidad_; ++candidato) {
            // ESTUDIO: Instrumentación manual; no equivale a instrucciones CPU.
            ++iteraciones_;
            if (matriz_[celda(amigo, candidato)] == 1) ++conteos[candidato];
        }
    }
    int usados = 0;
    // ESTUDIO: Recorre elementos ocupados; considerar costo N o M según módulo.
    for (int candidato = 0; candidato < cantidad_; ++candidato) {
        // ESTUDIO: Instrumentación manual; no equivale a instrucciones CPU.
        ++iteraciones_;
        if (candidato == activo || matriz_[celda(activo, candidato)] != 0 ||
            matriz_[celda(candidato, activo)] != 0) continue;
        int coincidencias = conteos[candidato];
        if (coincidencias == 0) continue;
        int lugar = 0;
        // ESTUDIO: Recorrido controlado por condición; evaluar costo y contador.
        while (lugar < usados) {
            // ESTUDIO: Instrumentación manual; no equivale a instrucciones CPU.
            ++iteraciones_;
            if (coincidencias > comunes[lugar] ||
                (coincidencias == comunes[lugar] && ids_[candidato] < ids[lugar])) break;
            ++lugar;
        }
        if (lugar >= k) continue;
        int ultimo = usados < k ? usados : k - 1;
        // ESTUDIO: Recorrido controlado por condición; evaluar costo y contador.
        for (int i = ultimo; i > lugar; --i) {
            // ESTUDIO: Instrumentación manual; no equivale a instrucciones CPU.
            ++iteraciones_;
            ids[i] = ids[i - 1];
            comunes[i] = comunes[i - 1];
        }
        ids[lugar] = ids_[candidato];
        comunes[lugar] = coincidencias;
        if (usados < k) ++usados;
    }
    // ESTUDIO: Libera memoria previamente reservada.
    delete[] conteos;
    return usados;
}
// ESTUDIO: Escanea columna del receptor para identificar emisores pendientes.
void AdminAmistades::listarSolicitudes(int receptor, std::ostream& salida) const {
    int activo = buscarUsuario(receptor), cantidad = 0;
    if (activo < 0) { salida << "Usuario inexistente.\n"; return; }
    // ESTUDIO: Recorre elementos ocupados; considerar costo N o M según módulo.
    for (int i = 0; i < cantidad_; ++i) {
        // ESTUDIO: Instrumentación manual; no equivale a instrucciones CPU.
        ++iteraciones_;
        if (matriz_[celda(i, activo)] == 2) {
            salida << ids_[i] << " - " << nombres_[i] << '\n';
            ++cantidad;
        }
    }
    if (!cantidad) salida << "No tienes solicitudes recibidas.\n";
}
// ESTUDIO: Submenú que acumula iteraciones de varias acciones.
void AdminAmistades::mostrarMenu(int usuario, std::istream& entrada, std::ostream& salida) {
    if (buscarUsuario(usuario) < 0) { salida << "Usuario inexistente.\n"; return; }
    int opcion = -1;
    // ESTUDIO: Recorrido controlado por condición; evaluar costo y contador.
    while (opcion != 0 && entrada && salida) {
        // ESTUDIO: Instrumentación manual; no equivale a instrucciones CPU.
        ++iteraciones_;
        salida << "\nAMISTADES\n1. Enviar solicitud\n2. Ver recibidas\n"
                  "3. Aceptar solicitud\n4. Rechazar solicitud\n0. Volver\nOpcion: ";
        if (!leerEntero(entrada, opcion)) { salida << "Ingresa un numero.\n"; continue; }
        if (opcion == 1) {
            char nombre[1025];
            salida << "Nombre de usuario: ";
            if (!entrada.getline(nombre, sizeof nombre)) {
                if (!entrada.eof()) {
                    entrada.clear();
                    entrada.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
                }
                salida << "Nombre demasiado largo o entrada finalizada.\n";
                continue;
            }
            salida << (envioDeSolicitud(usuario, nombre) ? "Solicitud enviada.\n" :
                "No se puede enviar: revisa usuario, amistad o solicitud pendiente.\n");
        } else if (opcion == 2) listarSolicitudes(usuario, salida);
        else if (opcion == 3 || opcion == 4) {
            listarSolicitudes(usuario, salida);
            int emisor = 0;
            salida << "ID del emisor (0 para volver): ";
            if (!leerEntero(entrada, emisor) || emisor == 0) continue;
            bool ok = opcion == 3 ? aceptarSolicitud(usuario, emisor) : rechazarSolicitud(usuario, emisor);
            salida << (ok ? "Solicitud actualizada.\n" : "No existe esa solicitud recibida.\n");
        } else if (opcion != 0) salida << "Opcion invalida.\n";
    }
}
// ESTUDIO: Presenta top K y permite seleccionar destinatario.
void AdminAmistades::mostrarMenuSugerencias(int usuario, std::istream& entrada, std::ostream& salida) {
    int k = 0, ids[20], comunes[20];
    salida << "Cantidad de sugerencias (1 a 20): ";
    if (!leerEntero(entrada, k)) { salida << "Cantidad invalida.\n"; return; }
    int total = sugerirAmigos(usuario, k, ids, comunes);
    if (total < 0) { salida << "Usuario o cantidad invalida.\n"; return; }
    if (!total) { salida << "No hay sugerencias disponibles.\n"; return; }
    // ESTUDIO: Recorrido controlado por condición; evaluar costo y contador.
    for (int i = 0; i < total; ++i) {
        // ESTUDIO: Instrumentación manual; no equivale a instrucciones CPU.
        ++iteraciones_;
        salida << i + 1 << ". " << getNombre(ids[i]) << " (" << comunes[i] << " en comun)\n";
    }
    int seleccion = 0;
    salida << "Numero de sugerencia para enviar solicitud (0 para volver): ";
    if (!leerEntero(entrada, seleccion) || seleccion == 0) return;
    if (seleccion < 1 || seleccion > total) { salida << "Seleccion invalida.\n"; return; }
    salida << (envioDeSolicitud(usuario, ids[seleccion - 1]) ? "Solicitud enviada.\n" :
        "No fue posible enviar la solicitud.\n");
}
void AdminAmistades::reiniciarIteraciones() const { iteraciones_ = 0; }
unsigned long long AdminAmistades::getIteraciones() const { return iteraciones_; }
std::size_t AdminAmistades::getMemoriaReservada() const {
    return sizeof(*this) + static_cast<std::size_t>(capacidad_) *
        (sizeof(int) + sizeof(char*) + static_cast<std::size_t>(capacidad_)) + bytesNombres_;
}
