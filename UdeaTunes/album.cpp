#include "album.h"
#include <iostream>
#include <fstream>
#include <sstream>
using namespace std;

// ----------------------------
// Función auxiliar para limpiar espacios
// ----------------------------
static void trimString(string& s) {
    if (s.empty()) return;
    size_t inicio = s.find_first_not_of(" \t");
    size_t fin = s.find_last_not_of(" \t");
    if (inicio == string::npos || fin == string::npos) {
        s.clear();
    } else {
        s = s.substr(inicio, fin - inicio + 1);
    }
}

// ----------------------------
// Constructores
// ----------------------------
Album::Album() : codigo(0), idArtista(0), duracionTotal(0.0f), puntuacion(0) {}

Album::Album(int codigo, int idArtista, const string& nombre, const string& genero,
             const string& fechaLanzamiento, float duracionTotal,
             const string& sello, const string& portada, int puntuacion)
    : codigo(codigo), idArtista(idArtista), nombre(nombre), genero(genero),
    fechaLanzamiento(fechaLanzamiento), duracionTotal(duracionTotal),
    sello(sello), portada(portada), puntuacion(puntuacion) {}

// ----------------------------
// Métodos get
// ----------------------------
int Album::getCodigo() const { return codigo; }
int Album::getIdArtista() const { return idArtista; }
string Album::getNombre() const { return nombre; }
string Album::getGenero() const { return genero; }
string Album::getFechaLanzamiento() const { return fechaLanzamiento; }
float Album::getDuracionTotal() const { return duracionTotal; }
string Album::getSello() const { return sello; }
string Album::getPortada() const { return portada; }
int Album::getPuntuacion() const { return puntuacion; }

// ----------------------------
// Mostrar información
// ----------------------------
void Album::mostrarInfo() const {
    cout << codigo << "\t"
         << nombre << "\t"
         << genero << "\t"
         << fechaLanzamiento << "\t"
         << duracionTotal << "\t"
         << sello << "\t"
         << portada << "\t"
         << puntuacion << endl;
}

void Album::mostrarResumen() const {
    cout << "[" << codigo << "] " << nombre << " - " << genero
         << " (" << fechaLanzamiento << ")" << endl;
}

// ----------------------------
// Cargar álbumes por artista
// ----------------------------
void Album::cargarPorArtista(const string& rutaArchivo, int idArtistaBuscado) {
    ifstream file(rutaArchivo.c_str());
    if (!file.is_open()) {
        cerr << "Error: no se pudo abrir el archivo de albumes.\n";
        return;
    }

    string linea;
    bool encontrado = false;
    cout << "=============================\n";
    cout << "   ALBUMES DEL ARTISTA\n";
    cout << "=============================\n";

    while (getline(file, linea)) {
        if (linea.empty()) continue;

        stringstream ss(linea);
        string campo1, campo2, campo3, campo4, campo5, campo6, campo7, campo8, campo9;

        if (!getline(ss, campo1, ',')) continue;
        if (!getline(ss, campo2, ',')) continue;
        if (!getline(ss, campo3, ',')) continue;
        if (!getline(ss, campo4, ',')) continue;
        if (!getline(ss, campo5, ',')) continue;
        if (!getline(ss, campo6, ',')) continue;
        if (!getline(ss, campo7, ',')) continue;
        if (!getline(ss, campo8, ',')) continue;
        if (!getline(ss, campo9, ',')) continue;

        trimString(campo1);
        trimString(campo2);
        trimString(campo3);
        trimString(campo4);
        trimString(campo5);
        trimString(campo6);
        trimString(campo7);
        trimString(campo8);
        trimString(campo9);

        int codigoNum = 0, idArtista = 0;
        try {
            codigoNum = stoi(campo1);
            idArtista = stoi(campo2);
        } catch (...) {
            cerr << "Advertencia: línea inválida -> " << linea << endl;
            continue;
        }

        if (idArtista == idArtistaBuscado) {
            cout << campo1 << "," << campo3 << "," << campo4 << "," << campo5 << ","
                 << campo6 << "," << campo7 << "," << campo8 << "," << campo9 << endl;
            encontrado = true;
        }
    }

    if (!encontrado) {
        cout << "No se encontraron albumes para este artista.\n";
    }

    file.close();
}

// ----------------------------
// Cargar todos los álbumes en memoria
// ----------------------------
Album* Album::cargarTodos(const string& rutaArchivo, int& cantidad) {
    ifstream file(rutaArchivo.c_str());
    if (!file.is_open()) {
        cerr << "Error: no se pudo abrir el archivo " << rutaArchivo << endl;
        cantidad = 0;
        return nullptr;
    }

    // Primera pasada: contar álbumes
    cantidad = 0;
    string linea;
    while (getline(file, linea)) {
        if (!linea.empty()) cantidad++;
    }

    if (cantidad == 0) {
        file.close();
        return nullptr;
    }

    // Crear array dinámico
    Album* albumes = new Album[cantidad];

    // Segunda pasada: cargar datos
    file.clear();
    file.seekg(0);
    int i = 0;

    while (getline(file, linea) && i < cantidad) {
        if (linea.empty()) continue;

        stringstream ss(linea);
        string c1, c2, c3, c4, c5, c6, c7, c8, c9;

        if (!getline(ss, c1, ',')) continue;
        if (!getline(ss, c2, ',')) continue;
        if (!getline(ss, c3, ',')) continue;
        if (!getline(ss, c4, ',')) continue;
        if (!getline(ss, c5, ',')) continue;
        if (!getline(ss, c6, ',')) continue;
        if (!getline(ss, c7, ',')) continue;
        if (!getline(ss, c8, ',')) continue;
        if (!getline(ss, c9, ',')) continue;

        trimString(c1);
        trimString(c2);
        trimString(c3);
        trimString(c4);
        trimString(c5);
        trimString(c6);
        trimString(c7);
        trimString(c8);
        trimString(c9);

        try {
            albumes[i] = Album(
                stoi(c1),           // codigo
                stoi(c2),           // idArtista
                c3,                 // nombre
                c4,                 // genero
                c5,                 // fechaLanzamiento
                stof(c6),           // duracionTotal
                c7,                 // sello
                c8,                 // portada
                stoi(c9)            // puntuacion
                );
            i++;
        } catch (...) {
            cerr << "Advertencia: error al parsear línea: " << linea << endl;
        }
    }

    file.close();
    cantidad = i;
    return albumes;
}
