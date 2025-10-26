#include "plataforma.h"
#include <fstream>
#include <sstream>
#include <iostream>
#include <string>
#include <thread>
#include <chrono>
#include <cstdlib>
using namespace std;

// ============================
// Constructor y Destructor
// ============================
Plataforma::Plataforma()
    : artistas(nullptr), albumes(nullptr), canciones(nullptr),
    cantArtistas(0), cantAlbumes(0), cantCanciones(0) {}

Plataforma::~Plataforma() {
    liberarMemoria();
}

// ============================
// Función auxiliar: limpiar texto
// ============================
string limpiar(const string& texto) {
    size_t inicio = 0;
    size_t fin = texto.size();

    while (inicio < fin && isspace(static_cast<unsigned char>(texto[inicio])))
        inicio++;

    while (fin > inicio && isspace(static_cast<unsigned char>(texto[fin - 1])))
        fin--;

    return texto.substr(inicio, fin - inicio);
}

// ============================
// Inicio de sesión
// ============================
bool Plataforma::iniciarSesion(const string& archivo, const string& nickname,
                               const string& clave, Usuario& usuario) {
    ifstream file(archivo);
    if (!file.is_open()) {
        cerr << "Error: no se pudo abrir el archivo " << archivo << endl;
        return false;
    }

    string linea;
    while (getline(file, linea)) {
        stringstream ss(linea);
        string nick, membresiaStr, ciudad, pais, fecha, claveArchivo;

        getline(ss, nick, ',');
        getline(ss, membresiaStr, ',');
        getline(ss, ciudad, ',');
        getline(ss, pais, ',');
        getline(ss, fecha, ',');
        getline(ss, claveArchivo);

        nick = limpiar(nick);
        claveArchivo = limpiar(claveArchivo);

        if (nick == nickname && claveArchivo == clave) {
            usuario.setNickname(nick);
            usuario.setMembresia(stoi(membresiaStr));
            usuario.setCiudad(ciudad);
            usuario.setPais(pais);
            usuario.setFecha(fecha);
            usuario.setPassword(claveArchivo);
            file.close();
            return true;
        }
    }

    file.close();
    return false;
}

// ============================
// Cargar datos en memoria (CORREGIDO)
// ============================
bool Plataforma::cargarDatos(const string& rutaArtistas,
                             const string& rutaAlbumes,
                             const string& rutaCanciones) {
    cout << "\n=============================\n";
    cout << "    CARGANDO DATOS...\n";
    cout << "=============================\n";

    // 1. Cargar TODAS las canciones primero
    canciones = Cancion::cargarCanciones(rutaCanciones, cantCanciones);
    if (!canciones) {
        cerr << "Error: no se pudieron cargar canciones.\n";
        liberarMemoria();
        return false;
    }

    // 2. Cargar álbumes con las canciones ya cargadas
    albumes = Album::cargarTodos(rutaAlbumes, cantAlbumes, canciones, cantCanciones);
    if (!albumes) {
        cerr << "Error: no se pudieron cargar álbumes.\n";
        liberarMemoria();
        return false;
    }

    // 3. Cargar artistas con los álbumes ya cargados
    artistas = Artista::cargarTodos(rutaArtistas, cantArtistas, albumes, cantAlbumes);
    if (!artistas) {
        cerr << "Error: no se pudieron cargar artistas.\n";
        liberarMemoria();
        return false;
    }

    cout << "Artistas cargados: " << cantArtistas << endl;
    cout << "Albumes cargados: " << cantAlbumes << endl;
    cout << "Canciones totales: " << cantCanciones << endl;

    return true;
}

