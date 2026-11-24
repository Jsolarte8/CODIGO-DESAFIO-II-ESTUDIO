// COPIA DE ESTUDIO: lógica original; comentarios didácticos.
#ifndef GESTOR_ARCHIVOS_H
#define GESTOR_ARCHIVOS_H

#include <cstddef>
#include "estructuras_base.h"
#include "monitor_recursos.h"

class AdminAmistades;
class GestorMensajes;

class GestorArchivos {
private:
    UsuarioLigero* arregloUsuariosLigeros;
    int totalUsuarios;
    // ESTUDIO: Instrumentación manual; mutable permite contar en métodos const.
    mutable unsigned long long iteracionesDadas;

public:
    GestorArchivos();
    ~GestorArchivos();

    // ESTUDIO: Carga N usuarios ligeros antes de construir la red.
    bool cargarIndiceUsuarios(const char* rutaArchivo);

    int getTotalUsuarios() const;
    const UsuarioLigero& getUsuarioLigero(int indice) const;

    // ESTUDIO: Carga bajo demanda un perfil individual; reserva arreglo de fotos.
    bool leerPerfilCompleto(const char* idUsuario, PerfilCompleto& perfilDestino) const;
    // ESTUDIO: Relee perfil, recorre fotos y reescribe archivo completo; último recorrido no se cuenta.
    bool guardarCambioDeLike(const char* idUsuario, const char* nombreDeArchivo,
                              int nuevaCantidadDeLikes) const;

    // ESTUDIO: Lee pares y delega a AdminAmistades.
    bool cargarAmistades(const char* rutaArchivo, AdminAmistades& red) const;
    // ESTUDIO: Lee pares dirigidos emisor/receptor.
    bool cargarSolicitudes(const char* rutaArchivo, AdminAmistades& red) const;
    // ESTUDIO: Lee campos y delega validación e inserción ordenada.
    bool cargarMensajes(const char* rutaArchivo, const AdminAmistades& red, GestorMensajes& moduloMensajes) const;

    void reiniciarIteraciones() const;
    unsigned long long getIteraciones() const;
    std::size_t getMemoriaReservada() const;
};

#endif
