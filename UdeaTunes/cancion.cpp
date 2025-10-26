#include "cancion.h"
#include <iostream>
#include <fstream>
#include <sstream>
using namespace std;

Cancion::Cancion() : idAlbum(0), nombre(""), duracion(0) {}

Cancion::Cancion(int idAlbum, const string& nombre, const string& ruta128,
                 const string& ruta320, int duracion, const Creditos& creditos)
    : idAlbum(idAlbum), nombre(nombre), duracion(duracion) {}

int Cancion::getIdAlbum() const { return idAlbum; }
string Cancion::getNombre() const { return nombre; }
int Cancion::getDuracion() const { return duracion; }

void Cancion::reproducir() const {
    cout << "♪ Reproduciendo: " << nombre << " (" << duracion << "s)" << endl;
}

void Cancion::mostrarInfo() const {
    cout << "\n" << nombre << "\n";
    cout << "\tID Álbum: " << idAlbum << "\n";
    cout << "\tDuración: " << duracion << " segundos\n";
}

void Cancion::mostrarResumen() const {
    cout << nombre << " (" << duracion << "s)\n";
}

// Función auxiliar para limpiar espacios
static string limpiarTexto(const string& str) {
    size_t inicio = 0;
    size_t fin = str.size();

    while (inicio < fin && isspace(static_cast<unsigned char>(str[inicio])))
        inicio++;

    while (fin > inicio && isspace(static_cast<unsigned char>(str[fin - 1])))
        fin--;

    return str.substr(inicio, fin - inicio);
}

static Creditos parsearCreditos(const string& texto) {
    string temp = texto;

    if (!temp.empty() && temp.front() == '(' && temp.back() == ')') {
        temp = temp.substr(1, temp.size() - 2);
    }

    string tokens[50];
    int total = 0;
    string token;
    stringstream ss(temp);

    while (getline(ss, token, ',')) {
        tokens[total++] = token;
    }

    int nProd = 0, nMus = 0, nComp = 0;
    for (int i = 0; i < total; i++) {
        if (tokens[i].find("PROD") != string::npos) nProd++;
        else if (tokens[i].find("MUSC") != string::npos) nMus++;
        else if (tokens[i].find("COMP") != string::npos) nComp++;
    }

    string* productores = new string[nProd];
    string* musicos = new string[nMus];
    string* compositores = new string[nComp];

    int ip = 0, im = 0, ic = 0;
    for (int i = 0; i < total; i++) {
        if (tokens[i].find("PROD") != string::npos) productores[ip++] = tokens[i];
        else if (tokens[i].find("MUSC") != string::npos) musicos[im++] = tokens[i];
        else if (tokens[i].find("COMP") != string::npos) compositores[ic++] = tokens[i];
    }

    Creditos cred(productores, nProd, musicos, nMus, compositores, nComp);

    delete[] productores;
    delete[] musicos;
    delete[] compositores;

    return cred;
}

// Método principal para cargar todas las canciones
Cancion* Cancion::cargarCanciones(const string& rutaArchivo, int& cantidad) {
    ifstream file(rutaArchivo);
    if (!file.is_open()) {
        cerr << "Error: no se pudo abrir el archivo de canciones: " << rutaArchivo << endl;
        cantidad = 0;
        return nullptr;
    }

    cantidad = 0;
    string linea;
    while (getline(file, linea))
        if (!linea.empty()) cantidad++;

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

        try {
            size_t posParentesis = linea.find_last_of('(');
            if (posParentesis == string::npos) {
                cerr << "Advertencia: Línea sin créditos: " << linea << endl;
                continue;
            }

            string parteAntes = linea.substr(0, posParentesis - 1);
            string creditosTxt = linea.substr(posParentesis);

            stringstream ss(parteAntes);
            string temp;
            string nombre, ruta128, ruta320;

            // Leer idAlbum
            if (!getline(ss, temp, ',')) {
                cerr << "Error leyendo idAlbum: " << linea << endl;
                continue;
            }
            temp = limpiarTexto(temp);
            if (temp.empty()) {
                cerr << "idAlbum vacío: " << linea << endl;
                continue;
            }
            int idAlbum = stoi(temp);

            // Leer nombre
            if (!getline(ss, nombre, ',')) {
                cerr << "Error leyendo nombre: " << linea << endl;
                continue;
            }
            nombre = limpiarTexto(nombre);

            // Leer rutas
            getline(ss, ruta128, ',');
            getline(ss, ruta320, ',');

            // Leer duración
            if (!getline(ss, temp, ',')) {
                cerr << "Error leyendo duración: " << linea << endl;
                continue;
            }
            temp = limpiarTexto(temp);
            if (temp.empty()) {
                cerr << "Duración vacía: " << linea << endl;
                continue;
            }
            int duracion = stoi(temp);

            Creditos cred = parsearCreditos(creditosTxt);
            canciones[i] = Cancion(idAlbum, nombre, ruta128, ruta320, duracion, cred);

            i++;

        } catch (const invalid_argument& e) {
            cerr << "Error de conversión en línea: " << linea << endl;
            cerr << "Detalle: " << e.what() << endl;
        } catch (const out_of_range& e) {
            cerr << "Número fuera de rango en línea: " << linea << endl;
            cerr << "Detalle: " << e.what() << endl;
        } catch (...) {
            cerr << "Error desconocido al parsear línea: " << linea << endl;
        }
    }

    file.close();
    cantidad = i;
    cout << "✓ Canciones cargadas: " << cantidad << endl;
    return canciones;
}

// Método alternativo para compatibilidad con código antiguo
void Cancion::cargarCanciones(const string& rutaArchivo,
                              int idAlbumBuscado,
                              Cancion**& cancionesAlbum,
                              int& cantidadAlbum,
                              Cancion* cancionesGlobal,
                              int totalGlobal) {
    cantidadAlbum = 0;

    // Primera pasada: contar canciones que pertenecen a este álbum
    for (int i = 0; i < totalGlobal; i++) {
        int idCancion = cancionesGlobal[i].getIdAlbum();
        int albumDeCancion = idCancion / 100; // Extraer código del álbum (100010101 / 100 = 1000101)

        if (albumDeCancion == idAlbumBuscado) {
            cantidadAlbum++;
        }
    }

    if (cantidadAlbum == 0) {
        cancionesAlbum = nullptr;
        return;
    }

    // Segunda pasada: crear array de punteros y asignar referencias
    cancionesAlbum = new Cancion*[cantidadAlbum];
    int idx = 0;

    for (int i = 0; i < totalGlobal; i++) {
        int idCancion = cancionesGlobal[i].getIdAlbum();
        int albumDeCancion = idCancion / 100;

        if (albumDeCancion == idAlbumBuscado) {
            cancionesAlbum[idx++] = &cancionesGlobal[i];
        }
    }
}
