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
    // Constructores y destructor
    Creditos();
    Creditos(string* prod, int nProd, string* mus, int nMus, string* comp, int nComp);
    Creditos(const Creditos& other);
    Creditos& operator=(const Creditos& other);
    ~Creditos();

    // Mostrar créditos
    void mostrar() const;

    // 🔹 Nueva función estática: cargar créditos según ID de canción
    static Creditos desdeArchivo(long idCancion);

    // (Opcional) getters si los necesitas en el futuro
    int getNumProductores() const { return nProductores; }
    int getNumMusicos() const { return nMusicos; }
    int getNumCompositores() const { return nCompositores; }
    string* getProductores() const { return productores; }
    string* getMusicos() const { return musicos; }
    string* getCompositores() const { return compositores; }
};

#endif // CREDITOS_H
