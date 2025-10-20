#include "cancion.h"
#include <iostream>
#include <fstream>
#include <sstream>
using namespace std;

Cancion::Cancion() {}

Cancion::Cancion(int id, int idAlbum, const string& nombre, const string& duracion,
                 const string& rutaAudio128, const string& rutaAudio320,
                 const string& creditos, int reproducciones)
    : id(id), idAlbum(idAlbum), nombre(nombre), duracion(duracion),
    rutaAudio128(rutaAudio128), rutaAudio320(rutaAudio320),
    creditos(creditos), reproducciones(reproducciones) {}

void Cancion::mostrarInfo() const {
    cout << "\n🎵 " << nombre << "\n";
    cout << "\tID Cancion: " << id << "\n";
    cout << "\tID Album: " << idAlbum << "\n";
    cout << "\tDuracion: " << duracion << " segundos\n";
    cout << "\tAudio 128 kbps: " << rutaAudio128 << "\n";
    cout << "\tAudio 320 kbps: " << rutaAudio320 << "\n";
    cout << "\tCreditos: " << creditos << "\n";
    cout << "\tReproducciones: " << reproducciones << "\n";
    cout << "--------------------------------------------\n";
}

void Cancion::cargarPorAlbum(const string& rutaArchivo, int idAlbumBuscado) {
    ifstream file(rutaArchivo);
    if (!file.is_open()) {
        cerr << "Error: no se pudo abrir el archivo de canciones.\n";
        return;
    }

    string linea;
    bool encontrado = false;
    cout << "\n=== CANCIONES DEL ALBUM " << idAlbumBuscado << " ===\n";

    while (getline(file, linea)) {
        if (linea.empty()) continue;

        stringstream ss(linea);
        string temp;
        int id, idAlbum, reproducciones;
        string nombre, duracion, ruta128, ruta320, creditos;

        try {
            getline(ss, temp, ',');
            id = stoi(temp);

            getline(ss, temp, ',');
            idAlbum = stoi(temp);

            getline(ss, nombre, ',');
            getline(ss, duracion, ',');
            getline(ss, ruta128, ',');
            getline(ss, ruta320, ',');
            getline(ss, creditos, ',');
            getline(ss, temp, ',');
            reproducciones = stoi(temp);
        }
        catch (invalid_argument&) {
            cerr << "Advertencia: formato invalido -> " << linea << endl;
            continue;
        }

        if (idAlbum == idAlbumBuscado) {
            Cancion c(id, idAlbum, nombre, duracion, ruta128, ruta320, creditos, reproducciones);
            c.mostrarInfo();
            encontrado = true;
        }
    }

    if (!encontrado) {
        cout << "\nNo se encontraron canciones para este album.\n";
    }

    file.close();
}
