#ifndef USUARIO_H
#define USUARIO_H

#include "listafavoritos.h"  // ✅CORREGIDO: usa comillas en lugar de brackets
#include <iostream>
#include <string>
using namespace std;

class Usuario
{
public:
    //CONSTRUCTORES
    Usuario(string,string,string,string,string,bool,int, listaFavoritos);
    Usuario();

    //setters
    void setNickname(const string& n);
    void setMembresia(bool m);
    void setCiudad(const string& c);
    void setPais(const string& p);
    void setFecha(const string& f);
    void setPassword(const string& c);
    void setListaFavoritos(const string& n, string* l, int nC);
    void setContadorPublicidad(int CP);

    //getters
    string getNickname() const;
    bool getMembresia() const;
    string getCiudad() const;
    string getPais() const;
    string getFecha() const;
    string getPassword() const;
    listaFavoritos getListaFavoritos() const;

    void mostrarInfo();

    //para la creacion de los arreglos dinamicos
    static Usuario* cargarUsuarios(const string& rutaArchivoU, const string& rutaArchivoLF, int& cantidad);

private:
    string nickname;
    string password;
    string ciudad;
    string pais;
    string fechaRegistro;
    bool membresia;
    int contadorPublicidad;
    listaFavoritos MiLista;

};

#endif // USUARIO_H
