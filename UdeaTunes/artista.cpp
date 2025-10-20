#include "artista.h"
#include <iostream>
#include <fstream>
#include <sstream>
using namespace std;

// ============================
// Constructores
// ============================
Artista::Artista() : codigo(0), edad(0), seguidores(0), posicion(0) {}

Artista::Artista(int codigo, const string& nombre, int edad, const string& pais, int seguidores, int posicion)
    : codigo(codigo), nombre(nombre), edad(edad), pais(pais), seguidores(seguidores), posicion(posicion) {}

// ============================
// Getters
// ============================
int Artista::getCodigo() const { return codigo; }
string Artista::getNombre() const { return nombre; }
int Artista::getEdad() const { return edad; }
string Artista::getPais() const { return pais; }
int Artista::getSeguidores() const { return seguidores; }
int Artista::getPosicion() const { return posicion; }

// ============================
// Setters
// ============================
void Artista::setCodigo(int codigo) { this->codigo = codigo; }
void Artista::setNombre(const string& nombre) { this->nombre = nombre; }
void Artista::setEdad(int edad) { this->edad = edad; }
void Artista::setPais(const string& pais) { this->pais = pais; }
void Artista::setSeguidores(int seguidores) { this->seguidores = seguidores; }
void Artista::setPosicion(int posicion) { this->posicion = posicion; }

// ============================
// Mostrar información
// ============================
void Artista::mostrarInfo() const {
    cout << "---------------------------------------\n";
    cout << "Codigo: " << codigo << endl;
    cout << "Nombre: " << nombre << endl;
    cout << "Edad: " << edad << endl;
    cout << "Pais: " << pais << endl;
    cout << "Seguidores: " << seguidores << endl;
    cout << "Posicion: " << posicion << endl;
    cout << "---------------------------------------\n";
}

// ============================
// Cargar un artista por código
// ============================
bool Artista::cargarDesdeArchivo(const string& rutaArchivo, int codigoBuscado) {
    ifstream file(rutaArchivo);
    if (!file.is_open()) {
        cerr << "Error: no se pudo abrir el archivo " << rutaArchivo << endl;
        return false;
    }

    string linea;
    while (getline(file, linea)) {
        stringstream ss(linea);
        string codigoStr, nombreStr, edadStr, paisStr, seguidoresStr, posicionStr;

        getline(ss, codigoStr, ',');
        getline(ss, nombreStr, ',');
        getline(ss, edadStr, ',');
        getline(ss, paisStr, ',');
        getline(ss, seguidoresStr, ',');
        getline(ss, posicionStr, ',');

        int codigoLeido = stoi(codigoStr);
        if (codigoLeido == codigoBuscado) {
            codigo = codigoLeido;
            nombre = nombreStr;
            edad = stoi(edadStr);
            pais = paisStr;
            seguidores = stoi(seguidoresStr);
            posicion = stoi(posicionStr);

            file.close();
            return true;
        }
    }

    file.close();
    return false;
}

// ============================
// Mostrar todos los artistas del archivo
// ============================
void Artista::mostrarTodos(const string& rutaArchivo) {
    ifstream file(rutaArchivo);
    if (!file.is_open()) {
        cerr << "Error: no se pudo abrir el archivo " << rutaArchivo << endl;
        return;
    }

    string linea;
    cout << "\n========== LISTA DE ARTISTAS ==========\n";
    while (getline(file, linea)) {
        stringstream ss(linea);
        string codigoStr, nombreStr, edadStr, paisStr, seguidoresStr, posicionStr;

        getline(ss, codigoStr, ',');
        getline(ss, nombreStr, ',');
        getline(ss, edadStr, ',');
        getline(ss, paisStr, ',');
        getline(ss, seguidoresStr, ',');
        getline(ss, posicionStr, ',');

        cout << "[" << codigoStr << "] " << nombreStr << " (" << paisStr << "), Seguidores: " << seguidoresStr << endl;
    }
    cout << "=======================================\n";

    file.close();
}
