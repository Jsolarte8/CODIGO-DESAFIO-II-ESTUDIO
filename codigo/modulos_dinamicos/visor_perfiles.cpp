// COPIA DE ESTUDIO: comportamiento original conservado.
// Perfil bajo demanda y gestión de likes; no posee red ni base de datos.
#include "visor_perfiles.h"
#include <iostream>

using namespace std;

static bool textosIguales(const char* primero, const char* segundo) {
    int posicion = 0;
    // ESTUDIO: Recorrido controlado por condición; evaluar costo y contador.
    while (primero[posicion] != '\0' && segundo[posicion] != '\0') {
        if (primero[posicion] != segundo[posicion]) {
            return false;
        }
        posicion++;
    }
    return primero[posicion] == segundo[posicion];
}

static int convertirTextoANumero(const char* texto) {
    int numero = 0;
    int posicion = 0;
    // ESTUDIO: Recorrido controlado por condición; evaluar costo y contador.
    while (texto[posicion] != '\0') {
        numero = numero * 10 + (texto[posicion] - '0');
        posicion++;
    }
    return numero;
}

VisorPerfiles::VisorPerfiles() {
    iteracionesDadas = 0;
}

// ESTUDIO: Busca nombre en índice ligero y copia ID al buffer del llamador.
bool VisorPerfiles::buscarPerfilPorNombre(const GestorArchivos& baseDatos, const char* nombreBuscado,
                                           char* idEncontradoDestino) const {
    int posicionEncontrada = -1;
    int posicionActual = 0;
    // ESTUDIO: Recorrido controlado por condición; evaluar costo y contador.
    while (posicionActual < baseDatos.getTotalUsuarios() && posicionEncontrada == -1) {
        // ESTUDIO: Instrumentación manual; no equivale a instrucciones CPU.
        iteracionesDadas++;

        const UsuarioLigero& usuarioActual = baseDatos.getUsuarioLigero(posicionActual);
        if (textosIguales(usuarioActual.getNombreUsuario(), nombreBuscado)) {
            posicionEncontrada = posicionActual;
        }

        posicionActual++;
    }

    if (posicionEncontrada == -1) {
        return false;
    }

    const UsuarioLigero& usuarioEncontrado = baseDatos.getUsuarioLigero(posicionEncontrada);

    int longitudDelId = 0;
    // ESTUDIO: Recorrido controlado por condición; evaluar costo y contador.
    while (usuarioEncontrado.getIdUsuario()[longitudDelId] != '\0') {
        longitudDelId++;
    }

    int posicionDeCopia = 0;
    // ESTUDIO: Recorrido controlado por condición; evaluar costo y contador.
    while (posicionDeCopia < longitudDelId) {
        idEncontradoDestino[posicionDeCopia] = usuarioEncontrado.getIdUsuario()[posicionDeCopia];
        posicionDeCopia++;
    }
    idEncontradoDestino[longitudDelId] = '\0';

    return true;
}

// ESTUDIO: Carga perfil temporal y filtra fotos públicas o de amigos.
void VisorPerfiles::mostrarCarrusel(const GestorArchivos& baseDatos, const AdminAmistades& redDeAmistades,
                                     const char* idPropio, const char* idEncontrado) const {
    // ESTUDIO: Instrumentación manual; no equivale a instrucciones CPU.
    iteracionesDadas++;

    PerfilCompleto perfilEncontrado;
    bool seLeyoBien = baseDatos.leerPerfilCompleto(idEncontrado, perfilEncontrado);
    if (!seLeyoBien) {
        cout << "Ese usuario todavia no tiene fotos cargadas." << endl;
        return;
    }

    int idPropioComoNumero = convertirTextoANumero(idPropio);
    int idEncontradoComoNumero = convertirTextoANumero(idEncontrado);
    bool sonAmigos = redDeAmistades.sonAmigos(idPropioComoNumero, idEncontradoComoNumero);

    cout << "Carpeta de fotos: " << perfilEncontrado.getRutaCarpetaDeFotos() << endl;

    int cantidadMostradas = 0;
    int posicionActual = 0;
    // ESTUDIO: Recorrido controlado por condición; evaluar costo y contador.
    while (posicionActual < perfilEncontrado.getCantidadDeFotos()) {
        // ESTUDIO: Instrumentación manual; no equivale a instrucciones CPU.
        iteracionesDadas++;

        FotoConLikes& fotoActual = perfilEncontrado.getFotoEnPosicion(posicionActual);
        bool sePuedeMostrar = fotoActual.getEsPublica() || sonAmigos;

        if (sePuedeMostrar) {
            cout << "  - " << fotoActual.getNombreDeArchivo()
                 << " | likes: " << fotoActual.getCantidadDeLikes()
                 << (fotoActual.getEsPublica() ? " | publica" : " | solo amigos") << endl;
            cantidadMostradas++;
        }

        posicionActual++;
    }

    if (cantidadMostradas == 0) {
        cout << "No hay fotos visibles para ti en este perfil." << endl;
    }
}

// ESTUDIO: Relee perfil y busca foto; gestor vuelve a leer y reescribe. Falta verificar privacidad aquí.
bool VisorPerfiles::darLike(const GestorArchivos& baseDatos, const char* idDelPerfil,
                             const char* nombreDeLaFoto) const {
    // ESTUDIO: Instrumentación manual; no equivale a instrucciones CPU.
    iteracionesDadas++;

    PerfilCompleto perfilActual;
    bool seLeyoBien = baseDatos.leerPerfilCompleto(idDelPerfil, perfilActual);
    if (!seLeyoBien) {
        return false;
    }

    int cantidadDeLikesActual = -1;
    int posicionActual = 0;
    // ESTUDIO: Recorrido controlado por condición; evaluar costo y contador.
    while (posicionActual < perfilActual.getCantidadDeFotos() && cantidadDeLikesActual == -1) {
        // ESTUDIO: Instrumentación manual; no equivale a instrucciones CPU.
        iteracionesDadas++;

        FotoConLikes& fotoActual = perfilActual.getFotoEnPosicion(posicionActual);
        if (textosIguales(fotoActual.getNombreDeArchivo(), nombreDeLaFoto)) {
            cantidadDeLikesActual = fotoActual.getCantidadDeLikes();
        }

        posicionActual++;
    }

    if (cantidadDeLikesActual == -1) {
        cout << "Esa foto no existe en ese perfil." << endl;
        return false;
    }

    int nuevaCantidadDeLikes = cantidadDeLikesActual + 1;
    bool seGuardoBien = baseDatos.guardarCambioDeLike(idDelPerfil, nombreDeLaFoto, nuevaCantidadDeLikes);

    if (seGuardoBien) {
        cout << "Le diste like a " << nombreDeLaFoto << ". Ahora tiene "
             << nuevaCantidadDeLikes << " likes." << endl;
    }

    return seGuardoBien;
}

void VisorPerfiles::reiniciarIteraciones() const {
    iteracionesDadas = 0;
}

unsigned long long VisorPerfiles::getIteraciones() const {
    return iteracionesDadas;
}

std::size_t VisorPerfiles::getMemoriaReservada() const {
    return sizeof(*this);
}
