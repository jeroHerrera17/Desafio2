#include "plataforma.h"
#include <fstream>
#include <sstream>
#include <iostream>
#include <string>
// Se eliminan las librerías <thread>, <chrono>, <cstdlib>
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
// Funcion auxiliar: limpiar texto
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
// Inicio de sesion
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
// Cargar datos en memoria
// ============================
bool Plataforma::cargarDatos(const string& rutaArtistas,
                             const string& rutaAlbumes,
                             const string& rutaCanciones) {
    cout << "\n=============================\n";
    cout << "     CARGANDO DATOS...\n";
    cout << "=============================\n";

    canciones = Cancion::cargarCanciones(rutaCanciones, cantCanciones);
    if (!canciones) { liberarMemoria(); return false; }

    albumes = Album::cargarTodos(rutaAlbumes, cantAlbumes, canciones, cantCanciones);
    if (!albumes) { liberarMemoria(); return false; }

    artistas = Artista::cargarTodos(rutaArtistas, cantArtistas, albumes, cantAlbumes);
    if (!artistas) { liberarMemoria(); return false; }

    cout << "Artistas cargados: " << cantArtistas << endl;
    cout << "Albumes cargados: " << cantAlbumes << endl;
    cout << "Canciones totales: " << cantCanciones << endl;

    return true;
}

