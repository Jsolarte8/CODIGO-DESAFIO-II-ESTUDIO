// COPIA DE ESTUDIO: lógica original; comentarios didácticos.
#include "gestor_archivos.h"
#include <iostream>
#include <fstream>
#include "../modulos_dinamicos/admin_amistades.h"
#include "../modulos_dinamicos/gestor_mensajes.h"

using namespace std;

// ESTUDIO: Convierte dígitos ASCII a entero decimal sin validación completa.
static int convertirTextoANumero(const char* texto) {
    int numero = 0;
    int posicion = 0;
    // ESTUDIO: Recorrido: identificar límite y costo; no todos los ciclos se contabilizan.
    while (texto[posicion] != '\0') {
        numero = numero * 10 + (texto[posicion] - '0');
        posicion++;
    }
    return numero;
}

// ESTUDIO: Extrae campo delimitado, reserva memoria y avanza cursor por referencia; el llamador libera.
static char* extraerCampo(const char* linea, int& inicio, char delimitador) {
    int fin = inicio;
    // ESTUDIO: Recorrido: identificar límite y costo; no todos los ciclos se contabilizan.
    while (linea[fin] != delimitador && linea[fin] != '\0') {
        fin++;
    }

    int longitud = fin - inicio;
    // ESTUDIO: Reserva memoria dinámica; requiere liberación correspondiente.
    char* campo = new char[longitud + 1];
    // ESTUDIO: Recorrido: identificar límite y costo; no todos los ciclos se contabilizan.
    for (int i = 0; i < longitud; i++) {
        campo[i] = linea[inicio + i];
    }
    campo[longitud] = '\0';

    inicio = (linea[fin] == delimitador) ? fin + 1 : fin;
    return campo;
}

static void quitarRetornoDeCarroSiExiste(char* texto) {
    int longitud = 0;
    // ESTUDIO: Recorrido: identificar límite y costo; no todos los ciclos se contabilizan.
    while (texto[longitud] != '\0') {
        longitud++;
    }
    if (longitud > 0 && texto[longitud - 1] == '\r') {
        texto[longitud - 1] = '\0';
    }
}

static bool textosIguales(const char* primero, const char* segundo) {
    int posicion = 0;
    // ESTUDIO: Recorrido: identificar límite y costo; no todos los ciclos se contabilizan.
    while (primero[posicion] != '\0' && segundo[posicion] != '\0') {
        if (primero[posicion] != segundo[posicion]) {
            return false;
        }
        posicion++;
    }
    return primero[posicion] == segundo[posicion];
}

// ESTUDIO: Forma ruta relativa a partir de ID.
static void construirRutaDePerfil(const char* idUsuario, char* rutaDestino, int tamanoMaximo) {
    const char* prefijo = "estaticos_y_db/perfiles/";
    const char* sufijo = ".txt";
    int posicionDestino = 0;

    int posicionPrefijo = 0;
    // ESTUDIO: Recorrido: identificar límite y costo; no todos los ciclos se contabilizan.
    while (prefijo[posicionPrefijo] != '\0' && posicionDestino < tamanoMaximo - 1) {
        rutaDestino[posicionDestino] = prefijo[posicionPrefijo];
        posicionDestino++;
        posicionPrefijo++;
    }

    int posicionId = 0;
    // ESTUDIO: Recorrido: identificar límite y costo; no todos los ciclos se contabilizan.
    while (idUsuario[posicionId] != '\0' && posicionDestino < tamanoMaximo - 1) {
        rutaDestino[posicionDestino] = idUsuario[posicionId];
        posicionDestino++;
        posicionId++;
    }

    int posicionSufijo = 0;
    // ESTUDIO: Recorrido: identificar límite y costo; no todos los ciclos se contabilizan.
    while (sufijo[posicionSufijo] != '\0' && posicionDestino < tamanoMaximo - 1) {
        rutaDestino[posicionDestino] = sufijo[posicionSufijo];
        posicionDestino++;
        posicionSufijo++;
    }

    rutaDestino[posicionDestino] = '\0';
}

GestorArchivos::GestorArchivos() {
    arregloUsuariosLigeros = nullptr;
    totalUsuarios = 0;
    iteracionesDadas = 0;
}

GestorArchivos::~GestorArchivos() {
    // ESTUDIO: Libera memoria del propietario.
    delete[] arregloUsuariosLigeros;
}

