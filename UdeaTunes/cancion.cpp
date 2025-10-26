#include "cancion.h"
#include "creditos.h"
#include <iostream>
#include <fstream>
#include <sstream>
using namespace std;

Cancion::Cancion() : idAlbum(0), nombre(""), ruta128(""), ruta320(""), duracion(0), creditos() {}

Cancion::Cancion(int idAlbum, const string& nombre, const string& ruta128,
                 const string& ruta320, int duracion, const Creditos& creditos)
    : idAlbum(idAlbum), nombre(nombre), ruta128(ruta128),
    ruta320(ruta320), duracion(duracion), creditos(creditos) {}

// Getters
int Cancion::getIdAlbum() const { return idAlbum; }
string Cancion::getNombre() const { return nombre; }
string Cancion::getRuta128() const { return ruta128; }
string Cancion::getRuta320() const { return ruta320; }
int Cancion::getDuracion() const { return duracion; }
Creditos Cancion::getCreditos() const { return creditos; }

// Mostrar información básica
void Cancion::mostrarInfo() const {
    cout << "\n" << nombre << "\n";
    cout << "\tID Álbum: " << idAlbum << "\n";
    cout << "\tDuración: " << duracion << " segundos\n";
    cout << "\tRuta 128 kbps: " << ruta128 << "\n";
    cout << "\tRuta 320 kbps: " << ruta320 << "\n";
}

// Mostrar resumen con créditos
void Cancion::mostrarResumen() const {
    cout << "════════════════════════════════════════" << endl;
    cout << " **Resumen de Canción** " << endl;
    cout << "  - ID Álbum: " << idAlbum << endl;
    cout << "  - Nombre: " << nombre << endl;
    cout << "  - Duración: " << duracion << " segundos" << endl;
    cout << "  - Rutas: " << endl;
    cout << "      128 kbps: " << ruta128 << endl;
    cout << "      320 kbps: " << ruta320 << endl;
    cout << "  - Créditos:" << endl;
    creditos.mostrar();
    cout << "════════════════════════════════════════" << endl;
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

// Parsear texto de créditos "(nombre1... , nombre2... , etc)"
static Creditos parsearCreditos(const string& texto) {
    string temp = texto;
    if (!temp.empty() && temp.front() == '(' && temp.back() == ')')
        temp = temp.substr(1, temp.size() - 2);

    string tokens[50];
    int total = 0;
    string token;
    stringstream ss(temp);

    while (getline(ss, token, ',')) {
        tokens[total++] = limpiarTexto(token);
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

// Cargar canciones desde archivo de texto
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
            if (!getline(ss, temp, ',')) continue;
            int idAlbum = stoi(limpiarTexto(temp));

            // Leer nombre
            if (!getline(ss, nombre, ',')) continue;
            nombre = limpiarTexto(nombre);

            // Leer rutas
            getline(ss, ruta128, ',');
            ruta128 = limpiarTexto(ruta128);

            getline(ss, ruta320, ',');
            ruta320 = limpiarTexto(ruta320);

            // Leer duración
            if (!getline(ss, temp, ',')) continue;
            int duracion = stoi(limpiarTexto(temp));

            Creditos cred = parsearCreditos(creditosTxt);

            canciones[i] = Cancion(idAlbum, nombre, ruta128, ruta320, duracion, cred);
            i++;

        } catch (...) {
            cerr << "Error al procesar la línea: " << linea << endl;
        }
    }

    file.close();
    cantidad = i;
    cout << "✓ Canciones cargadas: " << cantidad << endl;
    return canciones;
}
