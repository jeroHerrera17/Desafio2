#ifndef ALBUM_H
#define ALBUM_H

#include <string>
using namespace std;

class Album {
private:
    int codigo;                 // Código único del álbum (ej: 1000101)
    string nombre;              // Nombre del álbum
    string genero;              // Género musical
    string fechaLanzamiento;    // Fecha de lanzamiento
    float duracionTotal;        // Duración total en minutos
    string sello;               // Sello disquero
    string portada;             // Ruta de la imagen PNG
    int puntuacion;             // Puntuación de 1 a 10

public:
    // Constructores
    Album();
    Album(int codigo, const string& nombre, const string& genero,
          const string& fechaLanzamiento, float duracionTotal,
          const string& sello, const string& portada, int puntuacion);

    // Getters
    int getCodigo() const;
    string getNombre() const;
    string getGenero() const;
    string getFechaLanzamiento() const;
    float getDuracionTotal() const;
    string getSello() const;
    string getPortada() const;
    int getPuntuacion() const;

    // Mostrar información
    void mostrarInfo() const;

    // Cargar y mostrar solo álbumes de un artista
    static void cargarPorArtista(const string& rutaArchivo, int idArtistaBuscado);
};

#endif
