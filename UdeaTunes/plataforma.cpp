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
Plataforma::Plataforma() : artistas(nullptr), albumes(nullptr), canciones(nullptr),
    cantArtistas(0), cantAlbumes(0), cantCanciones(0) {}

Plataforma::~Plataforma() {
    liberarMemoria();
}

// ============================
// Funcion auxiliar para limpiar texto
// ============================
string limpiar(const string& texto) {
    size_t inicio = 0;
    size_t fin = texto.size();

    while (inicio < fin && (texto[inicio] == ' ' || texto[inicio] == '\n' ||
                            texto[inicio] == '\r' || texto[inicio] == '\t')) {
        inicio++;
    }

    while (fin > inicio && (texto[fin - 1] == ' ' || texto[fin - 1] == '\n' ||
                            texto[fin - 1] == '\r' || texto[fin - 1] == '\t')) {
        fin--;
    }

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
bool Plataforma::cargarDatos(const string& rutaArtistas, const string& rutaAlbumes,
                             const string& rutaCanciones) {
    cout << "\n=============================\n";
    cout << "    CARGANDO DATOS...\n";
    cout << "=============================\n";

    artistas = Artista::cargarTodos(rutaArtistas, cantArtistas);
    albumes = Album::cargarTodos(rutaAlbumes, cantAlbumes);
    canciones = Cancion::cargarTodas(rutaCanciones, cantCanciones);

    if (!artistas || !albumes || !canciones) {
        cerr << "Error: no se pudieron cargar los datos.\n";
        liberarMemoria();
        return false;
    }

    cout << "Artistas cargados: " << cantArtistas << endl;
    cout << "Albumes cargados: " << cantAlbumes << endl;
    cout << "Canciones cargadas: " << cantCanciones << endl;

    return true;
}

// ============================
// Liberar memoria
// ============================
void Plataforma::liberarMemoria() {
    if (artistas) {
        delete[] artistas;
        artistas = nullptr;
    }
    if (albumes) {
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
    cout << "  Ingrese codigo del artista\n";
    cout << "  [S/s] Salir\n";
    cout << "-----------------------------\n";
    cout << "Opcion: ";

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
        cout << "\nEntrada invalida. Ingrese un numero o 'S' para salir.\n";
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
        cout << "\nNo se encontro ningun artista con ese codigo.\n";
        pausar();
        return;
    }

    bool volverArtistas = false;
    while (!volverArtistas && !salir) {
        menuAlbumes(artistaSeleccionado, volverArtistas, salir);
    }
}

// ============================
// MENU: Albumes del Artista
// ============================
void Plataforma::menuAlbumes(Artista* artistaSeleccionado, bool& volverArtistas, bool& salir) {

    cout << "=============================\n";
    cout << "    ARTISTA SELECCIONADO\n";
    cout << "=============================\n";
    artistaSeleccionado->mostrarInfo();

    cout << "\n=============================\n";
    cout << "    ALBUMES DEL ARTISTA\n";
    cout << "=============================\n\n";

    int codigoArtista = artistaSeleccionado->getCodigo();
    bool tieneAlbumes = false;

    for (int i = 0; i < cantAlbumes; i++) {
        if (albumes[i].getIdArtista() == codigoArtista) {
            albumes[i].mostrarResumen();
            tieneAlbumes = true;
        }
    }

    if (!tieneAlbumes) {
        cout << "No se encontraron albumes para este artista.\n";
        pausar();
        volverArtistas = true;
        return;
    }

    cout << "\n-----------------------------\n";
    cout << "  [I/i] Ver info del artista\n";
    cout << "  [A/a] Elegir otro artista\n";
    cout << "  [S/s] Salir\n";
    cout << "  O ingrese codigo del album\n";
    cout << "-----------------------------\n";
    cout << "Opcion: ";

    string entrada;
    cin >> entrada;
    cin.ignore(10000, '\n');

    if (entrada == "S" || entrada == "s") {
        salir = true;
        return;
    }
    if (entrada == "I" || entrada == "i") {
        cout << "=============================\n";
        cout << "    INFORMACION DEL ARTISTA\n";
        cout << "=============================\n";
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
        cout << "\nEntrada invalida. Ingrese un numero o una opcion de letra.\n";
        pausar();
        return;
    }

    Album* albumSeleccionado = nullptr;
    for (int i = 0; i < cantAlbumes; i++) {
        if (albumes[i].getCodigo() == codigoAlbum) {
            albumSeleccionado = &albumes[i];
            break;
        }
    }

    if (!albumSeleccionado) {
        cout << "\nNo se encontro ningun album con ese codigo.\n";
        pausar();
        return;
    }

    bool volverAlbumes = false;
    while (!volverAlbumes && !volverArtistas && !salir) {
        menuCanciones(albumSeleccionado, volverAlbumes, volverArtistas, salir);
    }
}

// ============================
// MENU: Canciones del Album
// ============================
void Plataforma::menuCanciones(Album* albumSeleccionado, bool& volverAlbumes,
                               bool& volverArtistas, bool& salir) {

    cout << "=============================\n";
    cout << "   CANCIONES DEL ALBUM\n";
    cout << "=============================\n";
    cout << "Album: " << albumSeleccionado->getNombre() << "\n";
    cout << "Genero: " << albumSeleccionado->getGenero() << "\n";
    cout << "-----------------------------\n";

    bool hayCanciones = false;
    int contador = 0;

    for (int i = 0; i < cantCanciones; i++) {
        int idAlbumCancion = canciones[i].getIdAlbum() / 100;
        if (idAlbumCancion == albumSeleccionado->getCodigo()) {
            cout << ++contador << ". " << canciones[i].getNombre()
            << " (" << canciones[i].getDuracion() << " seg)\n";
            hayCanciones = true;
        }
    }

    if (!hayCanciones) {
        cout << "No hay canciones registradas para este album.\n";
        pausar();
        volverAlbumes = true;
        return;
    }

    cout << "\n-----------------------------\n";
    cout << "  [R/r] Reproducir album completo\n";
    cout << "  [A/a] Volver a albumes\n";
    cout << "  [T/t] Volver a artistas\n";
    cout << "  [S/s] Salir\n";
    cout << "  O ingrese numero de cancion para reproducir\n";
    cout << "-----------------------------\n";
    cout << "Opcion: ";

    string entrada;
    cin >> entrada;

    if (entrada == "R" || entrada == "r") {
        albumSeleccionado->reproducirAleatorio(canciones, cantCanciones, albumSeleccionado->getCodigo());
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
        if (opcion <= 0 || opcion > contador) throw invalid_argument("Fuera de rango");

        int indice = -1;
        int cuenta = 0;

        for (int i = 0; i < cantCanciones; i++) {
            int idAlbumCancion = canciones[i].getIdAlbum() / 100;
            if (idAlbumCancion == albumSeleccionado->getCodigo()) {
                cuenta++;
                if (cuenta == opcion) {
                    indice = i;
                    break;
                }
            }
        }

        if (indice == -1) {
            cout << "Numero de cancion invalido.\n";
        } else {
            canciones[indice].reproducir();
        }

    } catch (invalid_argument&) {
        cout << "Entrada invalida.\n";
    }

    pausar();
}
