#ifndef LISTAFAVORITOS_H
#define LISTAFAVORITOS_H

#include <iostream>
#include <string>
#include <cancion.h>

using namespace std;

class listaFavoritos
{
public:
    listaFavoritos(string,string* ,int);
    listaFavoritos();
    ~listaFavoritos();


    void agregarCancion();
    void eliminarCancion();

    //setters
    void setLista(Cancion L);
    void setnombreDueño(const string& D);
    void setNumeroCanciones(int NC);
    //getters
    Cancion getCancion(Cancion l[10000]);
    string getNombreDueño(string ND);
    int getNumeroCanciones(int NC);

private:
    string nombreDueño;
    string* lista;
    int numeroCanciones;
};

#endif // LISTAFAVORITOS_H
