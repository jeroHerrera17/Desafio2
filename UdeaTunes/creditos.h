#ifndef CREDITOS_H
#define CREDITOS_H

#include <string>
#include <iostream>
using namespace std;

class Creditos {
private:
    string* productores;
    string* musicos;
    string* compositores;
    int nProductores;
    int nMusicos;
    int nCompositores;

public:
    Creditos();
    Creditos(string* productores, int nProd,
             string* musicos, int nMus,
             string* compositores, int nComp);
    Creditos(const Creditos& other);
    Creditos& operator=(const Creditos& other);
    ~Creditos();

    void mostrar() const;
};

#endif
