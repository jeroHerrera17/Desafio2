#ifndef PLATAFORMA_H
#define PLATAFORMA_H

#include <string>
#include "artista.h"
#include "album.h"
#include "cancion.h"
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

    // Métodos privados auxiliares
    void limpiarPantalla();
    void pausar();

    // Métodos privados de navegación
    void menuArtistas(Usuario& usuario, bool& salir);
    void menuAlbumes(Artista* artistaSeleccionado, bool& volverArtistas, bool& salir);
    void menuCanciones(Album* albumSeleccionado, bool& volverAlbumes, bool& volverArtistas, bool& salir);
    void mostrarDetallesCancion(Cancion* cancion, bool& volverAlbumes, bool& volverArtistas, bool& salir);

public:
    // Constructor y destructor
    Plataforma();
    ~Plataforma();

    // Métodos públicos principales
    bool iniciarSesion(const string& archivo, const string& nickname,
                       const string& clave, Usuario& usuario);
    bool cargarDatos(const string& rutaArtistas, const string& rutaAlbumes,
                     const string& rutaCanciones);
    void ejecutar(const Usuario& usuario);
    void liberarMemoria();

    // Getters para estadísticas
    int getCantArtistas() const { return cantArtistas; }
    int getCantAlbumes() const { return cantAlbumes; }
    int getCantCanciones() const { return cantCanciones; }
};

#endif