// ESTUDIO: Carga N usuarios ligeros antes de construir la red.
bool GestorArchivos::cargarIndiceUsuarios(const char* rutaArchivo) {
    ifstream archivoLectura(rutaArchivo);
    if (!archivoLectura.is_open()) {
        cout << "Error critico no se pudo abrir la base de datos ligera." << endl;
        return false;
    }

    archivoLectura >> this->totalUsuarios;
    archivoLectura.ignore();

    // ESTUDIO: Reserva memoria dinámica; requiere liberación correspondiente.
    this->arregloUsuariosLigeros = new UsuarioLigero[this->totalUsuarios];

    const int TAM_MAX_LINEA = 256;
    char buffer[TAM_MAX_LINEA];

    int explorador = 0;
    // ESTUDIO: Recorrido: identificar límite y costo; no todos los ciclos se contabilizan.
    while (explorador < this->totalUsuarios) {
        // ESTUDIO: Instrumentación manual; mutable permite contar en métodos const.
        iteracionesDadas++;

        archivoLectura.getline(buffer, TAM_MAX_LINEA);
        quitarRetornoDeCarroSiExiste(buffer);

        int cursor = 0;
        // ESTUDIO: Extrae campo delimitado, reserva memoria y avanza cursor por referencia; el llamador libera.
        char* idLeido = extraerCampo(buffer, cursor, ';');
        // ESTUDIO: Extrae campo delimitado, reserva memoria y avanza cursor por referencia; el llamador libera.
        char* contrasenaLeida = extraerCampo(buffer, cursor, ';');
        // ESTUDIO: Extrae campo delimitado, reserva memoria y avanza cursor por referencia; el llamador libera.
        char* nombreLeido = extraerCampo(buffer, cursor, ';');

        this->arregloUsuariosLigeros[explorador].setIdUsuario(idLeido);
        this->arregloUsuariosLigeros[explorador].setContrasena(contrasenaLeida);
        this->arregloUsuariosLigeros[explorador].setNombreUsuario(nombreLeido);

        // ESTUDIO: Libera memoria del propietario.
        delete[] idLeido;
        // ESTUDIO: Libera memoria del propietario.
        delete[] contrasenaLeida;
        // ESTUDIO: Libera memoria del propietario.
        delete[] nombreLeido;
        explorador++;
    }

    archivoLectura.close();

    cout << "indice cargado bien. total usuarios: " << this->totalUsuarios << endl;
    return true;
}

int GestorArchivos::getTotalUsuarios() const {
    return totalUsuarios;
}

const UsuarioLigero& GestorArchivos::getUsuarioLigero(int indice) const {
    return arregloUsuariosLigeros[indice];
}

