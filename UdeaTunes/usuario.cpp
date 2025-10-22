#include "usuario.h"
#include "listafavoritos.h"
#include <string>
#include <iostream>
#include <fstream>
#include <sstream>

using namespace std;
Usuario::Usuario(string _nickname,string _password,string _ciudad,string _pais,string _fechaRegistro,bool _membresia,int _contadorPublicidad, listaFavoritos _MiLista){
    nickname = _nickname;
    password = _password;
    ciudad = _ciudad;
    pais = _pais;
    fechaRegistro = _fechaRegistro;
    membresia = _membresia;
    contadorPublicidad = _contadorPublicidad;
    if(_membresia == false) {MiLista = listaFavoritos();}
    else {MiLista = _MiLista;}
}

Usuario::Usuario(){
    contadorPublicidad = 0;
    membresia = false;
    MiLista = listaFavoritos();
}

void Usuario::setNickname(const string& n) { nickname = n; }
void Usuario::setMembresia(bool m) { membresia = m; }
void Usuario::setCiudad(const string& c) { ciudad = c; }
void Usuario::setPais(const string& p) { pais = p; }
void Usuario::setFecha(const string& f) { fechaRegistro = f; }
void Usuario::setPassword(const string& c) { password = c; }
void Usuario::setListaFavoritos(const string& n, string* l, int nC){
    MiLista = listaFavoritos(n, l, nC);
}
void Usuario::setContadorPublicidad(int CP) {contadorPublicidad = CP; }

string Usuario::getNickname()  { return nickname; }
bool Usuario::getMembresia()  { return membresia; }
string Usuario::getCiudad()  { return ciudad; }
string Usuario::getPais()  { return pais; }
string Usuario::getFecha()  { return fechaRegistro; }
string Usuario::getPassword()  { return password; }
//listaFavoritos Usuario::getListaFavoritos()  { return listaFavoritos; }


/*
 * Funcion para mostrar los atributos de un usuario
*/
void Usuario::mostrarInfo() {
    cout << "Usuario: " << nickname << endl;
    cout << "Membresia: " << (membresia ? "Premium" : "Estandar") << endl;
    cout << "Ciudad: " << ciudad << endl;
    cout << "Pais: " << pais << endl;
    cout << "Fecha de inscripcion: " << fechaRegistro << endl;
}

/*
 *
 * Funcion para la carga de el arreglo con los distintos usuarios(objetos de esta clase)
 *
*/
Usuario* Usuario::cargarUsuarios(const string& rutaArchivoU, const string& rutaArchivoLF, int& cantidad){

    ifstream archivo(rutaArchivoU);
    if(!archivo.is_open()){
        cerr << "Error, no se pudo abrir el archivo " << rutaArchivoU << endl;
        cantidad = 0;
        return nullptr;
    }

    ifstream archivoLF(rutaArchivoLF);
    if(!archivo.is_open()){
        cerr << "error, no se pudo abrir el archivo " << rutaArchivoLF << endl;
        return nullptr;
    }

    //Primero contamos la cantidad de usuarios para el arreglo dinamico
    cantidad = 0;
    string linea, lineaLF;
    while (getline(archivo,linea)) {
        if (!linea.empty()){
            cantidad ++;
        }
    }
    if(cantidad == 0){
        archivo.close();
        return nullptr;
    }

    Usuario* usuarios = new Usuario[cantidad];

    //Segundo, cargamos los datos de cada usuario y creamos los objetos

    archivo.clear();
    archivo.seekg(0);
    int i = 0;

    while(getline(archivo,linea) && i < cantidad){

        stringstream ss(linea);
        string nicknameStr, membresiaStr, ciudadStr, paisStr, fechaRegistroStr, passwordStr;
        bool membresiaBool;
        string nicknameDueñoStr;

        getline(ss, nicknameStr, ',');
        getline(ss, membresiaStr, ',');
        getline(ss, ciudadStr, ',');
        getline(ss, paisStr, ',');
        getline(ss, fechaRegistroStr, ',');
        getline(ss, passwordStr, ',');

        if(membresiaStr == "1"){
            getline(archivoLF,lineaLF);
            stringstream sLF(lineaLF);
            string campo = "";
            int contador = 0, j = 0;
            while (getline(sLF, campo, ',')) {
                contador ++;
            }
            string* _lista = new string[contador-1];
            ss.clear();
            ss.seekg(0);
            getline(sLF,campo, ',');
            while (getline(sLF, campo, ',')) {
                _lista[j] = campo;
            }
            usuarios[i].setListaFavoritos(nicknameStr,_lista, contador);
        }
        try{
            usuarios[i].setNickname(nicknameStr);
            if(membresiaStr == "1") {membresiaBool = true;}
            else{membresiaBool = false; }
            usuarios[i].setMembresia(membresiaBool);
            usuarios[i].setCiudad(ciudadStr);
            usuarios[i].setPais(paisStr);
            usuarios[i].setFecha(fechaRegistroStr);
            usuarios[i].setPassword(passwordStr);
            usuarios[i].setListaFavoritos(nicknameStr,_lista, contador);
        } catch(...){
            cerr << "Error al cargar la linea " << linea << " y/o el objeto usuario";
        }
    }
    archivo.close();
    cantidad = i;
    return usuarios;
}
