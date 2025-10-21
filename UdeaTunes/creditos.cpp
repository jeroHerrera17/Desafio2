#include "creditos.h"

Creditos::Creditos() {
    productores = nullptr;
    musicos = nullptr;
    compositores = nullptr;
    nProductores = nMusicos = nCompositores = 0;
}

Creditos::Creditos(string* productores, int nProd,
                   string* musicos, int nMus,
                   string* compositores, int nComp)
    : nProductores(nProd), nMusicos(nMus), nCompositores(nComp) {

    this->productores = new string[nProd];
    for (int i = 0; i < nProd; i++) this->productores[i] = productores[i];

    this->musicos = new string[nMus];
    for (int i = 0; i < nMus; i++) this->musicos[i] = musicos[i];

    this->compositores = new string[nComp];
    for (int i = 0; i < nComp; i++) this->compositores[i] = compositores[i];
}

Creditos::Creditos(const Creditos& other) {
    nProductores = other.nProductores;
    nMusicos = other.nMusicos;
    nCompositores = other.nCompositores;

    productores = new string[nProductores];
    for (int i = 0; i < nProductores; i++) productores[i] = other.productores[i];

    musicos = new string[nMusicos];
    for (int i = 0; i < nMusicos; i++) musicos[i] = other.musicos[i];

    compositores = new string[nCompositores];
    for (int i = 0; i < nCompositores; i++) compositores[i] = other.compositores[i];
}

Creditos& Creditos::operator=(const Creditos& other) {
    if (this != &other) {
        delete[] productores;
        delete[] musicos;
        delete[] compositores;

        nProductores = other.nProductores;
        nMusicos = other.nMusicos;
        nCompositores = other.nCompositores;

        productores = new string[nProductores];
        for (int i = 0; i < nProductores; i++) productores[i] = other.productores[i];

        musicos = new string[nMusicos];
        for (int i = 0; i < nMusicos; i++) musicos[i] = other.musicos[i];

        compositores = new string[nCompositores];
        for (int i = 0; i < nCompositores; i++) compositores[i] = other.compositores[i];
    }
    return *this;
}

Creditos::~Creditos() {
    delete[] productores;
    delete[] musicos;
    delete[] compositores;
}

void Creditos::mostrar() const {
    cout << "\n\t[CRÉDITOS]";
    cout << "\n\tProductores: ";
    for (int i = 0; i < nProductores; i++) cout << productores[i] << " ";
    cout << "\n\tMúsicos: ";
    for (int i = 0; i < nMusicos; i++) cout << musicos[i] << " ";
    cout << "\n\tCompositores: ";
    for (int i = 0; i < nCompositores; i++) cout << compositores[i] << " ";
    cout << "\n";
}
