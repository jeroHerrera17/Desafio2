#include "usuario.h"
#include "listafavoritos.h"
#include <string>
#include <iostream>
#include <fstream>


using namespace std;

Usuario::Usuario(string _nickname, string _password, string _ciudad, string _pais,
                 string _fechaRegistro, bool _membresia, int _contadorPublicidad) {
    nickname = _nickname;
    password = _password;
    ciudad = _ciudad;
    pais = _pais;
    fechaRegistro = _fechaRegistro;
    membresia = _membresia;
    contadorPublicidad = _contadorPublicidad;

}

Usuario::Usuario() {
    contadorPublicidad = 0;
    membresia = false;
}

// =====================
// Setters
// =====================
void Usuario::setNickname(const string& n) { nickname = n; }
void Usuario::setMembresia(bool m) { membresia = m; }
void Usuario::setCiudad(const string& c) { ciudad = c; }
void Usuario::setPais(const string& p) { pais = p; }
void Usuario::setFecha(const string& f) { fechaRegistro = f; }
void Usuario::setPassword(const string& c) { password = c; }
//void Usuario::setListaFavoritos(const string& n, Cancion** l, int nC) {
//    MiLista = listaFavoritos(n, l, nC);
//}
void Usuario::setContadorPublicidad(int CP) { contadorPublicidad = CP; }

// =====================
// Getters
// =====================
string Usuario::getNickname() const { return nickname; }
bool Usuario::getMembresia() const { return membresia; }
string Usuario::getCiudad() const { return ciudad; }
string Usuario::getPais() const { return pais; }
string Usuario::getFecha() const { return fechaRegistro; }
string Usuario::getPassword() const { return password; }

// AGREGADO: Implementación del getter que faltaba
//listaFavoritos Usuario::getListaFavoritos() const {
//    return MiLista;
//}


/*
 *====================================
 * Dejar de seguir lista de favoritos
 *====================================
*/
//void Usuario::dejarDeSeguirListaFavoritos()  {listaSeguida = nullptr;}


// =====================
// Mostrar información
// =====================
void Usuario::mostrarInfo() {
    cout << "Usuario: " << nickname << endl;
    cout << "Membresia: " << (membresia ? "Premium" : "Estandar") << endl;
    cout << "Ciudad: " << ciudad << endl;
    cout << "Pais: " << pais << endl;
    cout << "Fecha de inscripcion: " << fechaRegistro << endl;
}


