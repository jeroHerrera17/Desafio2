#ifndef CANCION_H
#define CANCION_H

#include <string>
using namespace std;

class Cancion {
private:
    int id;
    int idAlbum;
    string nombre;
    string duracion;
    string rutaAudio128;
    string rutaAudio320;
    string creditos;
    int reproducciones;

public:
    Cancion();
    Cancion(int id, int idAlbum, const string& nombre, const string& duracion,
            const string& rutaAudio128, const string& rutaAudio320,
            const string& creditos, int reproducciones);

    void mostrarInfo() const;
    void cargarPorAlbum(const string& rutaArchivo, int idAlbumBuscado);
};

#endif
