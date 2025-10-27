#include "creditos.h"
#include <iostream>
#include <fstream>
#include <sstream>
#include <cctype>
using namespace std;

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
    cout << "\n\t[Creditos]" << endl;

    cout << "\tProductores:" << endl;
    for (int i = 0; i < nProductores; i++) cout << "\t  - " << productores[i] << endl;

    cout << "\tMusicos:" << endl;
    for (int i = 0; i < nMusicos; i++) cout << "\t  - " << musicos[i] << endl;

    cout << "\tCompositores:" << endl;
    for (int i = 0; i < nCompositores; i++) cout << "\t  - " << compositores[i] << endl;
}

// ------------------------------------------------------------
//  Carga los créditos desde el archivo fijo "../../Datos/creditosCanciones.txt"
// Formato esperado:
// idCancion,Productores:Ana-Pérez-AB1234CD56|Carlos-Ríos-ZX9876YU43;Musicos:...;Compositores:...
// ------------------------------------------------------------
Creditos Creditos::desdeArchivo(long idCancion) {
    Creditos resultado;
    ifstream file("../../Datos/creditosCanciones.txt");
    if (!file.is_open()) {
        cerr << "Error: no se pudo abrir el archivo de créditos." << endl;
        return resultado;
    }

    string linea;
    while (getline(file, linea)) {
        if (linea.empty()) continue;

        stringstream ss(linea);
        string idStr;
        getline(ss, idStr, ',');
        long id = stol(idStr);

        if (id != idCancion) continue;

        // --- Se encontró la línea correspondiente ---
        string resto;
        getline(ss, resto);

        // arreglos temporales
        string prodTemp[20], musTemp[20], compTemp[20];
        int nP = 0, nM = 0, nC = 0;

        stringstream partes(resto);
        string seccion;

        auto trim = [](string s) {
            size_t start = s.find_first_not_of(" \t");
            size_t end = s.find_last_not_of(" \t");
            return (start == string::npos) ? string("") : s.substr(start, end - start + 1);
        };

        while (getline(partes, seccion, ';')) {
            seccion = trim(seccion);
            size_t pos = seccion.find(':');
            if (pos == string::npos) continue;

            string rol = trim(seccion.substr(0, pos));
            string datos = trim(seccion.substr(pos + 1));

            // normalizar rol
            for (auto& c : rol) c = tolower((unsigned char)c);

            stringstream nombres(datos);
            string persona;
            while (getline(nombres, persona, '|')) {
                persona = trim(persona);
                if (persona.empty()) continue;

                // cada persona se define como Nombre-Apellido-Codigo
                stringstream campos(persona);
                string nombre, apellido, codigo;
                getline(campos, nombre, '-');
                getline(campos, apellido, '-');
                getline(campos, codigo, '-');

                string textoFinal = nombre + " " + apellido + " (" + codigo + ")";
                if (rol == "productores" && nP < 20)
                    prodTemp[nP++] = textoFinal;
                else if (rol == "musicos" && nM < 20)
                    musTemp[nM++] = textoFinal;
                else if (rol == "compositores" && nC < 20)
                    compTemp[nC++] = textoFinal;
            }
        }

        resultado = Creditos(prodTemp, nP, musTemp, nM, compTemp, nC);
        break;
    }

    file.close();
    return resultado;
}
