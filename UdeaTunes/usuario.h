#ifndef USUARIO_H
#define USUARIO_H

#include <iostream>
#include <string>
using namespace std;
class Usuario
{
public:
    Usuario(string,string,string,string,string,int,int);
    agregarListaFavorito();
    seguirListaFavorito();
    ejecutarFavoritos();

    void setNickname(const string& n);
    void setMembresia(int m);
    void setCiudad(const string& c);
    void setPais(const string& p);
    void setFecha(const string& f);
    void setPassword(const string& c);

    string getNickname() ;
    int getMembresia();
    string getCiudad();
    string getPais();
    string getFecha();
    string getPassword();

    void mostrarInfo();

private:
    string nickname;
    string password;
    string ciudad;
    string pais;
    string fechaRegistro;
    int membresia;
    int contadorPublicidad;

};

#endif // USUARIO_H