// ============================
// Liberar memoria
// ============================
void Plataforma::liberarMemoria() {
    if (artistas) {
        for (int i = 0; i < cantArtistas; i++) {
            Album** albumesArtista = artistas[i].getAlbumes();
            if (albumesArtista) delete[] albumesArtista;
        }
        delete[] artistas;
        artistas = nullptr;
    }
    if (albumes) {
        for (int i = 0; i < cantAlbumes; i++) {
            Cancion** cancionesAlbum = albumes[i].getCanciones(); // Nota: Asumo que getCanciones() ahora retorna Cancion**
            if (cancionesAlbum) delete[] cancionesAlbum;
        }
        delete[] albumes;
        albumes = nullptr;
    }
    if (canciones) {
        delete[] canciones;
        canciones = nullptr;
    }
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
// Ejecutar la aplicacion
// ============================
void Plataforma::ejecutar(Usuario& usuario) {
    bool salir = false;
    while (!salir) {
        menuArtistas(usuario, salir);
    }

    cout << "=============================\n";
    cout << "     GRACIAS POR USAR\n";
    cout << "       UDEA TUNES\n";
    cout << "=============================\n";
    cout << "\nHasta pronto, " << usuario.getNickname() << "!\n\n";
}

// ============================
// MENU ARTISTAS
// ============================
void Plataforma::menuArtistas(Usuario& usuario, bool& salir) {
    while (!salir) {
        cout << "\n=============================\n";
        cout << "         UDEA TUNES\n";
        cout << "=============================\n";
        cout << "Usuario: " << usuario.getNickname() << "\n";
        cout << "=============================\n";
        cout << "     LISTA DE ARTISTAS\n";
        cout << "=============================\n";

        for (int i = 0; i < cantArtistas; i++) {
            artistas[i].mostrarResumen();
        }

        cout << "\n-----------------------------\n";
        cout << " Ingrese codigo del artista\n";
        cout << " [S/s] Salir\n";
        cout << "-----------------------------\n";
        cout << "Opcion: ";

        string entrada;
        cin >> entrada;
        cin.ignore(10000, '\n');

        if (entrada == "S" || entrada == "s") { salir = true; return; }

        int codigoArtista;
        try { codigoArtista = stoi(entrada); }
        catch (...) { cout << "\nEntrada invalida. Intente de nuevo.\n"; pausar(); continue; }

        Artista* artistaSeleccionado = nullptr;
        for (int i = 0; i < cantArtistas; i++) {
            if (artistas[i].getCodigo() == codigoArtista) { artistaSeleccionado = &artistas[i]; break; }
        }

        if (!artistaSeleccionado) { cout << "\nNo se encontro ningun artista con ese codigo.\n"; pausar(); continue; }

        if (usuario.getMembresia() >= 1) { // Premium
            menuPremium(artistaSeleccionado, usuario, salir);
        } else { // Estandar
            menuEstandar(artistaSeleccionado, usuario, salir);
        }
    }
}

// ============================
// MENU PREMIUM (SALTOS DE LÍNEA CORREGIDOS)
// ============================
void Plataforma::menuPremium(Artista* artista, Usuario& usuario, bool& salir) {
    bool volver = false;
    while (!salir && !volver) {
        cout << "\n=============================\n";
        cout << "    ARTISTA: " << artista->getNombre() << "\n";
        cout << "=============================\n";

        Album** albumesArtista = artista->getAlbumes();
        int cantAlb = artista->getCantAlbumes();

        if (!albumesArtista || cantAlb == 0) {
            cout << "No hay albumes para este artista.\n";
            pausar();
            return;
        }

        cout << "\nAlbumes disponibles:\n";
        for (int i = 0; i < cantAlb; i++) albumesArtista[i]->mostrarResumen();

        cout << "\n-----------------------------\n";
        // OPCIONES DE NAVEGACIÓN - Corregidas con \n
        cout << "[I/i] Info artista\n";
        cout << "[A/a] Otro artista\n";
        cout << "[S/s] Salir\n";
        cout << "Ingrese código del álbum: ";
        string entrada;
        cin >> entrada;
        cin.ignore(10000, '\n');

        if (entrada == "S" || entrada == "s") { salir = true; return; }
        if (entrada == "I" || entrada == "i") { artista->mostrarInfo(); pausar(); continue; }
        if (entrada == "A" || entrada == "a") { volver = true; continue; }

        int codigoAlbum;
        try { codigoAlbum = stoi(entrada); }
        catch (...) { cout << "\nEntrada invalida.\n"; pausar(); continue; }

        Album* albumSeleccionado = nullptr;
        for (int i = 0; i < cantAlb; i++) {
            if (albumesArtista[i]->getIdAlbum() == codigoAlbum) { albumSeleccionado = albumesArtista[i]; break; }
        }

        if (!albumSeleccionado) { cout << "\nNo se encontro album.\n"; pausar(); continue; }

        // Menú de canciones del album premium
        bool volverAlbum = false;
        while (!volverAlbum && !salir) {
            cout << "\n=============================\n";
            cout << "CANCIONES DE: " << albumSeleccionado->getNombre() << "\n";
            cout << "=============================\n";
            Cancion** cancionesAlbum = albumSeleccionado->getCanciones();
            int cantCanc = albumSeleccionado->getCantCanciones();

            if (!cancionesAlbum || cantCanc == 0) { cout << "No hay canciones.\n"; pausar(); break; }

            for (int i = 0; i < cantCanc; i++)
                cout << i + 1 << ". " << cancionesAlbum[i]->getNombre()
                     << " (" << cancionesAlbum[i]->getDuracion() << " seg)\n";

            // OPCIONES DE NAVEGACIÓN DE CANCIONES - Corregidas con \n
            cout << "\n[R/r] Reproducir álbum\n";
            cout << "[A/a] Volver a álbumes\n";
            cout << "[T/t] Volver a artistas\n";
            cout << "[S/s] Salir\n";
            cout << "O ingrese número de la canción: ";
            string opt;
            cin >> opt;
            cin.ignore(10000, '\n');

            if (opt == "S" || opt == "s") { salir = true; return; }
            if (opt == "A" || opt == "a") break; // volver a albumes
            if (opt == "T" || opt == "t") { volverAlbum = true; volver = true; break; }
            if (opt == "R" || opt == "r") {
                int tipo;
                cout << "1. Secuencial | 2. Aleatorio: ";
                if (!(cin >> tipo)) { cin.clear(); cin.ignore(10000,'\n'); cout << "\nEntrada invalida.\n"; pausar(); continue; }
                cin.ignore(10000,'\n');
                if (tipo == 1) albumSeleccionado->reproducirSecuencial();
                else if (tipo == 2) albumSeleccionado->reproducirAleatorio();
                else cout << "\nOpción de reproducción inválida.\n";
                pausar();
            } else {
                try {
                    int cancion = stoi(opt);
                    if (cancion <= 0 || cancion > cantCanc) throw invalid_argument("Fuera de rango");
                    cancionesAlbum[cancion - 1]->mostrarResumen();
                    pausar();
                } catch (...) { cout << "\nEntrada invalida.\n"; pausar(); }
            }
        }
    }
}

// ============================
// MENU ESTANDAR (SALTOS DE LÍNEA CORREGIDOS)
// ============================
void Plataforma::menuEstandar(Artista* artista, Usuario& usuario, bool& salir) {
    bool volver = false;
    while (!salir && !volver) {
        cout << "\n=============================\n";
        cout << "    ARTISTA: " << artista->getNombre() << "\n";
        cout << "=============================\n";

        Album** albumesArtista = artista->getAlbumes();
        int cantAlb = artista->getCantAlbumes();

        if (!albumesArtista || cantAlb == 0) {
            cout << "No hay albumes para este artista.\n";
            pausar();
            return;
        }

        cout << "\nAlbumes disponibles:\n";
        for (int i = 0; i < cantAlb; i++) albumesArtista[i]->mostrarResumen();

        cout << "\n-----------------------------\n";
        // OPCIONES DE NAVEGACIÓN - Corregidas con \n
        cout << "[I/i] Info artista\n";
        cout << "[A/a] Otro artista\n";
        cout << "[S/s] Salir\n";
        cout << "Ingrese código del álbum: ";
        string entrada;
        cin >> entrada;
        cin.ignore(10000, '\n');

        if (entrada == "S" || entrada == "s") { salir = true; return; }
        if (entrada == "I" || entrada == "i") { artista->mostrarInfo(); pausar(); continue; }
        if (entrada == "A" || entrada == "a") { volver = true; continue; }

        int codigoAlbum;
        try { codigoAlbum = stoi(entrada); }
        catch (...) { cout << "\nEntrada invalida.\n"; pausar(); continue; }

        Album* albumSeleccionado = nullptr;
        for (int i = 0; i < cantAlb; i++) {
            if (albumesArtista[i]->getIdAlbum() == codigoAlbum) { albumSeleccionado = albumesArtista[i]; break; }
        }

        if (!albumSeleccionado) { cout << "\nNo se encontro album.\n"; pausar(); continue; }

        // Menú canciones estandar
        bool volverAlbum = false;
        while (!volverAlbum && !salir) {
            cout << "\n=============================\n";
            cout << "CANCIONES DE: " << albumSeleccionado->getNombre() << "\n";
            cout << "=============================\n";
            Cancion** cancionesAlbum = albumSeleccionado->getCanciones();
            int cantCanc = albumSeleccionado->getCantCanciones();

            if (!cancionesAlbum || cantCanc == 0) { cout << "No hay canciones.\n"; pausar(); break; }

            for (int i = 0; i < cantCanc; i++)
                cout << i + 1 << ". " << cancionesAlbum[i]->getNombre()
                     << " (" << cancionesAlbum[i]->getDuracion() << " seg)\n";

            // OPCIONES DE NAVEGACIÓN DE CANCIONES - Corregidas con \n
            cout << "\n[R/r] Reproducir aleatorio\n";
            cout << "[A/a] Volver a álbumes\n";
            cout << "[T/t] Volver a artistas\n";
            cout << "[S/s] Salir\n";
            cout << "O ingrese número de la canción: ";
            string opt;
            cin >> opt;
            cin.ignore(10000, '\n');

            if (opt == "S" || opt == "s") { salir = true; return; }
            if (opt == "A" || opt == "a") break;
            if (opt == "T" || opt == "t") { volverAlbum = true; volver = true; break; }
            if (opt == "R" || opt == "r") {
                albumSeleccionado->reproducirAleatorioEstandar();
                pausar();
            } else {
                try {
                    int cancion = stoi(opt);
                    if (cancion <= 0 || cancion > cantCanc) throw invalid_argument("Fuera de rango");
                    cancionesAlbum[cancion - 1]->mostrarResumen();
                    pausar();
                } catch (...) { cout << "\nEntrada invalida.\n"; pausar(); }
            }
        }
    }
}
