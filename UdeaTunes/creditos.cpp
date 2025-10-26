#include "creditos.h"

Creditos::Creditos() {
    productores = nullptr;
    musicos = nullptr;
    compositores = nullptr;
    nProductores = nMusicos = nCompositores = 0;
}

Creditos::Creditos(string* prod, int nProd, string* mus, int nMus, string* comp, int nComp) {
    nProductores = nProd;
    nMusicos = nMus;
    nCompositores = nComp;

    productores = (nProd > 0) ? new string[nProd] : nullptr;
    musicos = (nMus > 0) ? new string[nMus] : nullptr;
    compositores = (nComp > 0) ? new string[nComp] : nullptr;

    for (int i = 0; i < nProd; i++) productores[i] = prod[i];
    for (int i = 0; i < nMus; i++) musicos[i] = mus[i];
    for (int i = 0; i < nComp; i++) compositores[i] = comp[i];
}

Creditos::Creditos(const Creditos& other) {
    nProductores = other.nProductores;
    nMusicos = other.nMusicos;
    nCompositores = other.nCompositores;

    productores = (nProductores > 0) ? new string[nProductores] : nullptr;
    musicos = (nMusicos > 0) ? new string[nMusicos] : nullptr;
    compositores = (nCompositores > 0) ? new string[nCompositores] : nullptr;

    for (int i = 0; i < nProductores; i++) productores[i] = other.productores[i];
    for (int i = 0; i < nMusicos; i++) musicos[i] = other.musicos[i];
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

        productores = (nProductores > 0) ? new string[nProductores] : nullptr;
        musicos = (nMusicos > 0) ? new string[nMusicos] : nullptr;
        compositores = (nCompositores > 0) ? new string[nCompositores] : nullptr;

        for (int i = 0; i < nProductores; i++) productores[i] = other.productores[i];
        for (int i = 0; i < nMusicos; i++) musicos[i] = other.musicos[i];
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
    cout << "\n\t[Créditos]" << endl;

    cout << "\tProductores: ";
    for (int i = 0; i < nProductores; i++) cout << productores[i] << " ";
    cout << endl;

    cout << "\tMúsicos: ";
    for (int i = 0; i < nMusicos; i++) cout << musicos[i] << " ";
    cout << endl;

    cout << "\tCompositores: ";
    for (int i = 0; i < nCompositores; i++) cout << compositores[i] << " ";
    cout << endl;
}
