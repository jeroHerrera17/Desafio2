#include "listafavoritos.h"
#include <iostream>
#include <fstream>
#include <sstream>
#include <cstdlib>
#include <ctime>
#include <thread>
using namespace std;

listaFavoritos::listaFavoritos(string _nombreDueño, Cancion** _lista, int _numeroCanciones) {
    nombreDueño = _nombreDueño;
    numeroCanciones = _numeroCanciones;
    lista = _lista;
}

listaFavoritos::listaFavoritos() {
    nombreDueño = "";
    lista = nullptr;
    numeroCanciones = 0;
}

listaFavoritos::~listaFavoritos() {
    delete[] lista;
}

/*
 *----------------------------------------
 *AGREGAR y ELIMINAR canciones de la lista
 *----------------------------------------
*/
void listaFavoritos::agregarCancion(const string& idCancion,Cancion* todas, int total){
    int nuevoNumeroCanciones = numeroCanciones + 1;
    Cancion** nuevaLista = new Cancion*[nuevoNumeroCanciones];
    for(int i = 0; i < numeroCanciones; i++){
        if(stoi(idCancion) == lista[i]->getIdAlbum()){
            cout << "Esta Cancion ya se encuentra en la lista ..." << endl;
            delete[] nuevaLista;
            return;
        }
        else{
            nuevaLista[i] = lista[i];
        }
    }
    int idCancionInt = stoi(idCancion);
    for(int j = 0; j < total; j++){
        if(idCancionInt == todas[j].getIdAlbum()){
            nuevaLista[nuevoNumeroCanciones - 1] = &todas[j];
            break;
        }
    }
    delete[] lista;

    lista = nuevaLista;
    numeroCanciones = nuevoNumeroCanciones;
}

void listaFavoritos::eliminarCancion(const string& idCancion){
    int nuevoNumeroCanciones = numeroCanciones - 1;
    bool encontrado = false;
    Cancion** nuevaLista = new Cancion*[nuevoNumeroCanciones];
    int j = 0;
    for(int i = 0; i < numeroCanciones; i++){
        if(stoi(idCancion) != lista[i]->getIdAlbum()){
            nuevaLista[i] = lista[j];
            j++;
        }
        else{
            cout << "Cancion eliminada con exito :D" << endl;
            encontrado = true;
            j++;
        }
    }
    if(encontrado == false){
        cout << "la cancion no se encuentra en la lista de favoritos" << endl;
        delete[] nuevaLista;
        return;
    }
    else{
        delete[] lista;
        lista = nuevaLista;
        numeroCanciones = nuevoNumeroCanciones;
    }
}


/*
 *-----------------------------
 *REPRODUCIR LISTA DE FAVORITOS
 *-----------------------------
*/

