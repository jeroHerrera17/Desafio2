#include "album.h"
#include <iostream>
#include <fstream>
#include <sstream>
#include <iomanip>
using namespace std;

Album::Album() {}

Album::Album(int codigo, const string& nombre, const string& genero,
             const string& fechaLanzamiento, float duracionTotal,
             const string& sello, const string& portada, int puntuacion)
    : codigo(codigo), nombre(nombre), genero(genero),
    fechaLanzamiento(fechaLanzamiento), duracionTotal(duracionTotal),
    sello(sello), portada(portada), puntuacion(puntuacion) {}

int Album::getCodigo() const { return codigo; }
string Album::getNombre() const { return nombre; }
string Album::getGenero() const { return genero; }
string Album::getFechaLanzamiento() const { return fechaLanzamiento; }
float Album::getDuracionTotal() const { return duracionTotal; }
string Album::getSello() const { return sello; }
string Album::getPortada() const { return portada; }
int Album::getPuntuacion() const { return puntuacion; }

void Album::mostrarInfo() const {
    cout << left << setw(10) << codigo
         << setw(20) << nombre
         << setw(15) << genero
         << setw(15) << fechaLanzamiento
         << setw(10) << duracionTotal
         << setw(20) << sello
         << setw(25) << portada
         << setw(5)  << puntuacion << endl;
}

void Album::cargarPorArtista(const string& rutaArchivo, int idArtistaBuscado) {
    ifstream file(rutaArchivo);
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
        if (linea.empty()) continue; // Ignorar líneas vacías

        stringstream ss(linea);
        string campo1, campo2, campo3, campo4, campo5, campo6, campo7, campo8, campo9;

        // Leer 9 campos separados por coma
        if (!getline(ss, campo1, ',')) continue;
        if (!getline(ss, campo2, ',')) continue;
        if (!getline(ss, campo3, ',')) continue;
        if (!getline(ss, campo4, ',')) continue;
        if (!getline(ss, campo5, ',')) continue;
        if (!getline(ss, campo6, ',')) continue;
        if (!getline(ss, campo7, ',')) continue;
        if (!getline(ss, campo8, ',')) continue;
        if (!getline(ss, campo9, ',')) continue;

        // Limpiar espacios
        auto trim = [](string& s) {
            s.erase(0, s.find_first_not_of(" \t"));
            s.erase(s.find_last_not_of(" \t") + 1);
        };
        trim(campo1); trim(campo2); trim(campo3); trim(campo4);
        trim(campo5); trim(campo6); trim(campo7); trim(campo8); trim(campo9);

        int codigo = 0, idArtista = 0;
        try {
            codigo = stoi(campo1);
            idArtista = stoi(campo2);
        } catch (...) {
            cerr << "Advertencia: línea inválida -> " << linea << endl;
            continue;
        }

        // Mostrar solo si coincide el ID del artista
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
