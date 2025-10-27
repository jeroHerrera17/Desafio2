#include "publicidad.h"
#include <fstream>
#include <iostream>
#include <sstream>
#include <ctime>
#include <cstdlib>

using namespace std;

publicidad::publicidad() {
    categoria = ' ';
    mensaje = "";
}

/*
 *-----------
 * setters
 *-----------
*/
void publicidad::setCategoria(const char& c) {categoria = c;}
void publicidad::setMensaje(const string& m) {mensaje = m; }

/*
 *----------
 * getters
 *----------
*/
char publicidad::getCategoria() {return categoria; }
string publicidad::getMensaje() {return mensaje; }

/*
 * FUNCION necesaria para mostrar la publicidad a los usuarios estandar
*/
void publicidad::verPublicidad(publicidad* mensajesPublicitarios, const int cantidad){
    //primero ponemos la semilla en el tiempo actual para tratar de hacer un numero verdaderamente aleatorio( que no se repita cada vez q se ejecute esta funcion)
    srand(time(0));

    /*Generar un numero aleatorio entre 1 y 6
    *si es 1, se mostrara un mensaje tipo C
    *si es 2 o 3, se mostrara un mensaje tipo B
    *si es 4, 5 o 6, se mostrara un mensaje tipo A
    */

    int numeroAzar = (rand() % 6) + 1;
    bool mensajeMostrado = false;

    string mensaje;
    char categoria;
    int posicion;

    while (mensajeMostrado == false) {

        /*A su vez generamos un numero(posicion) aleatoria del arreglo en el que se encuentran almacenado
         *  los objetos publicidad, es decir, los mensajes publicitarios
        */
        int posicionAzar = rand() % cantidad;

        /*Realizamos la verificacion de q la categoria del mensaje revisado actualmente corresponda con el seleccionado
         * en el rand() de numeroAzar, en caso contrario continuamos y revisamos con otro anuncio publicitario
        */
        if(numeroAzar == 1){
            if(mensajesPublicitarios[posicionAzar].getCategoria() == 'C'){

                mensaje = mensajesPublicitarios[posicionAzar].getMensaje();
                categoria = mensajesPublicitarios[posicionAzar].getCategoria();
                mensajeMostrado = true;
                posicion = posicionAzar;
            }
        }
        else if(numeroAzar == 2 || numeroAzar == 3){
            if(mensajesPublicitarios[posicionAzar].getCategoria() == 'B'){

                mensaje = mensajesPublicitarios[posicionAzar].getMensaje();
                categoria = mensajesPublicitarios[posicionAzar].getCategoria();
                mensajeMostrado = true;
                posicion = posicionAzar;
            }
        }
        else if(numeroAzar > 3){
            if(mensajesPublicitarios[posicionAzar].getCategoria() == 'A'){

                mensaje = mensajesPublicitarios[posicionAzar].getMensaje();
                categoria = mensajesPublicitarios[posicionAzar].getCategoria();
                mensajeMostrado = true;
                posicion = posicionAzar;
            }
        }
    }

    //Y para finalizar, mostramos el mensaje seleccionado junto con su categoria
    cout<<"\n\n\n\n\n";
    cout << mensajesPublicitarios[posicion].getMensaje() << endl;
    cout << "El mensaje publicitario tiene categoria: " << mensajesPublicitarios[posicion].getCategoria() << endl;
}
publicidad* publicidad::cargarPublicidad(const string& rutaArchivo, int& cantidad){
    ifstream archivo(rutaArchivo);
    if(!archivo.is_open()){
        cerr << "Error, no se pudo abrir el archivo " << rutaArchivo << endl;
        cantidad = 0;
        return nullptr;
    }

    cout << "Cargando publicidad desde: " << rutaArchivo << "...\n";

    cantidad = 0;
    string linea;
    while (getline(archivo, linea)) {
        if (!linea.empty()) {
            cantidad++;
        }
    }

    if(cantidad == 0){
        cout << "No se encontraron mensajes publicitarios en el archivo.\n";
        archivo.close();
        return nullptr;
    }

    publicidad* mensajesPublicitarios = new publicidad[cantidad];
    archivo.clear();
    archivo.seekg(0);
    int i = 0;

    while(getline(archivo, linea) && i < cantidad){
        if (linea.empty()) continue;

        stringstream ss(linea);
        string categoriaStr, mensajeStr;
        try{
            getline(ss, categoriaStr, ',');
            getline(ss, mensajeStr);

            if(mensajeStr.empty()) throw "El mensaje publicitario esta vacio";
            char categoriaChar = categoriaStr[0];

            mensajesPublicitarios[i].setCategoria(categoriaChar);
            mensajesPublicitarios[i].setMensaje(mensajeStr);

            // Mensaje de confirmación para cada mensaje cargado


            i++;
        }catch(...){
            cerr << "Error al leer la linea: " << linea << endl;
        }
    }

    archivo.close();
    cout << "Carga completada. Total de mensajes publicitarios: " << i << "\n";
    cantidad = i;
    return mensajesPublicitarios;
}
