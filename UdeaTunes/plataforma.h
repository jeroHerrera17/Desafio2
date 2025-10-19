#ifndef PLATAFORMA_H
#define PLATAFORMA_H

#include <string>
#include <iostream>
#include <fstream>
#include <sstream>


using namespace std;

class Plataforma {
public:
    Plataforma();
    ~Plataforma();

    bool iniciarSesion(const string& archivo, const string& nickname, const string& clave, Usuario& usuario);
};

#endif