// ============================
// Liberar memoria
// ============================
void Plataforma::liberarMemoria() {
    // Liberar los arrays de punteros de cada artista
    if (artistas) {
        for (int i = 0; i < cantArtistas; i++) {
            Album** albumesArtista = artistas[i].getAlbumes();
            if (albumesArtista) {
                delete[] albumesArtista;
            }
        }
    }

    // Liberar los arrays de punteros de cada álbum
    if (albumes) {
        for (int i = 0; i < cantAlbumes; i++) {
            Cancion** cancionesAlbum = albumes[i].getCanciones();
            if (cancionesAlbum) {
                delete[] cancionesAlbum;
            }
        }
    }

    if (artistas) { delete[] artistas; artistas = nullptr; }
    if (albumes)  { delete[] albumes;  albumes  = nullptr; }
    if (canciones){ delete[] canciones; canciones = nullptr; }
}

// ============================
// Pausar consola
// ============================
void Plataforma::pausar() {
    cout << "\nPresione ENTER para continuar...";
    cin.ignore(10000, '\n');
    cin.get();
}

// ============================
// Ejecutar la aplicación
// ============================
void Plataforma::ejecutar(const Usuario& usuario) {
    pausar();
    bool salir = false;

    while (!salir) {
        Usuario usuarioCopia = usuario;
        menuArtistas(usuarioCopia, salir);
    }

    cout << "=============================\n";
    cout << "     GRACIAS POR USAR\n";
    cout << "       UDEA TUNES\n";
    cout << "=============================\n";
    cout << "\nHasta pronto, " << usuario.getNickname() << "!\n\n";
}

// ============================
// MENU: Lista de Artistas
// ============================
void Plataforma::menuArtistas(Usuario& usuario, bool& salir) {
    cout << "=============================\n";
    cout << "         UDEA TUNES\n";
    cout << "=============================\n";
    cout << "Usuario: " << usuario.getNickname() << "\n";
    cout << "=============================\n";
    cout << "     LISTA DE ARTISTAS\n";
    cout << "=============================\n\n";

    for (int i = 0; i < cantArtistas; i++) {
        artistas[i].mostrarResumen();
    }

    cout << "\n-----------------------------\n";
    cout << "  Ingrese código del artista\n";
    cout << "  [S/s] Salir\n";
    cout << "-----------------------------\n";
    cout << "Opción: ";

    string entrada;
    cin >> entrada;
    cin.ignore(10000, '\n');

    if (entrada == "S" || entrada == "s") {
        salir = true;
        return;
    }

    int codigoArtista;
    try {
        codigoArtista = stoi(entrada);
    } catch (...) {
        cout << "\nEntrada inválida. Ingrese un número o 'S' para salir.\n";
        pausar();
        return;
    }

    Artista* artistaSeleccionado = nullptr;
    for (int i = 0; i < cantArtistas; i++) {
        if (artistas[i].getCodigo() == codigoArtista) {
            artistaSeleccionado = &artistas[i];
            break;
        }
    }

    if (!artistaSeleccionado) {
        cout << "\nNo se encontró ningún artista con ese código.\n";
        pausar();
        return;
    }

    bool volverArtistas = false;
    while (!volverArtistas && !salir) {
        menuAlbumes(artistaSeleccionado, volverArtistas, salir);
    }
}

