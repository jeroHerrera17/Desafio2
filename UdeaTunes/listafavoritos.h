#ifndef LISTAFAVORITOS_H
#define LISTAFAVORITOS_H

#include <string>

using namespace std;

class listaFavoritos
{
public:
    // Constructores y destructor
    listaFavoritos(string _nombreDueño, string* _lista, int _numeroCanciones);
    listaFavoritos();
    ~listaFavoritos();

    // Setters
    void setLista(string* _lista, int _numeroCanciones);
    void setNombreDueño(const string& ND);
    void setNumeroCanciones(int NC);

    // Getters
    string* getLista() const;
    string getNombreDueño() const;
    int getNumeroCanciones() const;

private:
    string nombreDueño;
    string* lista;
    int numeroCanciones;
};

#endif // LISTAFAVORITOS_H