// ESTUDIO: Carga bajo demanda un perfil individual; reserva arreglo de fotos.
bool GestorArchivos::leerPerfilCompleto(const char* idUsuario, PerfilCompleto& perfilDestino) const {
    // ESTUDIO: Instrumentación manual; mutable permite contar en métodos const.
    iteracionesDadas++;

    const int TAM_MAX_RUTA = 256;
    char rutaDelPerfil[TAM_MAX_RUTA];
    construirRutaDePerfil(idUsuario, rutaDelPerfil, TAM_MAX_RUTA);

    ifstream archivoLectura(rutaDelPerfil);
    if (!archivoLectura.is_open()) {
        cout << "Error: no existe el archivo de perfil para el usuario " << idUsuario << endl;
        return false;
    }

    const int TAM_MAX_LINEA = 256;
    char buffer[TAM_MAX_LINEA];

    archivoLectura.getline(buffer, TAM_MAX_LINEA);
    quitarRetornoDeCarroSiExiste(buffer);
    perfilDestino.setRutaCarpetaDeFotos(buffer);

    int cantidadDeFotos = 0;
    archivoLectura >> cantidadDeFotos;
    archivoLectura.ignore();

    perfilDestino.inicializarFotos(cantidadDeFotos);

    int posicionActual = 0;
    // ESTUDIO: Recorrido: identificar límite y costo; no todos los ciclos se contabilizan.
    while (posicionActual < cantidadDeFotos) {
        // ESTUDIO: Instrumentación manual; mutable permite contar en métodos const.
        iteracionesDadas++;

        archivoLectura.getline(buffer, TAM_MAX_LINEA);
        quitarRetornoDeCarroSiExiste(buffer);

        int cursor = 0;
        // ESTUDIO: Extrae campo delimitado, reserva memoria y avanza cursor por referencia; el llamador libera.
        char* nombreLeido = extraerCampo(buffer, cursor, ';');
        // ESTUDIO: Extrae campo delimitado, reserva memoria y avanza cursor por referencia; el llamador libera.
        char* likesLeidosTexto = extraerCampo(buffer, cursor, ';');
        // ESTUDIO: Extrae campo delimitado, reserva memoria y avanza cursor por referencia; el llamador libera.
        char* esPublicaTexto = extraerCampo(buffer, cursor, ';');

        int likesLeidos = 0;
        int posicionDigito = 0;
        // ESTUDIO: Recorrido: identificar límite y costo; no todos los ciclos se contabilizan.
        while (likesLeidosTexto[posicionDigito] != '\0') {
            likesLeidos = likesLeidos * 10 + (likesLeidosTexto[posicionDigito] - '0');
            posicionDigito++;
        }

        bool esPublicaLeida = (esPublicaTexto[0] == '1');

        FotoConLikes& fotoActual = perfilDestino.getFotoEnPosicion(posicionActual);
        fotoActual.setNombreDeArchivo(nombreLeido);
        fotoActual.setCantidadDeLikes(likesLeidos);
        fotoActual.setEsPublica(esPublicaLeida);

        // ESTUDIO: Libera memoria del propietario.
        delete[] nombreLeido;
        // ESTUDIO: Libera memoria del propietario.
        delete[] likesLeidosTexto;
        // ESTUDIO: Libera memoria del propietario.
        delete[] esPublicaTexto;

        posicionActual = posicionActual + 1;
    }

    archivoLectura.close();
    return true;
}

// ESTUDIO: Relee perfil, recorre fotos y reescribe archivo completo; último recorrido no se cuenta.
bool GestorArchivos::guardarCambioDeLike(const char* idUsuario, const char* nombreDeArchivo,
                                          int nuevaCantidadDeLikes) const {
    // ESTUDIO: Instrumentación manual; mutable permite contar en métodos const.
    iteracionesDadas++;

    PerfilCompleto perfilActual;
    // ESTUDIO: Carga bajo demanda un perfil individual; reserva arreglo de fotos.
    bool seLeyoBien = leerPerfilCompleto(idUsuario, perfilActual);
    if (!seLeyoBien) {
        return false;
    }

    bool seEncontroLaFoto = false;
    int posicionActual = 0;
    // ESTUDIO: Recorrido: identificar límite y costo; no todos los ciclos se contabilizan.
    while (posicionActual < perfilActual.getCantidadDeFotos()) {
        // ESTUDIO: Instrumentación manual; mutable permite contar en métodos const.
        iteracionesDadas++;

        FotoConLikes& fotoActual = perfilActual.getFotoEnPosicion(posicionActual);
        if (textosIguales(fotoActual.getNombreDeArchivo(), nombreDeArchivo)) {
            fotoActual.setCantidadDeLikes(nuevaCantidadDeLikes);
            seEncontroLaFoto = true;
        }

        posicionActual = posicionActual + 1;
    }

    if (!seEncontroLaFoto) {
        cout << "Error: esa foto no existe en el perfil del usuario " << idUsuario << endl;
        return false;
    }

    const int TAM_MAX_RUTA = 256;
    char rutaDelPerfil[TAM_MAX_RUTA];
    construirRutaDePerfil(idUsuario, rutaDelPerfil, TAM_MAX_RUTA);

    ofstream archivoEscritura(rutaDelPerfil);
    if (!archivoEscritura.is_open()) {
        cout << "Error: no se pudo volver a abrir el archivo de perfil para guardar el like." << endl;
        return false;
    }

    archivoEscritura << perfilActual.getRutaCarpetaDeFotos() << endl;
    archivoEscritura << perfilActual.getCantidadDeFotos() << endl;

    posicionActual = 0;
    // ESTUDIO: Recorrido: identificar límite y costo; no todos los ciclos se contabilizan.
    while (posicionActual < perfilActual.getCantidadDeFotos()) {
        FotoConLikes& fotoActual = perfilActual.getFotoEnPosicion(posicionActual);
        archivoEscritura << fotoActual.getNombreDeArchivo() << ";"
                          << fotoActual.getCantidadDeLikes() << ";"
                          << (fotoActual.getEsPublica() ? 1 : 0) << endl;
        posicionActual = posicionActual + 1;
    }

    archivoEscritura.close();
    return true;
}

