#include "plataforma.h"
#include "publicidad.h"
#include <fstream>
#include <sstream>
#include <iostream>
#include <string>
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
// Cargar datos en memoria
// ============================
bool Plataforma::cargarDatos(const string& rutaArtistas,
                             const string& rutaAlbumes,
                             const string& rutaCanciones,
                             const string& rutaListaFavoritos) {
    cout << "\n=============================\n";
    cout << "    CARGANDO DATOS...\n";
    cout << "=============================\n";

    canciones = Cancion::cargarCanciones(rutaCanciones, cantCanciones);
    if (!canciones) {
        cerr << "Error: no se pudieron cargar canciones.\n";
        liberarMemoria();
        return false;
    }

    albumes = Album::cargarTodos(rutaAlbumes, cantAlbumes, canciones, cantCanciones);
    if (!albumes) {
        cerr << "Error: no se pudieron cargar álbumes.\n";
        liberarMemoria();
        return false;
    }

    artistas = Artista::cargarTodos(rutaArtistas, cantArtistas, albumes, cantAlbumes);
    if (!artistas) {
        cerr << "Error: no se pudieron cargar artistas.\n";
        liberarMemoria();
        return false;
    }

    listasFavoritos = listaFavoritos::cargarTodos(rutaListaFavoritos, cantListasFavoritos, canciones, cantCanciones);
    if (!listasFavoritos) {
        cerr << "Error: no se pudieron cargar los albumes.\n";
        liberarMemoria();
        return false;
    }

    cout << "Artistas cargados: " << cantArtistas << endl;
    cout << "Albumes cargados: " << cantAlbumes << endl;
    cout << "Canciones totales: " << cantCanciones << endl;
    cout << "usuarios con listas de favoritos totales: " << cantListasFavoritos << endl;

    return true;
}

// ============================
// Liberar memoria
// ============================
void Plataforma::liberarMemoria() {
    if (artistas) {
        for (int i = 0; i < cantArtistas; i++) {
            Album** albumesArtista = artistas[i].getAlbumes();
            if (albumesArtista) {
                delete[] albumesArtista;
            }
        }
    }

    if (artistas) { delete[] artistas; artistas = nullptr; }
    if (albumes)  { delete[] albumes;  albumes  = nullptr; }
    if (canciones){ delete[] canciones; canciones = nullptr; }
    if (listasFavoritos) { delete[] listasFavoritos; listasFavoritos = nullptr; }
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
        if (usuarioCopia.getMembresia() >= 1) {
            menuPremium(usuarioCopia, salir);
        } else {
            menuEstandar(usuarioCopia, salir);
        }
    }

    cout << "=============================\n";
    cout << "     GRACIAS POR USAR\n";
    cout << "       UDEA TUNES\n";
    cout << "=============================\n";
    cout << "\nHasta pronto, " << usuario.getNickname() << "!\n\n";
}

// ============================
// MENU PREMIUM
// ============================
void Plataforma::menuPremium(Usuario& usuario, bool& salir) {
    while (!salir) {

        for(int i = 0; i++; )
        cout << "\n=============================\n";
        cout << "         UDEA TUNES\n";
        cout << "=============================\n";
        cout << "Usuario: " << usuario.getNickname() << " (Premium)\n";
        cout << "=============================\n";
        cout << "     LISTA DE ARTISTAS\n";
        cout << "=============================\n\n";

        for (int i = 0; i < cantArtistas; i++) {
            artistas[i].mostrarResumen();
        }

        cout << "\n-----------------------------\n";
        cout << " Ingrese código del artista\n";
        cout << " [R/r] si deseas acceder al menu de tu lista de favoritos\n";
        cout << " [S/s] Salir\n";
        cout << "-----------------------------\n";
        cout << "Opción: ";

        string entrada;
        cin >> entrada;
        cin.ignore(10000, '\n');

        if (entrada == "S" || entrada == "s") { salir = true; return; }

        else if(entrada == "R" || entrada == "r"){
            Plataforma::menuListaFavoritos(usuario, salir);
        }


        int codigoArtista;
        try { codigoArtista = stoi(entrada); }
        catch (...) { cout << "\nEntrada inválida. Intente de nuevo.\n"; pausar(); continue; }

        Artista* artistaSeleccionado = nullptr;
        for (int i = 0; i < cantArtistas; i++) {
            if (artistas[i].getCodigo() == codigoArtista) { artistaSeleccionado = &artistas[i]; break; }
        }

        if (!artistaSeleccionado) { cout << "\nNo se encontró ningún artista con ese código.\n"; pausar(); continue; }

        bool volver = false;
        while (!salir && !volver) {
            menuAlbumesPremium(artistaSeleccionado, volver, salir);
        }
    }
}

