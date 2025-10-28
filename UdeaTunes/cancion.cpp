#include "cancion.h"
#include "creditos.h"
#include <iostream>
#include <fstream>
#include <sstream>
#include <cctype>
using namespace std;

// ============================
// Constructores
// ============================
Cancion::Cancion()
    : idAlbum(0), nombre(""), ruta128(""), ruta320(""), duracion(0), creditos() {}

Cancion::Cancion(int idAlbum, const string& nombre, const string& ruta128,
                 const string& ruta320, int duracion, const Creditos& creditos)
    : idAlbum(idAlbum), nombre(nombre), ruta128(ruta128),
    ruta320(ruta320), duracion(duracion), creditos(creditos) {}

// ============================
// Getters
// ============================
int Cancion::getIdAlbum() const { return idAlbum; }
string Cancion::getNombre() const { return nombre; }
string Cancion::getRuta128() const { return ruta128; }
string Cancion::getRuta320() const { return ruta320; }
int Cancion::getDuracion() const { return duracion; }
Creditos Cancion::getCreditos() const { return creditos; }

// ============================
// Mostrar informacion
// ============================
void Cancion::mostrarInfo() const {
    cout << "\nNombre: " << nombre << "\n";
    cout << "\tID Album: " << idAlbum << "\n";
    cout << "\tDuracion: " << duracion << " segundos\n";
    cout << "\tRuta 128 kbps: " << ruta128 << "\n";
    cout << "\tRuta 320 kbps: " << ruta320 << "\n";
}

// ============================
// Mostrar resumen con creditos
// ============================
void Cancion::mostrarResumen() const {
    cout << "\n-------------------------------------------------\n";
    cout << "Resumen de Cancion\n";
    cout << "  ID Album: " << idAlbum << "\n";
    cout << "  Nombre: " << nombre << "\n";
    cout << "  Duracion: " << duracion << " segundos\n";
    cout << "  Rutas:\n";
    cout << "      128 kbps: " << ruta128 << "\n";
    cout << "      320 kbps: " << ruta320 << "\n";
    cout << "  Creditos:\n";
    creditos.mostrar();
    cout << "-------------------------------------------------\n";
}

// ============================
// Funcion auxiliar para limpiar espacios
// ============================
static string limpiarTexto(const string& str) {
    size_t inicio = 0, fin = str.size();
    while (inicio < fin && isspace(static_cast<unsigned char>(str[inicio]))) inicio++;
    while (fin > inicio && isspace(static_cast<unsigned char>(str[fin - 1]))) fin--;
    return str.substr(inicio, fin - inicio);
}

// ============================
// Cargar canciones desde archivo
// ============================
Cancion* Cancion::cargarCanciones(const string& rutaArchivo, int& cantidad) {
    int iteraciones = 0;
    size_t memoriaUsada = 0;

    ifstream file(rutaArchivo);
    if (!file.is_open()) {
        cerr << "Error: no se pudo abrir el archivo de canciones: " << rutaArchivo << endl;
        cantidad = 0;
        return nullptr;
    }

    // Contar lineas no vacias
    cantidad = 0;
    string linea;
    while (getline(file, linea)) {
        iteraciones++;
        if (!linea.empty()) cantidad++;
    }

    if (cantidad == 0) {
        file.close();
        cerr << "Error: archivo de canciones vacio" << endl;
        return nullptr;
    }

    Cancion* canciones = new Cancion[cantidad];
    memoriaUsada += sizeof(Cancion) * cantidad;

    file.clear();
    file.seekg(0);
    int i = 0;

    while (getline(file, linea) && i < cantidad) {
        iteraciones++;
        if (linea.empty()) continue;

        try {
            stringstream ss(linea);
            string temp, nombre, ruta128, ruta320;
            int idAlbum = 0, duracion = 0;

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

            // Cargar creditos desde archivo
            Creditos cred = Creditos::desdeArchivo(idAlbum);
            memoriaUsada += sizeof(idAlbum) + sizeof(duracion) + sizeof(cred);

            canciones[i] = Cancion(idAlbum, nombre, ruta128, ruta320, duracion, cred);
            i++;

        } catch (...) {
            cerr << "Error al procesar la linea: " << linea << endl;
        }
    }

    file.close();
    cantidad = i;
    cout << "Canciones cargadas correctamente: " << cantidad << endl;
    cout << "Recursos usados: iteraciones=" << iteraciones
         << ", memoria aprox.=" << memoriaUsada << " bytes\n";

    return canciones;
}
