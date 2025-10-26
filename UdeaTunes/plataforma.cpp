#include "plataforma.h"
#include <fstream>
#include <sstream>
#include <iostream>
#include <string>
#include <thread>
#include <chrono>
#include <cstdlib>
using namespace std;

// ============================
// Constructor y Destructor
// ============================
Plataforma::Plataforma()
    : artistas(nullptr), albumes(nullptr), canciones(nullptr),
    cantArtistas(0), cantAlbumes(0), cantCanciones(0) {}

Plataforma::~Plataforma() {
    liberarMemoria();
}

// ============================
// Función auxiliar para limpiar texto
// ============================
string limpiar(const string& texto) {
    size_t inicio = 0;
    size_t fin = texto.size();

    while (inicio < fin && (texto[inicio] == ' ' || texto[inicio] == '\n' ||
                            texto[inicio] == '\r' || texto[inicio] == '\t')) {
        inicio++;
    }

    while (fin > inicio && (texto[fin - 1] == ' ' || texto[fin - 1] == '\n' ||
                            texto[fin - 1] == '\r' || texto[fin - 1] == '\t')) {
        fin--;
    }

    return texto.substr(inicio, fin - inicio);
}

// ============================
// Inicio de sesión
// ============================
bool Plataforma::iniciarSesion(const string& archivo, const string& nickname,
                               const string& clave, Usuario& usuario) {
    ifstream file(archivo);
    if (!file.is_open()) {
        cerr << "Error: no se pudo abrir el archivo " << archivo << endl;
        return false;
    }

    string linea;
    while (getline(file, linea)) {
        stringstream ss(linea);
        string nick, membresiaStr, ciudad, pais, fecha, claveArchivo;

        getline(ss, nick, ',');
        getline(ss, membresiaStr, ',');
        getline(ss, ciudad, ',');
        getline(ss, pais, ',');
        getline(ss, fecha, ',');
        getline(ss, claveArchivo);

        nick = limpiar(nick);
        claveArchivo = limpiar(claveArchivo);

        if (nick == nickname && claveArchivo == clave) {
            usuario.setNickname(nick);
            usuario.setMembresia(stoi(membresiaStr));
            usuario.setCiudad(ciudad);
            usuario.setPais(pais);
            usuario.setFecha(fecha);
            usuario.setPassword(claveArchivo);
            file.close();
            return true;
        }
    }

    file.close();
    return false;
}

// ============================
// Cargar datos en memoria
// ============================
bool Plataforma::cargarDatos(const string& rutaArtistas,
                             const string& rutaAlbumes,
                             const string& rutaCanciones) {
    artistas = Artista::cargarTodos(rutaArtistas, cantArtistas);
    canciones = Cancion::cargarCanciones(rutaCanciones, cantCanciones);
    albumes = Album::cargarTodos(rutaAlbumes, cantAlbumes, canciones, cantCanciones);

    if (!artistas || !albumes || !canciones) {
        cerr << "Error al cargar archivos." << endl;
        return false;
    }

    cout << "Datos cargados correctamente." << endl;
    return true;
}

// ============================
// Liberar memoria
// ============================
void Plataforma::liberarMemoria() {
    delete[] artistas;
    delete[] albumes;
    delete[] canciones;
    artistas = nullptr;
    albumes = nullptr;
    canciones = nullptr;
}

// ============================
// Pausar consola
// ============================
void Plataforma::pausar() {
    cout << "\nPresione ENTER para continuar...";
    cin.ignore(10000, '\n');
    cin.get();
}
