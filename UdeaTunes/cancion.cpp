#include "cancion.h"
#include <iostream>
#include <fstream>
#include <sstream>
using namespace std;

// ----------------------------
// Constructor
// ----------------------------
Cancion::Cancion() : idAlbum(0), nombre(""), rutaAudio128(""),
    rutaAudio320(""), duracion(0) {}

Cancion::Cancion(int idAlbum, const string& nombre, const string& rutaAudio128,
                 const string& rutaAudio320, int duracion, const Creditos& creditos)
    : idAlbum(idAlbum), nombre(nombre), rutaAudio128(rutaAudio128),
    rutaAudio320(rutaAudio320), duracion(duracion), creditos(creditos) {}

// ----------------------------
// Mostrar información
// ----------------------------
void Cancion::mostrarInfo() const {
    cout << "\n " << nombre << "\n";
    cout << "\tID Album: " << idAlbum << "\n";
    cout << "\tDuracion: " << duracion << " segundos\n";
    cout << "\tAudio 128 kbps: " << rutaAudio128 << "\n";
    cout << "\tAudio 320 kbps: " << rutaAudio320 << "\n";
    creditos.mostrar();
    cout << "--------------------------------------------\n";
}

// ----------------------------
// Mostrar resumen
// ----------------------------
void Cancion::mostrarResumen() const {
    cout << " " << nombre << " (" << duracion << "s)\n"<<rutaAudio128;
}

// ----------------------------
// Función auxiliar para procesar los créditos
// ----------------------------
Creditos parsearCreditos(const string& texto) {
    string temp = texto;

    // Quitar paréntesis si existen
    if (!temp.empty() && temp.front() == '(' && temp.back() == ')') {
        temp = temp.substr(1, temp.size() - 2);
    }

    // Dividir por comas
    string tokens[50];
    int total = 0;
    string token;
    stringstream ss(temp);

    while (getline(ss, token, ',')) {
        tokens[total++] = token;
    }

    // Contar cada tipo de crédito
    int nProd = 0, nMus = 0, nComp = 0;
    for (int i = 0; i < total; i++) {
        if (tokens[i].find("PROD") != string::npos) nProd++;
        else if (tokens[i].find("MUSC") != string::npos) nMus++;
        else if (tokens[i].find("COMP") != string::npos) nComp++;
    }

    // Crear arrays dinámicos
    string* productores = new string[nProd];
    string* musicos = new string[nMus];
    string* compositores = new string[nComp];

    // Clasificar cada token
    int ip = 0, im = 0, ic = 0;
    for (int i = 0; i < total; i++) {
        if (tokens[i].find("PROD") != string::npos) productores[ip++] = tokens[i];
        else if (tokens[i].find("MUSC") != string::npos) musicos[im++] = tokens[i];
        else if (tokens[i].find("COMP") != string::npos) compositores[ic++] = tokens[i];
    }

    // Crear objeto Creditos
    Creditos cred(productores, nProd, musicos, nMus, compositores, nComp);

    // Liberar memoria
    delete[] productores;
    delete[] musicos;
    delete[] compositores;

    return cred;
}

// ----------------------------
// Cargar canciones por álbum
// ----------------------------
void Cancion::cargarPorAlbum(const string& rutaArchivo, int idAlbumBuscado) {
    ifstream file(rutaArchivo);
    if (!file.is_open()) {
        cerr << "Error: no se pudo abrir el archivo " << rutaArchivo << endl;
        return;
    }

    string linea;
    bool encontrado = false;
    cout << "\n=== CANCIONES DEL ALBUM " << idAlbumBuscado << " ===\n";

    while (getline(file, linea)) {
        if (linea.empty()) continue;

        size_t posParentesis = linea.find_last_of('(');
        if (posParentesis == string::npos) {
            cerr << "Formato incorrecto: no se encuentran creditos en la línea:\n" << linea << endl;
            continue;
        }

        string parteAntes = linea.substr(0, posParentesis - 1);
        string creditosTxt = linea.substr(posParentesis);

        stringstream ss(parteAntes);
        string idCompletoStr;
        int idAlbum, duracion;
        string nombre, ruta128, ruta320;

        try {
            getline(ss, idCompletoStr, ',');
            if (idCompletoStr.empty()) throw "ID de canción vacío";

            string albumIdStr = idCompletoStr.substr(0, 7);
            idAlbum = stoi(albumIdStr);

            getline(ss, nombre, ',');
            getline(ss, ruta128, ',');
            getline(ss, ruta320, ',');

            string duracionStr;
            getline(ss, duracionStr, ',');
            if (duracionStr.empty()) throw "Duración vacía";
            duracion = stoi(duracionStr);
        }
        catch (const char* msg) {
            cerr << "Error al parsear línea: " << msg << endl;
            continue;
        }
        catch (...) {
            cerr << "Error desconocido al parsear línea." << endl;
            continue;
        }

        if (idAlbum == idAlbumBuscado) {
            Creditos cred = parsearCreditos(creditosTxt);
            Cancion c(idAlbum, nombre, ruta128, ruta320, duracion, cred);
            c.mostrarInfo();
            encontrado = true;
        }
    }

    if (!encontrado) {
        cout << "\nNo se encontraron canciones para este álbum.\n";
    }

    file.close();
}

// ----------------------------
// Cargar todas las canciones
// ----------------------------
Cancion* Cancion::cargarTodas(const string& rutaArchivo, int& cantidad) {
    ifstream file(rutaArchivo);
    if (!file.is_open()) {
        cerr << "Error: no se pudo abrir el archivo de canciones.\n";
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

    Cancion* canciones = new Cancion[cantidad];

    file.clear();
    file.seekg(0);
    int i = 0;

    while (getline(file, linea) && i < cantidad) {
        if (linea.empty()) continue;

        size_t posParentesis = linea.find_last_of('(');
        if (posParentesis == string::npos) {
            cerr << "Advertencia: formato inválido (sin créditos) -> " << linea << endl;
            continue;
        }

        string parteAntes = linea.substr(0, posParentesis - 1);
        string creditosTxt = linea.substr(posParentesis);

        stringstream ss(parteAntes);
        string temp;
        int idAlbum, duracion;
        string nombre, ruta128, ruta320;

        try {
            getline(ss, temp, ',');
            if (temp.empty()) throw "ID vacío";
            idAlbum = stoi(temp);

            getline(ss, nombre, ',');
            getline(ss, ruta128, ',');
            getline(ss, ruta320, ',');
            getline(ss, temp, ',');
            if (temp.empty()) throw "Duración vacía";
            duracion = stoi(temp);

            Creditos cred = parsearCreditos(creditosTxt);
            canciones[i] = Cancion(idAlbum, nombre, ruta128, ruta320, duracion, cred);
            i++;
        }
        catch (const char* msg) {
            cerr << "Advertencia: formato inválido -> " << linea << "\n Motivo: " << msg << endl;
            continue;
        }
        catch (...) {
            cerr << "Advertencia: error desconocido -> " << linea << endl;
            continue;
        }
    }

    file.close();
    cantidad = i;
    return canciones;
}
void Cancion::reproducir() const {
    cout << "Reproduciendo: " << nombre
         << " (" << duracion << "s)" << endl;
}
