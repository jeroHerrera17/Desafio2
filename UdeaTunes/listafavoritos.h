#ifndef LISTAFAVORITOS_H
#define LISTAFAVORITOS_H

#include <iostream>
#include <string>
using namespace std;

class listaFavoritos
{
public:
    listaFavoritos();
    void agregarCancion();
    void eliminarCancion();

    //setters
    void setLista(cancion L);
    void setnombreDueño(const string& D);
    void setNumeroCanciones(int NC);
    //getters
    cancion getCancion(cancion l[10000]);
    string getNombreDueño(string ND);
    int getNumeroCanciones(int NC);

private:
    string nombreDueño;
    cancion lista[10000];
    int numeroCanciones;
};

#endif // LISTAFAVORITOS_H
