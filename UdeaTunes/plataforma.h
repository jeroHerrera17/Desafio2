#ifndef PLATAFORMA_H
#define PLATAFORMA_H

#include <string>
#include "artista.h"
#include "album.h"
#include "cancion.h"
#include "usuario.h"
#include "listafavoritos.h"

using namespace std;

// Forward declaration
class publicidad;

class Plataforma {
private:
    // Datos en memoria
    Artista* artistas;
    Album* albumes;
    Cancion* canciones;
    listaFavoritos* listasFavoritos;

    int cantArtistas;
    int cantAlbumes;
    int cantCanciones;
    int cantListasFavoritos;


    // Métodos privados auxiliares
    void pausar();

    // Métodos privados de navegación - PREMIUM
    void menuPremium(Usuario& usuario, bool& salir);
    void menuListaFavoritos(Usuario& usuario, bool& salir);
    void menuAlbumesPremium(Artista* artista, bool& volver, bool& salir);
    void menuCancionesPremium(Album* album, Artista* artista, bool& volverAlbum, bool& volverArtistas, bool& salir);

    // Métodos privados de navegación - ESTÁNDAR
    void menuEstandar(Usuario& usuario, bool& salir);
    void menuAlbumesEstandar(Artista* artista, publicidad* mensajesPublicitarios, int cantidadPublicidad, bool& volver, bool& salir);
    void menuCancionesEstandar(Album* album, Artista* artista, publicidad* mensajesPublicitarios, int cantidadPublicidad, bool& volverAlbum, bool& volverArtistas, bool& salir);

public:
    // Constructor y destructor
    Plataforma();
    ~Plataforma();

    // Métodos públicos principales
    bool iniciarSesion(const string& archivo, const string& nickname,
                       const string& clave, Usuario& usuario);
    bool cargarDatos(const string& rutaArtistas, const string& rutaAlbumes,
                     const string& rutaCanciones, const string& rutaListaFavoritos);
    void ejecutar(const Usuario& usuario);
    void liberarMemoria();

    // Getters para estadísticas
    int getCantArtistas() const { return cantArtistas; }
    int getCantAlbumes() const { return cantAlbumes; }
    int getCantCanciones() const { return cantCanciones; }
};

#endif
