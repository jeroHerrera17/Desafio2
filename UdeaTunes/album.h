#ifndef ALBUM_H
#define ALBUM_H

#include <string>
#include "cancion.h"

using namespace std;

// Forward declaration
class publicidad;

/**
 * @brief Representa un álbum musical con sus metadatos y canciones.
 */
class Album {
private:
    int idAlbum;
    string nombre;
    string generos[4];      // Hasta 4 géneros
    string fechaLanzamiento;
    float duracionTotal;
    string sello;
    string portada;
    int puntuacion;

    Cancion* referenciasCanciones;  // Apunta a las canciones asociadas
    int cantidadCanciones;

public:
    Album();
    Album(int idAlbum, const string& nombre, const string* generos,
          const string& fechaLanzamiento, float duracionTotal,
          const string& sello, const string& portada, int puntuacion);

    // Asignar canciones
    void asignarCanciones(Cancion* canciones, int cantidad);

    // Getters
    int getIdAlbum() const;
    string getNombre() const;
    int getCantCanciones() const;
    string getGenero(int indice) const;
    Cancion* getCanciones() const;
    string getPortada() const;

    // Mostrar información
    void mostrarResumen() const;

    // Funciones de reproducción
    void reproducirSecuencial(const string& nombreArtista, const string& rutaPortada);
    void reproducirAleatorio(const string& nombreArtista, const string& rutaPortada);
    void reproducirAleatorioEstandar(publicidad* mensajesPublicitarios, int cantidadPublicidad,
                                     const string& nombreArtista, const string& rutaPortada);

    // Cargar álbumes desde archivo
    static Album* cargarTodos(const string& rutaArchivo, int& cantidad,
                              Cancion* todasCanciones, int totalCanciones);
};

#endif
