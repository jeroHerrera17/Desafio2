#ifndef CANCION_H
#define CANCION_H

#include <string>
#include "creditos.h"
using namespace std;

class Cancion {
private:
    int idAlbum;           // Primeros 7 dígitos del ID completo
    string nombre;
    string rutaAudio128;
    string rutaAudio320;
    int duracion;          // En segundos
    Creditos creditos;

public:
    // Constructores
    Cancion();
    Cancion(int idAlbum, const string& nombre, const string& rutaAudio128,
            const string& rutaAudio320, int duracion, const Creditos& creditos);

    // Métodos
    void mostrarInfo() const;
    void mostrarResumen() const;
    static void cargarPorAlbum(const string& rutaArchivo, int idAlbumBuscado);

    // IMPORTANTE: Cargar todas las canciones en memoria
    static Cancion* cargarTodas(const string& rutaArchivo, int& cantidad);

    // Getters
    int getIdAlbum() const { return idAlbum; }
    string getNombre() const { return nombre; }
    int getDuracion() const { return duracion; }
    void reproducir() const;
};

// Función auxiliar para parsear créditos
Creditos parsearCreditos(const string& texto);

#endif
