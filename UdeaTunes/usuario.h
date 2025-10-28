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
    void setListaFavoritos(const string& n, Cancion** l, int nC);
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
    static Usuario* cargarUsuarios(const string& rutaArchivoU, const string& rutaArchivoLF, int& cantidad, Cancion* todas, int total);

    //para Seguir/ dejar de seguir la lista de favoritos de otro usuario premium
    void seguirListaFavoritos(const string& nombreDueño,Usuario* todos, int total);
    void dejarDeSeguirListaFavoritos();

    //Para actualizar los datos del archivo .txt de la lista de favoritos
    void actualizarArchivoTXT(const string& rutaArchivo, Usuario* usuarios, int cantidad);

private:
    string nickname;
    string password;
    string ciudad;
    string pais;
    string fechaRegistro;
    bool membresia;
    int contadorPublicidad;
    listaFavoritos MiLista;
    listaFavoritos* listaSeguida;

};

#endif // USUARIO_H
