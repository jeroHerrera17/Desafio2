#include "artista.h"
#include "album.h"
#include <iostream>
#include <fstream>
#include <sstream>
using namespace std;

// ============================
// Constructores
// ============================

// Constructor por defecto, inicializa valores vacíos
Artista::Artista()
    : codigo(0), edad(0), seguidores(0), posicion(0),
    albumes(nullptr), cantAlbumes(0) {}

// Constructor con parámetros
Artista::Artista(int codigo, const string& nombre, int edad, const string& pais,
                 int seguidores, int posicion)
    : codigo(codigo), nombre(nombre), edad(edad), pais(pais),
    seguidores(seguidores), posicion(posicion),
    albumes(nullptr), cantAlbumes(0) {}

// ============================
// Getters (métodos para obtener datos)
// ============================
int Artista::getCodigo() const { return codigo; }
string Artista::getNombre() const { return nombre; }
int Artista::getEdad() const { return edad; }
string Artista::getPais() const { return pais; }
int Artista::getSeguidores() const { return seguidores; }
int Artista::getPosicion() const { return posicion; }
Album** Artista::getAlbumes() const { return albumes; }
int Artista::getCantAlbumes() const { return cantAlbumes; }

// ============================
// Setters (métodos para asignar datos)
// ============================
void Artista::setCodigo(int codigo) { this->codigo = codigo; }
void Artista::setNombre(const string& nombre) { this->nombre = nombre; }
void Artista::setEdad(int edad) { this->edad = edad; }
void Artista::setPais(const string& pais) { this->pais = pais; }
void Artista::setSeguidores(int seguidores) { this->seguidores = seguidores; }
void Artista::setPosicion(int posicion) { this->posicion = posicion; }

// Asigna un arreglo de álbumes a este artista
void Artista::setAlbumes(Album** albumesArtista, int cantidad) {
    albumes = albumesArtista;
    cantAlbumes = cantidad;
}

// ============================
// Mostrar información completa
// ============================

// Muestra todos los datos del artista
void Artista::mostrarInfo() const {
    cout << "---------------------------------------\n";
    cout << "Codigo: " << codigo << "\n";
    cout << "Nombre: " << nombre << "\n";
    cout << "Edad: " << edad << "\n";
    cout << "Pais: " << pais << "\n";
    cout << "Seguidores: " << seguidores << "\n";
    cout << "Posicion: " << posicion << "\n";
    cout << "Albumes: " << cantAlbumes << "\n";
    cout << "---------------------------------------\n";
}

// ============================
// Mostrar resumen en lista
// ============================

// Muestra datos básicos del artista (resumen)
void Artista::mostrarResumen() const {
    cout << "[" << codigo << "] " << nombre << " (" << pais
         << "), Seguidores: " << seguidores << "\n";
}

// ============================
// Cargar un artista por código
// ============================

