#include "creditos.h"
#include <iostream>
#include <fstream>
#include <sstream>
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
    cout << "\n[Creditos]" << endl;

    cout << "Productores:" << endl;
    for (int i = 0; i < nProductores; i++)
        cout << "  - " << productores[i] << endl;

    cout << "Musicos:" << endl;
    for (int i = 0; i < nMusicos; i++)
        cout << "  - " << musicos[i] << endl;

    cout << "Compositores:" << endl;
    for (int i = 0; i < nCompositores; i++)
        cout << "  - " << compositores[i] << endl;
}

// ------------------------------------------------------------
// Carga los creditos desde el archivo fijo "../../Datos/creditosCanciones.txt"
// Formato esperado:
// idCancion,Productores:Ana-Perez-AB1234|Carlos-Rios-ZX9876;Musicos:...;Compositores:...
// ------------------------------------------------------------
Creditos Creditos::desdeArchivo(long idCancion) {
    // variables de monitoreo de recursos
    size_t memoriaUsada = 0;
    int iteraciones = 0;

    Creditos resultado;
    ifstream file("../../Datos/creditosCanciones.txt");
    if (!file.is_open()) {
        cerr << "Error: no se pudo abrir el archivo de creditos." << endl;
        return resultado;
    }

    string linea;
    while (getline(file, linea)) {
        iteraciones++;
        if (linea.empty()) continue;

        stringstream ss(linea);
        string idStr;
        getline(ss, idStr, ',');
        long id = stol(idStr);
        if (id != idCancion) continue;

        string resto;
        getline(ss, resto);

        string prodTemp[20], musTemp[20], compTemp[20];
        int nP = 0, nM = 0, nC = 0;

        stringstream partes(resto);
        string seccion;

        // funcion para recortar espacios
        auto trim = [](string s) {
            size_t start = 0;
            size_t end = s.size();
            while (start < end && (s[start] == ' ' || s[start] == '\t')) start++;
            while (end > start && (s[end - 1] == ' ' || s[end - 1] == '\t')) end--;
            return s.substr(start, end - start);
        };

        // funcion para pasar a minusculas sin usar <cctype>
        auto toLowerString = [](string s) {
            for (char& c : s) {
                if (c >= 'A' && c <= 'Z') c = c - 'A' + 'a';
            }
            return s;
        };

        while (getline(partes, seccion, ';')) {
            seccion = trim(seccion);
            size_t pos = seccion.find(':');
            if (pos == string::npos) continue;

            string rol = trim(seccion.substr(0, pos));
            string datos = trim(seccion.substr(pos + 1));
            rol = toLowerString(rol);

            stringstream nombres(datos);
            string persona;

            while (getline(nombres, persona, '|')) {
                persona = trim(persona);
                if (persona.empty()) continue;

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

                memoriaUsada += textoFinal.size();
            }
        }

        resultado = Creditos(prodTemp, nP, musTemp, nM, compTemp, nC);
        break;
    }

    file.close();

    // mostrar resumen de recursos al final
    cout << "Creditos cargados correctamente." << endl;
    cout << "Uso de recursos en Creditos::desdeArchivo:" << endl;
    cout << "  - Memoria usada: " << memoriaUsada << " bytes aprox." << endl;
    cout << "  - Iteraciones realizadas: " << iteraciones << endl;

    return resultado;
}
