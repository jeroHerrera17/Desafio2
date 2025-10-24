#ifndef USUARIO_H
#define USUARIO_H

#include <listafavoritos.h>
#include <iostream>
#include <string>
using namespace std;
class Usuario
{
public:
    //CONSTRUCTORES
    Usuario(string,string,string,string,string,bool,int, listaFavoritos);
    Usuario();
    //void agregarListaFavorito();
    //void seguirListaFavorito();

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
    string getNickname() ;
    bool getMembresia();
    string getCiudad();
    string getPais();
    string getFecha();
    string getPassword();
    //listaFavoritos getListaFavoritos();

    void mostrarInfo();

    //para la creacion de los arreglos dinamicos
    //int contarUsuarios(const string& rutaArchivo);
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