// Busca un artista en un archivo CSV por su código
bool Artista::cargarDesdeArchivo(const string& rutaArchivo, int codigoBuscado) {
    ifstream file(rutaArchivo);
    if (!file.is_open()) {
        cerr << "Error: no se pudo abrir el archivo " << rutaArchivo << "\n";
        return false;
    }

    string linea;
    while (getline(file, linea)) {
        stringstream ss(linea);
        string codigoStr, nombreStr, edadStr, paisStr, seguidoresStr, posicionStr;

        // Leer los campos separados por coma
        getline(ss, codigoStr, ',');
        getline(ss, nombreStr, ',');
        getline(ss, edadStr, ',');
        getline(ss, paisStr, ',');
        getline(ss, seguidoresStr, ',');
        getline(ss, posicionStr, ',');

        // Convertir el código a entero
        int codigoLeido = stoi(codigoStr);

        // Si coincide con el código buscado, asignar datos
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
    return false; // No se encontró el artista
}

// ============================
// Mostrar todos los artistas del archivo
// ============================

// Recorre el archivo CSV y muestra cada artista
void Artista::mostrarTodos(const string& rutaArchivo) {
    ifstream file(rutaArchivo);
    if (!file.is_open()) {
        cerr << "Error: no se pudo abrir el archivo " << rutaArchivo << "\n";
        return;
    }

    string linea;
    cout << "\n========== LISTA DE ARTISTAS ==========\n";
    while (getline(file, linea)) {
        stringstream ss(linea);
        string codigoStr, nombreStr, edadStr, paisStr, seguidoresStr, posicionStr;

        // Leer los valores separados por coma
        getline(ss, codigoStr, ',');
        getline(ss, nombreStr, ',');
        getline(ss, edadStr, ',');
        getline(ss, paisStr, ',');
        getline(ss, seguidoresStr, ',');
        getline(ss, posicionStr, ',');

        // Mostrar formato corto
        cout << "[" << codigoStr << "] " << nombreStr << " (" << paisStr
             << "), Seguidores: " << seguidoresStr << "\n";
    }
    cout << "=======================================\n";
    file.close();
}

// ============================
// Cargar todos los artistas y asignarles álbumes
// ============================

// Carga todos los artistas del archivo y les asigna sus álbumes correspondientes
Artista* Artista::cargarTodos(const string& rutaArchivo, int& cantidad,
                              Album* albumesGlobal, int totalAlbumes) {
    int iteraciones = 0;
    size_t memoria = sizeof(cantidad) + sizeof(albumesGlobal) + sizeof(totalAlbumes);

    ifstream file(rutaArchivo);
    if (!file.is_open()) {
        cerr << "Error: no se pudo abrir el archivo " << rutaArchivo << "\n";
        cantidad = 0;
        return nullptr;
    }

    // Contar cuántos artistas hay en el archivo
    cantidad = 0;
    string linea;
    while (getline(file, linea)) {
        iteraciones++;
        if (!linea.empty()) cantidad++;
    }

    if (cantidad == 0) {
        file.close();
        return nullptr;
    }

    // Crear arreglo dinámico de artistas
    Artista* artistas = new Artista[cantidad];
    memoria += sizeof(Artista) * cantidad;

    // Reiniciar lectura desde el principio
    file.clear();
    file.seekg(0);
    int i = 0;

    // Leer cada línea y llenar los objetos
    while (getline(file, linea) && i < cantidad) {
        iteraciones++;
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

            // Contabilizar memoria usada por strings
            memoria += nombreStr.size() + paisStr.size();
            i++;
        } catch (...) {
            cerr << "Advertencia: error al parsear línea: " << linea << "\n";
        }
    }

    file.close();
    cantidad = i;

    // Asignar álbumes a cada artista
    if (albumesGlobal && totalAlbumes > 0) {
        for (int i = 0; i < cantidad; i++) {
            iteraciones++;
            int codigoArtista = artistas[i].getCodigo();

            // Contar cuántos álbumes pertenecen a este artista
            int cantAlbumesArtista = 0;
            for (int j = 0; j < totalAlbumes; j++) {
                iteraciones++;
                if (albumesGlobal[j].getIdAlbum() / 100 == codigoArtista)
                    cantAlbumesArtista++;
            }

            // Si tiene álbumes, asignar punteros a ellos
            if (cantAlbumesArtista > 0) {
                Album** albumesArtista = new Album*[cantAlbumesArtista];
                memoria += sizeof(Album*) * cantAlbumesArtista;

                int idx = 0;
                for (int j = 0; j < totalAlbumes; j++) {
                    iteraciones++;
                    if (albumesGlobal[j].getIdAlbum() / 100 == codigoArtista)
                        albumesArtista[idx++] = &albumesGlobal[j];
                }

                artistas[i].setAlbumes(albumesArtista, cantAlbumesArtista);
            }
        }
    }

    // ==========================
    // Mostrar resumen final
    // ==========================
    cout << "--- Resumen carga de artistas ---\n";
    cout << "Total cargados: " << cantidad << "\n";
    cout << "Iteraciones: " << iteraciones << "\n";

    cout << "Memoria estimada utilizada: " << memoria << " bytes";

    // Conversión manual sin iomanip
    if (memoria >= 1024 * 1024) {
        double mb = (double)memoria / (1024.0 * 1024.0);
        long parteEntera = (long)mb;
        long parteDecimal = (long)((mb - parteEntera) * 100);
        cout << " (" << parteEntera << "." << parteDecimal << " MB)";
    }
    else if (memoria >= 1024) {
        double kb = (double)memoria / 1024.0;
        long parteEntera = (long)kb;
        long parteDecimal = (long)((kb - parteEntera) * 100);
        cout << " (" << parteEntera << "." << parteDecimal << " KB)";
    }

    cout << "\n==============================================\n";

    return artistas;
}
