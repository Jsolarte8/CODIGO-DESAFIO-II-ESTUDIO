// COPIA DE ESTUDIO: comportamiento original conservado.
// Orquestador con referencias no propietarias a objetos que viven en main.
#ifndef MENU_H
#define MENU_H

#include "../core/gestor_archivos.h"
#include "../core/monitor_recursos.h"
#include "autenticacion.h"
#include "admin_amistades.h"
#include "visor_perfiles.h"
#include "gestor_mensajes.h"

class Menu {
private:
    GestorArchivos& baseDatos;
    Autenticacion& moduloAcceso;
    AdminAmistades& moduloAmistades;
    VisorPerfiles& moduloPerfiles;
    GestorMensajes& moduloMensajes;
    MonitorRecursos& monitor;

    // ESTUDIO: Registra usuarios antes de cargar amistades y mensajes.
    void cargarUsuariosEnRedDeAmistades();
    // ESTUDIO: Despacha acciones; fecha de envío está fija.
    void ejecutarOpcion(int opcionElegida);
    // ESTUDIO: Captura cinco módulos y reinicia después del reporte; primero incluye arranque.
    void mostrarReporteDeTodos();

public:
    Menu(GestorArchivos& baseDatosInicial, Autenticacion& accesoInicial,
         AdminAmistades& amistadesInicial, VisorPerfiles& perfilesInicial,
         GestorMensajes& mensajesInicial, MonitorRecursos& monitorInicial);

    // ESTUDIO: Orquesta carga, autenticación, prioridad y menú.
    bool iniciar(const char* rutaIndiceUsuarios);
};

#endif
