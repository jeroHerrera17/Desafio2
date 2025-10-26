#ifndef ARTISTA_H
#define ARTISTA_H

#include <string>
using namespace std;

// Forward declaration
class Album;

class Artista {
private:
    int codigo;
    string nombre;
    int edad;
    string pais;
    int seguidores;
    int posicion;

    // Referencias a álbumes del artista
    Album** albumes;
    int cantAlbumes;

public:
    // Constructores
    Artista();
    Artista(int codigo, const string& nombre, int edad, const string& pais,
            int seguidores, int posicion);

    // Getters
    int getCodigo() const;
    string getNombre() const;
    int getEdad() const;
    string getPais() const;
    int getSeguidores() const;
    int getPosicion() const;

    // Setters
    void setCodigo(int codigo);
    void setNombre(const string& nombre);
    void setEdad(int edad);
    void setPais(const string& pais);
    void setSeguidores(int seguidores);
    void setPosicion(int posicion);

    // Métodos de visualización
    void mostrarInfo() const;
    void mostrarResumen() const;

    // Manejo de álbumes
    void setAlbumes(Album** albumesArtista, int cantidad);
    Album** getAlbumes() const;
    int getCantAlbumes() const;

    // Métodos de archivo
    bool cargarDesdeArchivo(const string& rutaArchivo, int codigoBuscado);
    static void mostrarTodos(const string& rutaArchivo);

    // Cargar todos los artistas y asignarles sus álbumes
    static Artista* cargarTodos(const string& rutaArchivo, int& cantidad,
                                Album* albumesGlobal, int totalAlbumes);
};

#endif
