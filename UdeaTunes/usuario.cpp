#include "usuario.h"
#include "listafavoritos.h"
#include <string>

Usuario::Usuario(string _nickname,string _password,string _ciudad,string _pais,string _fechaRegistro,bool _membresia,int _contadorPublicidad){
    nickname = _nickname;
    password = _password;
    ciudad = _ciudad;
    pais = _pais;
    fechaRegistro = _fechaRegistro;
    membresia = _membresia;
    contadorPublicidad = _contadorPublicidad;
    listaFavoritos::listaFavoritos
}

Usuario::Usuario(string _nickname,string _password,string _ciudad,string _pais,string _fechaRegistro,bool _membresia,int _contadorPublicidad, listaFavoritos _MiLista){

}

void Usuario::setNickname(const string& n) { nickname = n; }
void Usuario::setMembresia(bool m) { membresia = m; }
void Usuario::setCiudad(const string& c) { ciudad = c; }
void Usuario::setPais(const string& p) { pais = p; }
void Usuario::setFecha(const string& f) { fechaRegistro = f; }
void Usuario::setPassword(const string& c) { password = c; }
void Usuario::setListaFavoritos(listaFavoritos ML) {MiLista = ML; }
void Usuario::setContadorPublicidad(int CP) {contadorPublicidad = CP; }

string Usuario::getNickname()  { return nickname; }
bool Usuario::getMembresia()  { return membresia; }
string Usuario::getCiudad()  { return ciudad; }
string Usuario::getPais()  { return pais; }
string Usuario::getFecha()  { return fechaRegistro; }
string Usuario::getPassword()  { return password; }
listaFavoritos Usuario::getListaFavoritos()  { return listaFavoritos; }

void Usuario::mostrarInfo() {
    cout << "Usuario: " << nickname << endl;
    cout << "Membresia: " << (membresia ? "Premium" : "Estandar") << endl;
    cout << "Ciudad: " << ciudad << endl;
    cout << "Pais: " << pais << endl;
    cout << "Fecha de inscripcion: " << fechaRegistro << endl;
}