//============================
//MENU LISTA FAVORITOS
//============================
void Plataforma::menuListaFavoritos(Usuario& usuario, bool& salir){
    string entrada;
    listaFavoritos* ptr;
    for(int i = 0; i < cantListasFavoritos; i++){
        if(usuario.getNickname() == listasFavoritos[i].getNombreDueño()){
            ptr = &listasFavoritos[i];
        }
    }
    cout << "\n\n\n\n=============================\n";
    cout << "      MENU LISTA DE FAVORITOS\n";
    cout << "=============================\n";
    cout << "Usuario: " << usuario.getNickname() << " (Premium)\n";
    cout << "=============================\n";
    cout << "     LISTA DE ACCIONES\n";
    cout << "=============================\n\n";

    cout << "\n-----------------------------\n";
    cout << " Ingrese la accion que desea realizar\n";
    cout << " [R/r] Reproducir lista de favoritos\n";
    cout << " [A/a] agregar cancion a la lista de favoritos\n";
    cout << " [E/e] eliminar cancion de la lista de favoritos\n";
    cout << " [S/s] Salir\n";
    cout << "-----------------------------\n";
    cout << "Opción: ";

    cin >> entrada;
    if(entrada ==  "R" || entrada == "r"){
        cout << "\n\n\n\n";
        ptr->reproducirListaFavoritos();
    }
    else if(entrada == "A" || entrada == "a"){
        cout << "\n\n\n\n";
        string IdCancion;
        cout << "Ingresa el codigo de la cancion que deseeas agregar: ";
        cin >> IdCancion;
        ptr->agregarCancion(IdCancion, canciones, cantCanciones);
    }
    else if(entrada == "E" || entrada == "e"){
        cout << "\n\n\n\n";
        string IdCancion;
        cout << "Ingresa el codigo de la cancion que deseeas eliminar: ";
        cin >> IdCancion;
        ptr->eliminarCancion(IdCancion);
    }
    else if(entrada == "S" || entrada == "s"){
        salir = true;
        return;
    }
}

// ============================
// MENU ÁLBUMES PREMIUM
// ============================
void Plataforma::menuAlbumesPremium(Artista* artista, bool& volver, bool& salir) {
    cout << "\n=============================\n";
    cout << "    ARTISTA: " << artista->getNombre() << "\n";
    cout << "=============================\n";

    Album** albumesArtista = artista->getAlbumes();
    int cantAlb = artista->getCantAlbumes();

    if (!albumesArtista || cantAlb == 0) {
        cout << "No hay álbumes para este artista.\n";
        pausar();
        return;
    }

    cout << "\nÁlbumes disponibles:\n";
    for (int i = 0; i < cantAlb; i++) albumesArtista[i]->mostrarResumen();

    cout << "\n-----------------------------\n";
    cout << "[I/i] Info artista\n";
    cout << "[A/a] Otro artista\n";
    cout << "[S/s] Salir\n";
    cout << "Ingrese código del álbum: ";

    string entrada;
    cin >> entrada;
    cin.ignore(10000, '\n');

    if (entrada == "S" || entrada == "s") { salir = true; return; }
    if (entrada == "I" || entrada == "i") { artista->mostrarInfo(); pausar(); return; }
    if (entrada == "A" || entrada == "a") { volver = true; return; }

    int codigoAlbum;
    try { codigoAlbum = stoi(entrada); }
    catch (...) { cout << "\nEntrada inválida.\n"; pausar(); return; }

    Album* albumSeleccionado = nullptr;
    for (int i = 0; i < cantAlb; i++) {
        if (albumesArtista[i]->getIdAlbum() == codigoAlbum) { albumSeleccionado = albumesArtista[i]; break; }
    }

    if (!albumSeleccionado) { cout << "\nNo se encontró álbum.\n"; pausar(); return; }

    bool volverAlbum = false;
    while (!volverAlbum && !salir) {
        menuCancionesPremium(albumSeleccionado, artista, volverAlbum, volver, salir);
    }
}

