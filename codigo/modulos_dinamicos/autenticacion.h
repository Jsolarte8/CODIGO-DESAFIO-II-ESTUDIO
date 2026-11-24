// COPIA DE ESTUDIO: comportamiento original conservado.
// Acceso mediante índice ligero; dueño de copia del ID activo.
#ifndef AUTENTICACION_H
#define AUTENTICACION_H

#include <cstddef>
#include "../core/gestor_archivos.h"
#include "../core/estructuras_base.h"
#include "admin_amistades.h"

class Autenticacion {
private:
    char* idDelUsuarioActivo;
    // ESTUDIO: mutable permite contar trabajo desde métodos const.
    mutable unsigned long long iteracionesDadas;

public:
    Autenticacion();
    ~Autenticacion();

    // ESTUDIO: Busca ID, comprueba contraseña y conserva copia del ID activo.
    bool iniciarSesion(const GestorArchivos& baseDatos);
    const char* getIdDelUsuarioActivo() const;
    // ESTUDIO: Cuello O(N²): cada candidato provoca búsquedas lineales en sonAmigos; luego cuenta comunes y sugiere.
    void mostrarMejoresAmigos(const AdminAmistades& redDeAmistades) const;

    void reiniciarIteraciones() const;
    unsigned long long getIteraciones() const;
    std::size_t getMemoriaReservada() const;
};

#endif
