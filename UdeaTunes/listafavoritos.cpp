#include "listafavoritos.h"

listaFavoritos::listaFavoritos(string _nombreDueño, string* _lista, int _numeroCanciones) {
    nombreDueño = _nombreDueño;
    numeroCanciones = _numeroCanciones;
    if (_numeroCanciones > 0 && _lista != nullptr) {
        lista = new string[_numeroCanciones];
        for (int i = 0; i < _numeroCanciones; i++)
            lista[i] = _lista[i];
    } else {
        lista = nullptr;
    }
}

listaFavoritos::listaFavoritos() {
    nombreDueño = "";
    lista = nullptr;
    numeroCanciones = 0;
}

listaFavoritos::~listaFavoritos() {
    delete[] lista;
}

void listaFavoritos::setLista(string* _lista, int _numeroCanciones) {
    delete[] lista;
    numeroCanciones = _numeroCanciones;
    if (_numeroCanciones > 0 && _lista != nullptr) {
        lista = new string[_numeroCanciones];
        for (int i = 0; i < _numeroCanciones; i++)
            lista[i] = _lista[i];
    } else {
        lista = nullptr;
    }
}

void listaFavoritos::setNombreDueño(const string& ND) {
    nombreDueño = ND;
}

void listaFavoritos::setNumeroCanciones(int NC) {
    numeroCanciones = NC;
}

string* listaFavoritos::getLista() const {
    return lista;
}

string listaFavoritos::getNombreDueño() const {
    return nombreDueño;
}

int listaFavoritos::getNumeroCanciones() const {
    return numeroCanciones;
}