// ============================
// MENU CANCIONES PREMIUM
// ============================
void Plataforma::menuCancionesPremium(Album* album, Artista* artista, bool& volverAlbum, bool& volverArtistas, bool& salir) {
    cout << "\n=============================\n";
    cout << "CANCIONES DE: " << album->getNombre() << "\n";
    cout << "Portada: " << album->getPortada() << "\n";
    cout << "=============================\n";

    Cancion* cancionesAlbum = album->getCanciones();
    int cantCanc = album->getCantCanciones();

    if (!cancionesAlbum || cantCanc == 0) { cout << "No hay canciones.\n"; pausar(); return; }

    for (int i = 0; i < cantCanc; i++)
        cout << i + 1 << ". " << cancionesAlbum[i].getNombre()
             << " (" << cancionesAlbum[i].getDuracion() << " seg)\n";

    cout << "\n[R/r] Reproducir álbum\n";
    cout << "[A/a] Volver a álbumes\n";
    cout << "[T/t] Volver a artistas\n";
    cout << "[S/s] Salir\n";
    cout << "O ingrese número de la canción: ";

    string opt;
    cin >> opt;
    cin.ignore(10000, '\n');

    if (opt == "S" || opt == "s") { salir = true; return; }
    if (opt == "A" || opt == "a") { volverAlbum = true; return; }
    if (opt == "T" || opt == "t") { volverAlbum = true; volverArtistas = true; return; }

    if (opt == "R" || opt == "r") {
        int tipo;
        cout << "1. Secuencial | 2. Aleatorio: ";
        if (!(cin >> tipo)) { cin.clear(); cin.ignore(10000,'\n'); cout << "\nEntrada inválida.\n"; pausar(); return; }
        cin.ignore(10000,'\n');

        string nombreArtista = artista->getNombre();
        string portadaAlbum = album->getPortada();

        if (tipo == 1)
            album->reproducirSecuencial(nombreArtista, portadaAlbum);
        else if (tipo == 2)
            album->reproducirAleatorio(nombreArtista, portadaAlbum);
        else
            cout << "\nOpción de reproducción inválida.\n";
        pausar();
    } else {
        try {
            int cancion = stoi(opt);
            if (cancion <= 0 || cancion > cantCanc) throw invalid_argument("Fuera de rango");
            cancionesAlbum[cancion - 1].mostrarResumen();
            pausar();
        } catch (...) { cout << "\nEntrada inválida.\n"; pausar(); }
    }
}

// ============================
// MENU ESTÁNDAR
// ============================
void Plataforma::menuEstandar(Usuario& usuario, bool& salir) {
    int cantidadPublicidad = 0;
    publicidad* mensajesPublicitarios = publicidad().cargarPublicidad("../../Datos/publicidad.txt", cantidadPublicidad);
    if (!mensajesPublicitarios || cantidadPublicidad == 0) {
        cout << "No se encontraron mensajes publicitarios. Se continuará sin publicidad.\n";
        mensajesPublicitarios = nullptr;
        cantidadPublicidad = 0;
    }

    while (!salir) {
        cout << "\n=============================\n";
        cout << "         UDEA TUNES\n";
        cout << "=============================\n";
        cout << "Usuario: " << usuario.getNickname() << " (Estándar)\n";
        cout << "=============================\n";
        cout << "     LISTA DE ARTISTAS\n";
        cout << "=============================\n\n";

        for (int i = 0; i < cantArtistas; i++) {
            artistas[i].mostrarResumen();
        }

        cout << "\n-----------------------------\n";
        cout << " Ingrese código del artista\n";
        cout << " [S/s] Salir\n";
        cout << "-----------------------------\n";
        cout << "Opción: ";

        string entrada;
        cin >> entrada;
        cin.ignore(10000, '\n');

        if (entrada == "S" || entrada == "s") { salir = true; break; }

        int codigoArtista;
        try { codigoArtista = stoi(entrada); }
        catch (...) { cout << "\nEntrada inválida. Intente de nuevo.\n"; pausar(); continue; }

        Artista* artistaSeleccionado = nullptr;
        for (int i = 0; i < cantArtistas; i++) {
            if (artistas[i].getCodigo() == codigoArtista) { artistaSeleccionado = &artistas[i]; break; }
        }

        if (!artistaSeleccionado) { cout << "\nNo se encontró ningún artista con ese código.\n"; pausar(); continue; }

        bool volver = false;
        while (!salir && !volver) {
            menuAlbumesEstandar(artistaSeleccionado, mensajesPublicitarios, cantidadPublicidad, volver, salir);
        }
    }

    if (mensajesPublicitarios) delete[] mensajesPublicitarios;
}

