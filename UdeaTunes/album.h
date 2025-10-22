#ifndef ALBUM_H
#define ALBUM_H

#include <string>
#include "cancion.h"
using namespace std;

class Album {
private:
    int codigo;
    int idArtista;
    string nombre;
    string genero;
    string fechaLanzamiento;
    float duracionTotal;
    string sello;
    string portada;
    int puntuacion;

public:
    Album();
    Album(int codigo, int idArtista, const string& nombre, const string& genero,
          const string& fechaLanzamiento, float duracionTotal,
          const string& sello, const string& portada, int puntuacion);

    // Getters
    // album.h
    int getCodigo() const;
    int getIdArtista() const;
    string getNombre() const;
    string getGenero() const;
    string getFechaLanzamiento() const;
    float getDuracionTotal() const;
    string getSello() const;
    string getPortada() const;
    int getPuntuacion() const;


    // Funciones principales
    void mostrarInfo() const;
    void mostrarResumen() const;

    void reproducirAleatorio(Cancion* canciones, int totalCanciones, int codigoAlbum);

    // Métodos estáticos
    static void cargarPorArtista(const string& rutaArchivo, int idArtistaBuscado);
    static Album* cargarTodos(const string& rutaArchivo, int& cantidad);
};

#endif