// ESTUDIO: Lee pares y delega a AdminAmistades.
bool GestorArchivos::cargarAmistades(const char* rutaArchivo, AdminAmistades& red) const {
    ifstream archivoLectura(rutaArchivo);
    if (!archivoLectura.is_open()) {
        return false;
    }

    int totalAmistades = 0;
    archivoLectura >> totalAmistades;
    archivoLectura.ignore();

    const int TAM_MAX_LINEA = 256;
    char buffer[TAM_MAX_LINEA];

    int i = 0;
    // ESTUDIO: Recorrido: identificar límite y costo; no todos los ciclos se contabilizan.
    while (i < totalAmistades && archivoLectura.getline(buffer, TAM_MAX_LINEA)) {
        // ESTUDIO: Instrumentación manual; mutable permite contar en métodos const.
        iteracionesDadas++;
        quitarRetornoDeCarroSiExiste(buffer);

        int cursor = 0;
        // ESTUDIO: Extrae campo delimitado, reserva memoria y avanza cursor por referencia; el llamador libera.
        char* id1Leido = extraerCampo(buffer, cursor, ';');
        // ESTUDIO: Extrae campo delimitado, reserva memoria y avanza cursor por referencia; el llamador libera.
        char* id2Leido = extraerCampo(buffer, cursor, ';');

        // ESTUDIO: Convierte dígitos ASCII a entero decimal sin validación completa.
        int id1 = convertirTextoANumero(id1Leido);
        // ESTUDIO: Convierte dígitos ASCII a entero decimal sin validación completa.
        int id2 = convertirTextoANumero(id2Leido);

        red.cargarAmistad(id1, id2);

        // ESTUDIO: Libera memoria del propietario.
        delete[] id1Leido;
        // ESTUDIO: Libera memoria del propietario.
        delete[] id2Leido;
        i++;
    }

    archivoLectura.close();
    return true;
}

// ESTUDIO: Lee pares dirigidos emisor/receptor.
bool GestorArchivos::cargarSolicitudes(const char* rutaArchivo, AdminAmistades& red) const {
    ifstream archivoLectura(rutaArchivo);
    if (!archivoLectura.is_open()) {
        return false;
    }

    int totalSolicitudes = 0;
    archivoLectura >> totalSolicitudes;
    archivoLectura.ignore();

    const int TAM_MAX_LINEA = 256;
    char buffer[TAM_MAX_LINEA];

    int i = 0;
    // ESTUDIO: Recorrido: identificar límite y costo; no todos los ciclos se contabilizan.
    while (i < totalSolicitudes && archivoLectura.getline(buffer, TAM_MAX_LINEA)) {
        // ESTUDIO: Instrumentación manual; mutable permite contar en métodos const.
        iteracionesDadas++;
        quitarRetornoDeCarroSiExiste(buffer);

        int cursor = 0;
        // ESTUDIO: Extrae campo delimitado, reserva memoria y avanza cursor por referencia; el llamador libera.
        char* idEmisorLeido = extraerCampo(buffer, cursor, ';');
        // ESTUDIO: Extrae campo delimitado, reserva memoria y avanza cursor por referencia; el llamador libera.
        char* idReceptorLeido = extraerCampo(buffer, cursor, ';');

        // ESTUDIO: Convierte dígitos ASCII a entero decimal sin validación completa.
        int emisor = convertirTextoANumero(idEmisorLeido);
        // ESTUDIO: Convierte dígitos ASCII a entero decimal sin validación completa.
        int receptor = convertirTextoANumero(idReceptorLeido);

        red.cargarSolicitud(emisor, receptor);

        // ESTUDIO: Libera memoria del propietario.
        delete[] idEmisorLeido;
        // ESTUDIO: Libera memoria del propietario.
        delete[] idReceptorLeido;
        i++;
    }

    archivoLectura.close();
    return true;
}