// ============================
// MENU: Álbumes del Artista
// ============================
void Plataforma::menuAlbumes(Artista* artistaSeleccionado,
                             bool& volverArtistas,
                             bool& salir) {

    cout << "=============================\n";
    cout << "    ARTISTA SELECCIONADO\n";
    cout << "=============================\n";
    artistaSeleccionado->mostrarInfo();

    cout << "\n=============================\n";
    cout << "    ÁLBUMES DEL ARTISTA\n";
    cout << "=============================\n\n";

    Album** albumesArtista = artistaSeleccionado->getAlbumes();
    int cantidadAlbumes = artistaSeleccionado->getCantAlbumes();

    if (!albumesArtista || cantidadAlbumes == 0) {
        cout << "No se encontraron álbumes para este artista.\n";
        pausar();
        volverArtistas = true;
        return;
    }

    // Mostrar los álbumes del artista
    for (int i = 0; i < cantidadAlbumes; i++) {
        albumesArtista[i]->mostrarResumen();
    }

    cout << "\n-----------------------------\n";
    cout << "  [I/i] Ver info del artista\n";
    cout << "  [A/a] Elegir otro artista\n";
    cout << "  [S/s] Salir\n";
    cout << "  O ingrese código del álbum\n";
    cout << "-----------------------------\n";
    cout << "Opción: ";

    string entrada;
    cin >> entrada;
    cin.ignore(10000, '\n');

    if (entrada == "S" || entrada == "s") {
        salir = true;
        return;
    }
    if (entrada == "I" || entrada == "i") {
        artistaSeleccionado->mostrarInfo();
        pausar();
        return;
    }
    if (entrada == "A" || entrada == "a") {
        volverArtistas = true;
        return;
    }

    int codigoAlbum;
    try {
        codigoAlbum = stoi(entrada);
    } catch (...) {
        cout << "\nEntrada inválida.\n";
        pausar();
        return;
    }

    Album* albumSeleccionado = nullptr;
    for (int i = 0; i < cantidadAlbumes; i++) {
        if (albumesArtista[i]->getIdAlbum() == codigoAlbum) {
            albumSeleccionado = albumesArtista[i];
            break;
        }
    }

    if (!albumSeleccionado) {
        cout << "\nNo se encontró ningún álbum con ese código.\n";
        pausar();
        return;
    }

    bool volverAlbumes = false;
    while (!volverAlbumes && !volverArtistas && !salir) {
        menuCanciones(albumSeleccionado, volverAlbumes, volverArtistas, salir);
    }
}

// ============================
// MENU: Canciones del Álbum
// ============================
void Plataforma::menuCanciones(Album* albumSeleccionado,
                               bool& volverAlbumes,
                               bool& volverArtistas,
                               bool& salir) {

    cout << "=============================\n";
    cout << "   CANCIONES DEL ÁLBUM\n";
    cout << "=============================\n";
    cout << "Álbum: " << albumSeleccionado->getNombre() << "\n";
    cout << "Género: " << albumSeleccionado->getGenero(0) << "\n";  // CORREGIDO: agregado índice
    cout << "-----------------------------\n";

    Cancion** cancionesAlbum = albumSeleccionado->getCanciones();
    int cantidad = albumSeleccionado->getCantCanciones();

    if (!cancionesAlbum || cantidad == 0) {
        cout << "No hay canciones en este álbum.\n";
        pausar();
        volverAlbumes = true;
        return;
    }

    for (int i = 0; i < cantidad; i++) {
        cout << i + 1 << ". " << cancionesAlbum[i]->getNombre()
        << " (" << cancionesAlbum[i]->getDuracion() << " seg)\n";
    }

    cout << "\n-----------------------------\n";
    cout << "  [R/r] Reproducir álbum completo\n";
    cout << "  [A/a] Volver a álbumes\n";
    cout << "  [T/t] Volver a artistas\n";
    cout << "  [S/s] Salir\n";
    cout << "  O ingrese número de canción para reproducir\n";
    cout << "-----------------------------\n";
    cout << "Opción: ";

    string entrada;
    cin >> entrada;

    if (entrada == "R" || entrada == "r") {
        albumSeleccionado->reproducirAleatorio();
        pausar();
        return;
    }
    if (entrada == "A" || entrada == "a") {
        volverAlbumes = true;
        return;
    }
    if (entrada == "T" || entrada == "t") {
        volverAlbumes = true;
        volverArtistas = true;
        return;
    }
    if (entrada == "S" || entrada == "s") {
        salir = true;
        return;
    }

    try {
        int opcion = stoi(entrada);
        if (opcion <= 0 || opcion > cantidad)
            throw invalid_argument("Fuera de rango");

        cancionesAlbum[opcion - 1]->mostrarResumen();

    } catch (...) {
        cout << "Entrada inválida.\n";
    }

    pausar();
}
