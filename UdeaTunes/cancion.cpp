#include "cancion.h"
#include "creditos.h"
#include <iostream>
#include <fstream>
#include <sstream>
#include <cctype>
using namespace std;

Cancion::Cancion()
    : idAlbum(0), nombre(""), ruta128(""), ruta320(""), duracion(0), creditos() {}

Cancion::Cancion(int idAlbum, const string& nombre, const string& ruta128,
                 const string& ruta320, int duracion, const Creditos& creditos)
    : idAlbum(idAlbum), nombre(nombre), ruta128(ruta128),
    ruta320(ruta320), duracion(duracion), creditos(creditos) {}

// ──────────────────────────────────────────────
// Getters
// ──────────────────────────────────────────────
int Cancion::getIdAlbum() const { return idAlbum; }
string Cancion::getNombre() const { return nombre; }
string Cancion::getRuta128() const { return ruta128; }
string Cancion::getRuta320() const { return ruta320; }
int Cancion::getDuracion() const { return duracion; }
Creditos Cancion::getCreditos() const { return creditos; }

// ──────────────────────────────────────────────
// Mostrar información básica
// ──────────────────────────────────────────────
void Cancion::mostrarInfo() const {
    cout << "\n🎵 " << nombre << "\n";
    cout << "\tID Álbum: " << idAlbum << "\n";
    cout << "\tDuración: " << duracion << " segundos\n";
    cout << "\tRuta 128 kbps: " << ruta128 << "\n";
    cout << "\tRuta 320 kbps: " << ruta320 << "\n";
}

// ──────────────────────────────────────────────
// Mostrar resumen con créditos
// ──────────────────────────────────────────────
void Cancion::mostrarResumen() const {
    cout << "\n_________________________________________________" << endl;
    cout << " --Resumen de Canción-- " << endl;
    cout << "  - ID Álbum: " << idAlbum << endl;
    cout << "  - Nombre: " << nombre << endl;
    cout << "  - Duración: " << duracion << " segundos" << endl;
    cout << "  - Rutas: " << endl;
    cout << "      128 kbps: " << ruta128 << endl;
    cout << "      320 kbps: " << ruta320 << endl;
    cout << "-------------------------------------" << endl;
    cout << "  Créditos:" << endl;
    creditos.mostrar();
    cout << "_________________________________________________" << endl;
}

// ──────────────────────────────────────────────
// Función auxiliar para limpiar espacios
// ──────────────────────────────────────────────
static string limpiarTexto(const string& str) {
    size_t inicio = 0;
    size_t fin = str.size();

    while (inicio < fin && isspace(static_cast<unsigned char>(str[inicio])))
        inicio++;
    while (fin > inicio && isspace(static_cast<unsigned char>(str[fin - 1])))
        fin--;

    return str.substr(inicio, fin - inicio);
}

// ──────────────────────────────────────────────
// Cargar canciones desde archivo (con créditos)
// ──────────────────────────────────────────────
Cancion* Cancion::cargarCanciones(const string& rutaArchivo, int& cantidad) {
    ifstream file(rutaArchivo);
    if (!file.is_open()) {
        cerr << "Error: no se pudo abrir el archivo de canciones: " << rutaArchivo << endl;
        cantidad = 0;
        return nullptr;
    }

    // Contar líneas no vacías
    cantidad = 0;
    string linea;
    while (getline(file, linea))
        if (!linea.empty()) cantidad++;

    if (cantidad == 0) {
        file.close();
        cerr << "Error: el archivo de canciones está vacío." << endl;
        return nullptr;
    }

    Cancion* canciones = new Cancion[cantidad];
    file.clear();
    file.seekg(0);

    int i = 0;
    while (getline(file, linea) && i < cantidad) {
        if (linea.empty()) continue;

        try {
            stringstream ss(linea);
            string temp;
            string nombre, ruta128, ruta320;
            int idAlbum = 0, duracion = 0;

            // Leer campos separados por coma
            if (!getline(ss, temp, ',')) continue;
            idAlbum = stoi(limpiarTexto(temp));

            if (!getline(ss, nombre, ',')) continue;
            nombre = limpiarTexto(nombre);

            if (!getline(ss, ruta128, ',')) continue;
            ruta128 = limpiarTexto(ruta128);

            if (!getline(ss, ruta320, ',')) continue;
            ruta320 = limpiarTexto(ruta320);

            if (!getline(ss, temp, ',')) continue;
            duracion = stoi(limpiarTexto(temp));

            // 🔹 Cargar créditos automáticamente desde archivo fijo
            Creditos cred = Creditos::desdeArchivo(idAlbum);

            // Crear canción completa
            canciones[i] = Cancion(idAlbum, nombre, ruta128, ruta320, duracion, cred);
            i++;

        } catch (...) {
            cerr << "Error al procesar la línea: " << linea << endl;
        }
    }

    file.close();
    cantidad = i;
    cout << " Canciones cargadas correctamente: " << cantidad << endl;
    return canciones;
}