// ESTUDIO: Lee campos y delega validación e inserción ordenada.
bool GestorArchivos::cargarMensajes(const char* rutaArchivo, const AdminAmistades& red,
                                    GestorMensajes& moduloMensajes) const {
    ifstream archivoLectura(rutaArchivo);
    if (!archivoLectura.is_open()) {
        return false;
    }

    int totalMensajes = 0;
    archivoLectura >> totalMensajes;
    archivoLectura.ignore();

    const int TAM_MAX_LINEA = 2048;
    char buffer[TAM_MAX_LINEA];

    int i = 0;
    // ESTUDIO: Recorrido: identificar límite y costo; no todos los ciclos se contabilizan.
    while (i < totalMensajes && archivoLectura.getline(buffer, TAM_MAX_LINEA)) {
        // ESTUDIO: Instrumentación manual; mutable permite contar en métodos const.
        iteracionesDadas++;
        quitarRetornoDeCarroSiExiste(buffer);

        int cursor = 0;
        // ESTUDIO: Extrae campo delimitado, reserva memoria y avanza cursor por referencia; el llamador libera.
        char* idLeido = extraerCampo(buffer, cursor, ';');
        // ESTUDIO: Extrae campo delimitado, reserva memoria y avanza cursor por referencia; el llamador libera.
        char* emisorLeido = extraerCampo(buffer, cursor, ';');
        // ESTUDIO: Extrae campo delimitado, reserva memoria y avanza cursor por referencia; el llamador libera.
        char* receptorLeido = extraerCampo(buffer, cursor, ';');
        // ESTUDIO: Extrae campo delimitado, reserva memoria y avanza cursor por referencia; el llamador libera.
        char* diaLeido = extraerCampo(buffer, cursor, ';');
        // ESTUDIO: Extrae campo delimitado, reserva memoria y avanza cursor por referencia; el llamador libera.
        char* mesLeido = extraerCampo(buffer, cursor, ';');
        // ESTUDIO: Extrae campo delimitado, reserva memoria y avanza cursor por referencia; el llamador libera.
        char* anioLeido = extraerCampo(buffer, cursor, ';');
        // ESTUDIO: Extrae campo delimitado, reserva memoria y avanza cursor por referencia; el llamador libera.
        char* textoLeido = extraerCampo(buffer, cursor, ';');
        // ESTUDIO: Extrae campo delimitado, reserva memoria y avanza cursor por referencia; el llamador libera.
        char* leidoTexto = extraerCampo(buffer, cursor, ';');

        // ESTUDIO: Convierte dígitos ASCII a entero decimal sin validación completa.
        int id = convertirTextoANumero(idLeido);
        // ESTUDIO: Convierte dígitos ASCII a entero decimal sin validación completa.
        int emisor = convertirTextoANumero(emisorLeido);
        // ESTUDIO: Convierte dígitos ASCII a entero decimal sin validación completa.
        int receptor = convertirTextoANumero(receptorLeido);
        // ESTUDIO: Convierte dígitos ASCII a entero decimal sin validación completa.
        int dia = convertirTextoANumero(diaLeido);
        // ESTUDIO: Convierte dígitos ASCII a entero decimal sin validación completa.
        int mes = convertirTextoANumero(mesLeido);
        // ESTUDIO: Convierte dígitos ASCII a entero decimal sin validación completa.
        int anio = convertirTextoANumero(anioLeido);
        bool estaLeido = (leidoTexto[0] == '1');

        moduloMensajes.cargarMensaje(red, id, emisor, receptor, Fecha(dia, mes, anio), textoLeido, estaLeido);

        // ESTUDIO: Libera memoria del propietario.
        delete[] idLeido;
        // ESTUDIO: Libera memoria del propietario.
        delete[] emisorLeido;
        // ESTUDIO: Libera memoria del propietario.
        delete[] receptorLeido;
        // ESTUDIO: Libera memoria del propietario.
        delete[] diaLeido;
        // ESTUDIO: Libera memoria del propietario.
        delete[] mesLeido;
        // ESTUDIO: Libera memoria del propietario.
        delete[] anioLeido;
        // ESTUDIO: Libera memoria del propietario.
        delete[] textoLeido;
        // ESTUDIO: Libera memoria del propietario.
        delete[] leidoTexto;

        i++;
    }

    archivoLectura.close();
    return true;
}

void GestorArchivos::reiniciarIteraciones() const {
    iteracionesDadas = 0;
}

unsigned long long GestorArchivos::getIteraciones() const {
    return iteracionesDadas;
}

std::size_t GestorArchivos::getMemoriaReservada() const {
    return sizeof(*this) + static_cast<std::size_t>(totalUsuarios) * sizeof(UsuarioLigero);
}
