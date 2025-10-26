#ifndef CANCION_H
#define CANCION_H

#include "creditos.h"
#include <string>
using namespace std;

class Cancion {
private:
    int idAlbum;       // ID del álbum al que pertenece (9 dígitos)
    string nombre;
    int duracion;

public:
    Cancion();
    Cancion(int idAlbum, const string& nombre, const string& ruta128,
            const string& ruta320, int duracion, const Creditos& creditos);

    // Getters
    int getIdAlbum() const;
    string getNombre() const;
    int getDuracion() const;

    void reproducir() const;
    void mostrarInfo() const;
    void mostrarResumen() const;

    // Funciones estáticas para cargar canciones
    static Cancion* cargarCanciones(const string& rutaArchivo, int& cantidad);

    // Función antigua para compatibilidad (si la necesitas)
    static void cargarCanciones(const string& rutaArchivo,
                                int idAlbumBuscado,
                                Cancion**& cancionesAlbum,
                                int& cantidadAlbum,
                                Cancion* cancionesGlobal,
                                int totalGlobal);
};

#endif
