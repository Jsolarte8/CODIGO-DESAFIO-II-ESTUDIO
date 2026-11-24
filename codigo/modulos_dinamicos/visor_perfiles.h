// COPIA DE ESTUDIO: comportamiento original conservado.
// Perfil bajo demanda y gestión de likes; no posee red ni base de datos.
#ifndef VISOR_PERFILES_H
#define VISOR_PERFILES_H

#include <cstddef>
#include "../core/gestor_archivos.h"
#include "../core/estructuras_base.h"
#include "admin_amistades.h"

class VisorPerfiles {
private:
    // ESTUDIO: mutable permite contar trabajo desde métodos const.
    mutable unsigned long long iteracionesDadas;

public:
    VisorPerfiles();

    // ESTUDIO: Busca nombre en índice ligero y copia ID al buffer del llamador.
    bool buscarPerfilPorNombre(const GestorArchivos& baseDatos, const char* nombreBuscado,
                                char* idEncontradoDestino) const;

    // ESTUDIO: Carga perfil temporal y filtra fotos públicas o de amigos.
    void mostrarCarrusel(const GestorArchivos& baseDatos, const AdminAmistades& redDeAmistades,
                          const char* idPropio, const char* idEncontrado) const;

    // ESTUDIO: Relee perfil y busca foto; gestor vuelve a leer y reescribe. Falta verificar privacidad aquí.
    bool darLike(const GestorArchivos& baseDatos, const char* idDelPerfil,
                 const char* nombreDeLaFoto) const;

    void reiniciarIteraciones() const;
    unsigned long long getIteraciones() const;
    std::size_t getMemoriaReservada() const;
};

#endif
