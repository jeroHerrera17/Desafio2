#include "listafavoritos.h"

listaFavoritos::listaFavoritos(string _nombreDueño, cancion _lista[10000], int _numeroCanciones) {
    nombreDueño = _nombreDueño;
    numeroCanciones = _numeroCanciones;
    for(int i = 0; i < _numeroCanciones; i++){
        lista[i] = _lista[i];
    }
}

void listaFavoritos::setLista(int L){

}
void listaFavoritos::setNumeroCanciones(int NM) {numeroCanciones = NM; }
void listaFavoritos::setnombreDueño(const string& ND) {nombreDueño = ND; }
