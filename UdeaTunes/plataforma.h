#ifndef PLATAFORMA_H
#define PLATAFORMA_H

#include <string>
#include "artista.h"
#include "album.h"
#include "cancion.h"
#include "usuario.h"

using namespace std;

class Plataforma {
private:
    // Datos en memoria
    Artista* artistas;
    Album* albumes;
    Cancion* canciones;

    int cantArtistas;
    int cantAlbumes;
    int cantCanciones;

    // ============================
    // Métodos auxiliares privados
    // ============================
    void limpiarPantalla();
    void pausar();

    // ============================
    // Menús de navegación
    // ============================
    void menuArtistas(Usuario& usuario, bool& salir);

    // Menú interno para usuarios premium
    void menuPremium(Artista* artista, Usuario& usuario, bool& salir);

    // Menú interno para usuarios estándar
    void menuEstandar(Artista* artista, Usuario& usuario, bool& salir);

    // Método opcional para mostrar detalles de una canción
    void mostrarDetallesCancion(Cancion* cancion, bool& volverAlbumes, bool& volverArtistas, bool& salir);

public:
    // ============================
    // Constructor y Destructor
    // ============================
    Plataforma();
    ~Plataforma();

    // ============================
    // Métodos públicos principales
    // ============================
    bool iniciarSesion(const string& archivo, const string& nickname,
                       const string& clave, Usuario& usuario);

    bool cargarDatos(const string& rutaArtistas, const string& rutaAlbumes,
                     const string& rutaCanciones);

    void ejecutar(Usuario& usuario);
    void liberarMemoria();

    // ============================
    // Getters para estadísticas
    // ============================
    int getCantArtistas() const { return cantArtistas; }
    int getCantAlbumes() const { return cantAlbumes; }
    int getCantCanciones() const { return cantCanciones; }
};

#endif
