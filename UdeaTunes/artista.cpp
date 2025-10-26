#include "artista.h"
#include "album.h"
#include <iostream>
#include <fstream>
#include <sstream>
using namespace std;

// ============================
// Constructores
// ============================
Artista::Artista()
    : codigo(0), edad(0), seguidores(0), posicion(0),
    albumes(nullptr), cantAlbumes(0) {}

Artista::Artista(int codigo, const string& nombre, int edad, const string& pais,
                 int seguidores, int posicion)
    : codigo(codigo), nombre(nombre), edad(edad), pais(pais),
    seguidores(seguidores), posicion(posicion),
    albumes(nullptr), cantAlbumes(0) {}

// ============================
// Getters
// ============================
int Artista::getCodigo() const { return codigo; }
string Artista::getNombre() const { return nombre; }
int Artista::getEdad() const { return edad; }
string Artista::getPais() const { return pais; }
int Artista::getSeguidores() const { return seguidores; }
int Artista::getPosicion() const { return posicion; }

// ============================
// Setters
// ============================
void Artista::setCodigo(int codigo) { this->codigo = codigo; }
void Artista::setNombre(const string& nombre) { this->nombre = nombre; }
void Artista::setEdad(int edad) { this->edad = edad; }
void Artista::setPais(const string& pais) { this->pais = pais; }
void Artista::setSeguidores(int seguidores) { this->seguidores = seguidores; }
void Artista::setPosicion(int posicion) { this->posicion = posicion; }

// ============================
// Manejo de álbumes
// ============================
void Artista::setAlbumes(Album** albumesArtista, int cantidad) {
    albumes = albumesArtista;
    cantAlbumes = cantidad;
}

Album** Artista::getAlbumes() const {
    return albumes;
}

int Artista::getCantAlbumes() const {
    return cantAlbumes;
}

// ============================
// Mostrar información completa
// ============================
void Artista::mostrarInfo() const {
    cout << "---------------------------------------\n";
    cout << "Codigo: " << codigo << endl;
    cout << "Nombre: " << nombre << endl;
    cout << "Edad: " << edad << endl;
    cout << "Pais: " << pais << endl;
    cout << "Seguidores: " << seguidores << endl;
    cout << "Posicion: " << posicion << endl;
    cout << "Albumes: " << cantAlbumes << endl;
    cout << "---------------------------------------\n";
}

// ============================
// Mostrar resumen en lista
// ============================
void Artista::mostrarResumen() const {
    cout << "[" << codigo << "] " << nombre << " (" << pais
         << "), Seguidores: " << seguidores << endl;
}

// ============================
// Cargar un artista por código
// ============================
bool Artista::cargarDesdeArchivo(const string& rutaArchivo, int codigoBuscado) {
    ifstream file(rutaArchivo);
    if (!file.is_open()) {
        cerr << "Error: no se pudo abrir el archivo " << rutaArchivo << endl;
        return false;
    }

    string linea;
    while (getline(file, linea)) {
        stringstream ss(linea);
        string codigoStr, nombreStr, edadStr, paisStr, seguidoresStr, posicionStr;

        getline(ss, codigoStr, ',');
        getline(ss, nombreStr, ',');
        getline(ss, edadStr, ',');
        getline(ss, paisStr, ',');
        getline(ss, seguidoresStr, ',');
        getline(ss, posicionStr, ',');

        int codigoLeido = stoi(codigoStr);
        if (codigoLeido == codigoBuscado) {
            codigo = codigoLeido;
            nombre = nombreStr;
            edad = stoi(edadStr);
            pais = paisStr;
            seguidores = stoi(seguidoresStr);
            posicion = stoi(posicionStr);
            file.close();
            return true;
        }
    }

    file.close();
    return false;
}

// ============================
// Mostrar todos los artistas del archivo
// ============================
void Artista::mostrarTodos(const string& rutaArchivo) {
    ifstream file(rutaArchivo);
    if (!file.is_open()) {
        cerr << "Error: no se pudo abrir el archivo " << rutaArchivo << endl;
        return;
    }

    string linea;
    cout << "\n========== LISTA DE ARTISTAS ==========\n";
    while (getline(file, linea)) {
        stringstream ss(linea);
        string codigoStr, nombreStr, edadStr, paisStr, seguidoresStr, posicionStr;

        getline(ss, codigoStr, ',');
        getline(ss, nombreStr, ',');
        getline(ss, edadStr, ',');
        getline(ss, paisStr, ',');
        getline(ss, seguidoresStr, ',');
        getline(ss, posicionStr, ',');

        cout << "[" << codigoStr << "] " << nombreStr << " (" << paisStr
             << "), Seguidores: " << seguidoresStr << endl;
    }
    cout << "=======================================\n";
    file.close();
}

// ============================
// Cargar todos los artistas y asignarles álbumes
// ============================
Artista* Artista::cargarTodos(const string& rutaArchivo, int& cantidad,
                              Album* albumesGlobal, int totalAlbumes) {
    ifstream file(rutaArchivo);
    if (!file.is_open()) {
        cerr << "Error: no se pudo abrir el archivo " << rutaArchivo << endl;
        cantidad = 0;
        return nullptr;
    }

    // Primera pasada: contar artistas
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
    Artista* artistas = new Artista[cantidad];

    // Segunda pasada: cargar datos
    file.clear();
    file.seekg(0);
    int i = 0;

    while (getline(file, linea) && i < cantidad) {
        stringstream ss(linea);
        string codigoStr, nombreStr, edadStr, paisStr, seguidoresStr, posicionStr;

        getline(ss, codigoStr, ',');
        getline(ss, nombreStr, ',');
        getline(ss, edadStr, ',');
        getline(ss, paisStr, ',');
        getline(ss, seguidoresStr, ',');
        getline(ss, posicionStr, ',');

        try {
            artistas[i].setCodigo(stoi(codigoStr));
            artistas[i].setNombre(nombreStr);
            artistas[i].setEdad(stoi(edadStr));
            artistas[i].setPais(paisStr);
            artistas[i].setSeguidores(stoi(seguidoresStr));
            artistas[i].setPosicion(stoi(posicionStr));
            i++;
        } catch (...) {
            cerr << "Advertencia: error al parsear línea: " << linea << endl;
        }
    }

    file.close();
    cantidad = i;

    // Ahora asignar álbumes a cada artista
    if (albumesGlobal && totalAlbumes > 0) {


        for (int i = 0; i < cantidad; i++) {
            int codigoArtista = artistas[i].getCodigo();



            // Contar cuántos álbumes tiene este artista
            int cantAlbumesArtista = 0;
            for (int j = 0; j < totalAlbumes; j++) {
                // Obtener el ID del artista dividiendo el idAlbum por 100
                int idArtistaDelAlbum = albumesGlobal[j].getIdAlbum() / 100;

                if (idArtistaDelAlbum == codigoArtista) {
                    cantAlbumesArtista++;

                }
            }


            // Crear array de punteros a álbumes
            if (cantAlbumesArtista > 0) {
                Album** albumesArtista = new Album*[cantAlbumesArtista];
                int idx = 0;

                for (int j = 0; j < totalAlbumes; j++) {
                    // IMPORTANTE: Usar la misma división (100) en ambos bucles
                    int idArtistaDelAlbum = albumesGlobal[j].getIdAlbum() / 100;
                    if (idArtistaDelAlbum == codigoArtista) {
                        albumesArtista[idx++] = &albumesGlobal[j];
                    }
                }

                artistas[i].setAlbumes(albumesArtista, cantAlbumesArtista);

            }
        }

    }

    return artistas;
}
