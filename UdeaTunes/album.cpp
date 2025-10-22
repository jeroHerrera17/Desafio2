#include "album.h"
#include <iostream>
#include <fstream>
#include <sstream>
#include <thread>
#include <chrono>
#include <cstdlib>
#include <ctime>

using namespace std;
void Album::reproducirAleatorio(Cancion* canciones, int totalCanciones, int codigoAlbum) {
    cout << "\n=============================\n";
    cout << "  REPRODUCCION ALEATORIA\n";
    cout << "=============================\n";
    cout << "Album: " << nombre << "\n";
    cout << "Genero: " << genero << "\n";
    cout << "-----------------------------\n";

    // Convertir el codigo del album a string para comparar prefijos
    string codigoAlbumStr = to_string(codigoAlbum);

    // Guardar los indices de las canciones que pertenecen al album
    int* indices = new int[totalCanciones];
    int conteo = 0;
    for (int i = 0; i < totalCanciones; i++) {
        string idCancionStr = to_string(canciones[i].getIdAlbum());
        if (idCancionStr.substr(0, 7) == codigoAlbumStr) {
            indices[conteo++] = i;
        }
    }

    if (conteo == 0) {
        cout << "No hay canciones para reproducir.\n";
        delete[] indices;
        return;
    }


    srand(time(nullptr));
    for (int i = conteo - 1; i > 0; i--) {
        int j = rand() % (i + 1);
        int temp = indices[i];
        indices[i] = indices[j];
        indices[j] = temp;
    }

    // Reproduccion simulada usando los indices mezclados
    for (int i = 0; i < conteo; i++) {
        Cancion& cancion = canciones[indices[i]];
        cout << "\nReproduciendo (" << i + 1 << "/" << conteo << "):\n";
        cancion.mostrarResumen();
        this_thread::sleep_for(chrono::seconds(3));
    }

    cout << "\n--- Fin del album aleatorio ---\n";

    delete[] indices;
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
// Getters
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
static void trimString(string& str) {
    size_t start = str.find_first_not_of(" \t\r\n");
    size_t end = str.find_last_not_of(" \t\r\n");
    if (start == string::npos || end == string::npos) {
        str.clear();
    } else {
        str = str.substr(start, end - start + 1);
    }
}

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

        trimString(c1); trimString(c2); trimString(c3); trimString(c4);
        trimString(c5); trimString(c6); trimString(c7); trimString(c8); trimString(c9);

        int idArtista = 0;
        try {
            idArtista = stoi(c2);
        } catch (...) {
            continue;
        }

        if (idArtista == idArtistaBuscado) {
            cout << c1 << "," << c3 << "," << c4 << "," << c5 << ","
                 << c6 << "," << c7 << "," << c8 << "," << c9 << endl;
            encontrado = true;
        }

        // Contador de iteraciones
        //medicionRecursos.agregarIteraciones();
        //medicionRecursos.agregarIteraciones();
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

    cantidad = 0;
    string linea;
    while (getline(file, linea)) {
        if (!linea.empty()) cantidad++;
    }

    if (cantidad == 0) {
        file.close();
        return nullptr;
    }

    Album* albumes = new Album[cantidad];

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

        trimString(c1); trimString(c2); trimString(c3); trimString(c4);
        trimString(c5); trimString(c6); trimString(c7); trimString(c8); trimString(c9);

        try {
            albumes[i] = Album(
                stoi(c1),
                stoi(c2),
                c3,
                c4,
                c5,
                stof(c6),
                c7,
                c8,
                stoi(c9)
                );
            i++;
        } catch (...) {
            continue;
        }

        // Contador de iteraciones
        //medicionRecursos.agregarIteraciones();
        //medicionRecursos.agregarIteraciones();
    }

    file.close();
    cantidad = i;
    return albumes;
}
