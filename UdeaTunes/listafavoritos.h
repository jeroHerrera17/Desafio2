#ifndef LISTAFAVORITOS_H
#define LISTAFAVORITOS_H

#include <string>
#include <cancion.h>
using namespace std;

class listaFavoritos
{
public:
    // Constructores y destructor
    listaFavoritos(string _nombreDueño, Cancion** _lista, int _numeroCanciones);
    listaFavoritos();
    ~listaFavoritos();

    //Cargar las listas de favoritos
    static listaFavoritos* cargarTodos(const string& rutaArchivo, int& cantidad, Cancion* todasCanciones, int totalCanciones);

    //REPRODUCTOR.
    void reproducirListaFavoritos();

    //AGREGAR Y ELIMINAR CANCIONES A LA LISTA
    void agregarCancion(const string&, Cancion* todas, int total);
    void eliminarCancion(const string&);

    // Setters
    void setLista(Cancion** _lista, int _numeroCanciones);
    void setNombreDueño(const string& ND);
    void setNumeroCanciones(int NC);

    // Getters
    Cancion** getLista() const;
    string getNombreDueño() const;
    int getNumeroCanciones() const;

private:
    string nombreDueño;
    Cancion** lista;
    int numeroCanciones;
};

#endif // LISTAFAVORITOS_H
