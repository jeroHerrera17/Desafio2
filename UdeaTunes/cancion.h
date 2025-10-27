#ifndef CANCION_H
#define CANCION_H

#include "creditos.h"
#include <string>
using namespace std;

class Cancion {
private:
    int idAlbum;            // ID del álbum al que pertenece
    string nombre;          // Nombre de la canción
    string ruta128;         // Ruta al archivo de audio de 128 kbps
    string ruta320;         // Ruta al archivo de audio de 320 kbps
    int duracion;           // Duración de la canción en segundos
    Creditos creditos;      // Créditos asociados a la canción

public:
    // Constructores
    Cancion();
    Cancion(int idAlbum, const string& nombre, const string& ruta128,
            const string& ruta320, int duracion, const Creditos& creditos);
    void asociarCreditos(Cancion* canciones, int cantidad);
    // Getters
    int getIdAlbum() const;
    string getNombre() const;
    string getRuta128() const;
    string getRuta320() const;
    int getDuracion() const;
    Creditos getCreditos() const;

    // Mostrar información
    void reproducir() const;
    void mostrarInfo() const;
    void mostrarResumen() const;

    // Cargar canciones desde archivo
    static Cancion* cargarCanciones(const string& rutaArchivo, int& cantidad);
    void asociarCreditos(Cancion* canciones, int cantidad, const string& rutaCreditos);
    // Método auxiliar (compatibilidad si se necesita por álbum)
    static void cargarCanciones(const string& rutaArchivo,
                                int idAlbumBuscado,
                                Cancion**& cancionesAlbum,
                                int& cantidadAlbum,
                                Cancion* cancionesGlobal,
                                int totalGlobal);
};

#endif
