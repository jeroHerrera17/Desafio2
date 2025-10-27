#ifndef ALBUM_H
#define ALBUM_H
#include "usuario.h"
#include <string>
#include "cancion.h"
#include "publicidad.h"
using namespace std;

/**
 * Clase Album
 * Representa un álbum musical con hasta 4 géneros y una colección de canciones.
 */
class Album {
private:
    // ==========================
    // ATRIBUTOS
    // ==========================
    int idAlbum;                    ///< Identificador único del álbum
    string nombre;                  ///< Nombre del álbum
    string generos[4];              ///< Hasta 4 géneros por álbum
    string fechaLanzamiento;        ///< Fecha de lanzamiento (formato AAAA-MM-DD)
    float duracionTotal;            ///< Duración total en minutos
    string sello;                   ///< Sello discográfico
    string portada;                 ///< Nombre o ruta de la imagen de portada
    int puntuacion;                 ///< Puntuación de 1 a 10
    Cancion** referenciasCanciones; ///< Punteros a las canciones del álbum
    int cantidadCanciones;          ///< Número de canciones asociadas

public:
    // ==========================
    // CONSTRUCTORES
    // ==========================
    Album();    ///< Constructor por defecto

    Album(int idAlbum, const string& nombre, const string generos[4],
          const string& fechaLanzamiento, float duracionTotal,
          const string& sello, const string& portada, int puntuacion);

    // ==========================
    // GETTERS
    // ==========================
    int getIdAlbum() const;                ///< Devuelve el ID del álbum
    string getNombre() const;              ///< Devuelve el nombre del álbum
    string getGenero(int i) const;         ///< Devuelve el género en la posición i
    string getFechaLanzamiento() const;    ///< Devuelve la fecha de lanzamiento
    float getDuracionTotal() const;        ///< Devuelve la duración total
    string getSello() const;               ///< Devuelve el sello discográfico
    string getPortada() const;             ///< Devuelve el nombre o ruta de la portada
    int getPuntuacion() const;             ///< Devuelve la puntuación
    Cancion** getCanciones() const;        ///< Devuelve las referencias a canciones
    int getCantCanciones() const;          ///< Devuelve la cantidad de canciones

    // ==========================
    // SETTERS
    // ==========================
    void setGenero(int i, const string& g); ///< Establece un género en la posición i
    void setCanciones(Cancion** canciones, int cantidad); ///< Asigna canciones al álbum

    // ==========================
    // VISUALIZACIÓN
    // ==========================
    void mostrarInfo() const;      ///< Muestra toda la información detallada del álbum
    void mostrarResumen() const;   ///< Muestra un resumen con nombre, géneros y puntuación
    void mostrarCanciones() const; ///< Muestra las canciones del álbum

    // ==========================
    // FUNCIONALIDAD
    // ==========================
    void asignarCanciones(Cancion* todas, int total); ///< Asocia canciones por ID del álbum
    void reproducirAleatorio(); ///< Reproduce una canción aleatoria del álbum
    void reproducirAleatorioEstandar(publicidad* mensajesPublicitarios, int cantidadPublicidad);
    void reproducirSecuencial();
    // ==========================
    // ARCHIVOS
    // ==========================
    static Album* cargarTodos(const string& rutaArchivo, int& cantidad,
                              Cancion* todasCanciones, int totalCanciones); ///< Carga álbumes desde archivo y asigna canciones
    static void guardarEnArchivo(const string& rutaArchivo, Album* albums, int cantidad); ///< Guarda álbumes en archivo
};

#endif // ALBUM_H