// ============================
// MENU ÁLBUMES ESTÁNDAR
// ============================
void Plataforma::menuAlbumesEstandar(Artista* artista, publicidad* mensajesPublicitarios, int cantidadPublicidad, bool& volver, bool& salir) {
    cout << "\n=============================\n";
    cout << "    ARTISTA: " << artista->getNombre() << "\n";
    cout << "=============================\n";

    Album** albumesArtista = artista->getAlbumes();
    int cantAlb = artista->getCantAlbumes();

    if (!albumesArtista || cantAlb == 0) {
        cout << "No hay álbumes para este artista.\n";
        pausar();
        return;
    }

    cout << "\nÁlbumes disponibles:\n";
    for (int i = 0; i < cantAlb; i++) albumesArtista[i]->mostrarResumen();

    cout << "\n-----------------------------\n";
    cout << "[I/i] Info artista\n";
    cout << "[A/a] Otro artista\n";
    cout << "[S/s] Salir\n";
    cout << "Ingrese código del álbum: ";

    string entrada;
    cin >> entrada;
    cin.ignore(10000, '\n');

    if (entrada == "S" || entrada == "s") { salir = true; return; }
    if (entrada == "I" || entrada == "i") { artista->mostrarInfo(); pausar(); return; }
    if (entrada == "A" || entrada == "a") { volver = true; return; }

    int codigoAlbum;
    try { codigoAlbum = stoi(entrada); }
    catch (...) { cout << "\nEntrada inválida.\n"; pausar(); return; }

    Album* albumSeleccionado = nullptr;
    for (int i = 0; i < cantAlb; i++) {
        if (albumesArtista[i]->getIdAlbum() == codigoAlbum) { albumSeleccionado = albumesArtista[i]; break; }
    }

    if (!albumSeleccionado) { cout << "\nNo se encontró álbum.\n"; pausar(); return; }

    bool volverAlbum = false;
    while (!volverAlbum && !salir) {
        menuCancionesEstandar(albumSeleccionado, artista, mensajesPublicitarios, cantidadPublicidad, volverAlbum, volver, salir);
    }
}

// ============================
// MENU CANCIONES ESTÁNDAR
// ============================
void Plataforma::menuCancionesEstandar(Album* album, Artista* artista, publicidad* mensajesPublicitarios, int cantidadPublicidad, bool& volverAlbum, bool& volverArtistas, bool& salir) {
    cout << "\n=============================\n";
    cout << "CANCIONES DE: " << album->getNombre() << "\n";
    cout << "Portada: " << album->getPortada() << "\n";
    cout << "=============================\n";

    Cancion* cancionesAlbum = album->getCanciones();
    int cantCanc = album->getCantCanciones();

    if (!cancionesAlbum || cantCanc == 0) { cout << "No hay canciones.\n"; pausar(); return; }

    for (int i = 0; i < cantCanc; i++)
        cout << i + 1 << ". " << cancionesAlbum[i].getNombre()
             << " (" << cancionesAlbum[i].getDuracion() << " seg)\n";

    cout << "\n[R/r] Reproducir aleatorio\n";
    cout << "[A/a] Volver a álbumes\n";
    cout << "[T/t] Volver a artistas\n";
    cout << "[S/s] Salir\n";
    cout << "O ingrese número de la canción: ";

    string opt;
    cin >> opt;
    cin.ignore(10000, '\n');

    if (opt == "S" || opt == "s") { salir = true; return; }
    if (opt == "A" || opt == "a") { volverAlbum = true; return; }
    if (opt == "T" || opt == "t") { volverAlbum = true; volverArtistas = true; return; }

    if (opt == "R" || opt == "r") {
        string nombreArtista = artista->getNombre();
        string portadaAlbum = album->getPortada();

        album->reproducirAleatorioEstandar(mensajesPublicitarios, cantidadPublicidad, nombreArtista, portadaAlbum);
        pausar();
    } else {
        try {
            int cancion = stoi(opt);
            if (cancion <= 0 || cancion > cantCanc) throw invalid_argument("Fuera de rango");
            cancionesAlbum[cancion - 1].mostrarResumen();
            pausar();
        } catch (...) { cout << "\nEntrada inválida.\n"; pausar(); }
    }
}
