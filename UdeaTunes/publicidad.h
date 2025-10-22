#ifndef PUBLICIDAD_H
#define PUBLICIDAD_H

#include <string>

using namespace std;
class publicidad
{
public:
    publicidad();
    publicidad* cargarPublicidad(const string& rutaArchivo, int& cantidad);

    //FUNCION para mostrar la publicidad teniendo en cuenta la categoria del mensaje publicitario
    void verPublicidad(publicidad* mensajesPublicitarios, const int cantidad);

    //setters
    void setCategoria(const char&);
    void setMensaje(const string&);

    //getters
    char getCategoria();
    string getMensaje();

private:
    char categoria;
    string mensaje;
};

#endif // PUBLICIDAD_H