void listaFavoritos::reproducirListaFavoritos(){
    if(lista == nullptr || numeroCanciones == 0){
        cout << "la lista de favoritos no tiene canciones guardadas";
        return;
    }

    int tipoReproduccion, siguienteAccion;
    cout << "como deseas reproducir tu lista de favoritos?" << endl
         << "1. Reproducir en su orden original" << endl
         << "2.reproducir en orden aleatorio" << endl;
    cin >> tipoReproduccion;
    switch(tipoReproduccion){

    //REPRODUCCION EN ORDEN ORIGINAL DE LA LISTA
    case 1:{
        cout <<"-------------------------------------" << endl
             <<"Reproduciendo en su orden original."<< endl
             <<"-------------------------------------" << endl;
        int posCancion = 0;

        //REPRODUCCION SI LA LLISTA TIENE MENOS DE 7 CANCIONES
        if(numeroCanciones < 7){
            while( posCancion < numeroCanciones){
                lista[posCancion] ->mostrarInfo();
                this_thread::sleep_for(chrono::seconds(3));

                //Siguiente accion
                cout << "que deseas hacer ahora?" << endl
                     << "1.reproducir siguiente cancion" << endl
                     << "2.reproducir de nuevo la cancion actual" << endl
                     << "3.reproducir la cancion anterior" << endl;
                cin >> siguienteAccion;
                switch (siguienteAccion) {
                case 1:
                    posCancion ++;
                    break;
                case 2:
                    break;
                case 3:
                    if (posCancion == 0){
                        cout << "Esta es la primer cancion, no se puede reproducir la anterior, reproduciendo siguiente cancion";
                        posCancion ++;
                    }
                    else{
                        posCancion --;
                    }
                    break;
                default:
                    cout << "Esta no es una opcion valida, reproduciendo siguiente cancion";
                    posCancion ++;
                }
                cout << endl << endl;
            }
        }

        //REPRODUCCION SI TIENE 7 o MAS CANCIONES EN LA LISTA
        else{
            while( posCancion < 7){
                lista[posCancion] ->mostrarInfo();
                this_thread::sleep_for(chrono::seconds(3));

                //Siguiente accion
                cout << "que deseas hacer ahora?" << endl
                     << "1.reproducir siguiente cancion" << endl
                     << "2.reproducir de nuevo la cancion actual" << endl
                     << "3.reproducir la cancion anterior" << endl;
                cin >> siguienteAccion;
                switch (siguienteAccion) {
                case 1:
                    posCancion ++;
                    break;
                case 2:
                    break;
                case 3:
                    if (posCancion == 0){
                        cout << "Esta es la primer cancion, no se puede reproducir la anterior, reproduciendo siguiente cancion";
                        posCancion ++;
                    }
                    else{
                        posCancion --;
                    }
                    break;
                default:
                    cout << "Esta no es una opcion valida, reproduciendo siguiente cancion";
                    posCancion ++;
                }
                cout << endl << endl;
            }
        }
        break;
    }

        //REPRODUCCION DE LA LISTA EN ORDEN ALEATORIO
    case 2:{
        cout <<"-------------------------------------" << endl
             <<"Reproduciendo en orden aleatorio ."<< endl
             <<"-------------------------------------" << endl;

        int posCancion = 0;
        srand(time(nullptr));

        //REPRODUCCION SI TIENE MENOS DE 7 CANCIONES
        if(numeroCanciones < 7){
            int* posiciones = new int[numeroCanciones];
            for(int i=0; i<numeroCanciones; i++){
                posiciones[i] = rand() % numeroCanciones;
            }
            while(posCancion < numeroCanciones){
                lista[posiciones[posCancion]] ->mostrarInfo();
                this_thread::sleep_for(chrono::seconds(3));

                //Siguiente accion
                cout << "que deseas hacer ahora?" << endl
                     << "1.reproducir siguiente cancion" << endl
                     << "2.reproducir de nuevo la cancion actual" << endl
                     << "3.reproducir la cancion anterior" << endl;
                cin >> siguienteAccion;
                switch (siguienteAccion) {
                case 1:
                    posCancion ++;
                    break;
                case 2:
                    break;
                case 3:
                    if (posCancion == 0){
                        cout << "Esta es la primer cancion, no se puede reproducir la anterior, reproduciendo siguiente cancion";
                        posCancion ++;
                    }
                    else{
                        posCancion --;
                    }
                    break;
                default:
                    cout << "Esta no es una opcion valida, reproduciendo siguiente cancion";
                    posCancion ++;
                }
                cout << endl << endl;
            }
            delete[] posiciones;
        }

        //REPRODUCCION SI TIENE 7 o MAS CANCIONES EN LA LISTA
        else{
            int posiciones[7];
            for(int i=0; i<7; i++){
                posiciones[i] = rand() % numeroCanciones;
            }
            while(posCancion < numeroCanciones){
                lista[posiciones[posCancion]] ->mostrarInfo();
                this_thread::sleep_for(chrono::seconds(3));

                //Siguiente accion
                cout << "que deseas hacer ahora?" << endl
                     << "1.reproducir siguiente cancion" << endl
                     << "2.reproducir de nuevo la cancion actual" << endl
                     << "3.reproducir la cancion anterior" << endl;
                cin >> siguienteAccion;
                switch (siguienteAccion) {
                case 1:
                    posCancion ++;
                    break;
                case 2:
                    break;
                case 3:
                    if (posCancion == 0){
                        cout << "Esta es la primer cancion, no se puede reproducir la anterior, reproduciendo siguiente cancion";
                        posCancion ++;
                    }
                    else{
                        posCancion --;
                    }
                    break;
                default:
                    cout << "Esta no es una opcion valida, reproduciendo siguiente cancion";
                    posCancion ++;
                }
                cout << endl << endl;
            }
        }
        break;
    }
    }
}

void listaFavoritos::setLista(Cancion** _lista, int _numeroCanciones) {
    lista = _lista;
    numeroCanciones = _numeroCanciones;
}

void listaFavoritos::setNombreDueño(const string& ND) {
    nombreDueño = ND;
}

void listaFavoritos::setNumeroCanciones(int NC) {
    numeroCanciones = NC;
}

Cancion** listaFavoritos::getLista() const {
    return lista;
}

string listaFavoritos::getNombreDueño() const {
    return nombreDueño;
}

int listaFavoritos::getNumeroCanciones() const {
    return numeroCanciones;
}
