#include "listafavoritos.h"

listaFavoritos::listaFavoritos(string _nombreDueño,string* _lista, int _numeroCanciones) {
    nombreDueño = _nombreDueño;
    lista = new string[_numeroCanciones];
    for(int i = 0; i < _numeroCanciones; i++){
        lista[i] = _lista[i];
    }
    numeroCanciones  = _numeroCanciones;

    //void listaFavoritos::setLista(int L
}

listaFavoritos::listaFavoritos(){
    nombreDueño = "";
    lista = nullptr;
    numeroCanciones = 0;
}
void listaFavoritos::setNumeroCanciones(int NM) {numeroCanciones = NM; }
void listaFavoritos::setnombreDueño(const string& ND) {nombreDueño = ND; }
