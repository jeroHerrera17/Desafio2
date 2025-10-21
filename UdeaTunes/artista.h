#ifndef ARTISTA_H
#define ARTISTA_H

#include <string>
using namespace std;

class Artista {
private:
    int codigo;
    string nombre;
    int edad;
    string pais;
    int seguidores;
    int posicion;

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
    void mostrarResumen() const; // Nueva función para mostrar en lista

    // Métodos de archivo
    bool cargarDesdeArchivo(const string& rutaArchivo, int codigoBuscado);
    static void mostrarTodos(const string& rutaArchivo);

    // Nuevo: Cargar todos los artistas en un array dinámico
    static Artista* cargarTodos(const string& rutaArchivo, int& cantidad);
};

#endif
