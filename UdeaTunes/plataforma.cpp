#include "plataforma.h"
#include <fstream>
#include <sstream>
#include <iostream>
#include <string>
#include <algorithm>
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
// Función auxiliar para limpiar texto
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
            usuario.setClave(claveArchivo);
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

    // Se asume que estos metodos estaticos existen
    artistas = Artista::cargarTodos(rutaArtistas, cantArtistas);
    albumes = Album::cargarTodos(rutaAlbumes, cantAlbumes);
    canciones = Cancion::cargarTodas(rutaCanciones, cantCanciones);

    if (!artistas || !albumes || !canciones) {
        cerr << "Error: No se pudieron cargar los datos.\n";
        liberarMemoria();
        return false;
    }

    cout << "✓ Artistas cargados: " << cantArtistas << endl;
    cout << "✓ Albumes cargados: " << cantAlbumes << endl;
    cout << "✓ Canciones cargadas: " << cantCanciones << endl;

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
// Métodos auxiliares de UI
// ============================
void Plataforma::limpiarPantalla() {
#ifdef _WIN32
    system("cls");
#else
    system("clear");
#endif
}

// Mejora en pausar para limpiar el buffer de forma segura
void Plataforma::pausar() {
    cout << "\nPresione ENTER para continuar...";
    cin.ignore(10000, '\n'); // Limpia hasta el siguiente salto de línea
    cin.get();
}

// ============================
// Ejecutar la aplicación
// ============================
void Plataforma::ejecutar(const Usuario& usuario) {
    pausar();
    bool salir = false;

    // Se usa const Usuario& para coincidir con la cabecera (inferred)
    while (!salir) {
        // Se usa una copia no-const porque la firma de menuArtistas la requiere
        Usuario usuarioCopia = usuario;
        menuArtistas(usuarioCopia, salir);
    }

    limpiarPantalla();
    cout << "=============================\n";
    cout << "     GRACIAS POR USAR\n";
    cout << "       UDEA TUNES\n";
    cout << "=============================\n";
    cout << "\n¡Hasta pronto, " << usuario.getNickname() << "!\n\n";
}

// ============================
// MENÚ: Lista de Artistas
// ============================
void Plataforma::menuArtistas(Usuario& usuario, bool& salir) {
    limpiarPantalla();

    cout << "=============================\n";
    cout << "    🎵 UDEA TUNES 🎵\n";
    cout << "=============================\n";
    cout << "Usuario: " << usuario.getNickname() << "\n";
    cout << "=============================\n";
    cout << "     LISTA DE ARTISTAS\n";
    cout << "=============================\n\n";

    for (int i = 0; i < cantArtistas; i++) {
        artistas[i].mostrarResumen();
    }

    cout << "\n━━━━━━━━━━━━━━━━━━━━━━━━━━━━━\n";
    cout << "  Ingrese codigo del artista\n";
    cout << "  [S/s] Salir\n"; // Opción de salir cambiada a letra
    cout << "━━━━━━━━━━━━━━━━━━━━━━━━━━━━━\n";
    cout << "→ Opcion: ";

    string entrada; // Leemos la entrada como string
    cin >> entrada;
    cin.ignore(10000, '\n'); // Limpiar buffer después de leer

    // Verificación para salir con 'S' o 's'
    if (entrada == "S" || entrada == "s") {
        salir = true;
        return;
    }

    // Intentar convertir la entrada a un código de artista (asumimos que todo lo que no es 's' es un código numérico)
    int codigoArtista;
    try {
        codigoArtista = stoi(entrada);
    } catch (const invalid_argument& e) {
        cout << "\n❌ Entrada invalida. Por favor ingrese un codigo numerico o 'S' para salir.\n";
        pausar();
        return;
    } catch (const out_of_range& e) {
        cout << "\n❌ El numero es demasiado grande.\n";
        pausar();
        return;
    }

    // Si la conversion fue exitosa, buscamos el artista
    Artista* artistaSeleccionado = nullptr;
    for (int i = 0; i < cantArtistas; i++) {
        if (artistas[i].getCodigo() == codigoArtista) {
            artistaSeleccionado = &artistas[i];
            break;
        }
    }

    if (!artistaSeleccionado) {
        cout << "\n❌ No se encontro ningun artista con ese codigo.\n";
        pausar();
        return;
    }

    // Navegar a menú de álbumes
    bool volverArtistas = false;
    while (!volverArtistas && !salir) {
        menuAlbumes(artistaSeleccionado, volverArtistas, salir);
    }
}

// ============================
// MENÚ: Álbumes del Artista
// ============================
void Plataforma::menuAlbumes(Artista* artistaSeleccionado, bool& volverArtistas, bool& salir) {
    limpiarPantalla();

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

    cout << "\n━━━━━━━━━━━━━━━━━━━━━━━━━━━━━\n";
    cout << "  [I/i] Ver info del artista\n"; // Cambiado a letra
    cout << "  [A/a] Elegir otro artista\n"; // Cambiado a letra
    cout << "  [S/s] Salir\n"; // Cambiado a letra
    cout << "  O ingrese codigo del album\n";
    cout << "━━━━━━━━━━━━━━━━━━━━━━━━━━━━━\n";
    cout << "→ Opcion: ";

    string entrada;
    cin >> entrada;
    cin.ignore(10000, '\n');

    // Control de opciones por letra
    if (entrada == "S" || entrada == "s") {
        salir = true;
        return;
    }
    if (entrada == "I" || entrada == "i") {
        limpiarPantalla();
        cout << "=============================\n";
        cout << "    INFORMACION DEL ARTISTA\n";
        cout << "=============================\n";
        artistaSeleccionado->mostrarInfo();
        pausar();
        return; // Vuelve al menú de álbumes
    }
    if (entrada == "A" || entrada == "a") {
        volverArtistas = true;
        return;
    }

    // Intentar convertir la entrada a un código de álbum (si no fue una letra de control)
    int codigoAlbum;
    try {
        codigoAlbum = stoi(entrada);
    } catch (const invalid_argument& e) {
        cout << "\n❌ Entrada invalida. Por favor ingrese un codigo numerico o una opcion de letra.\n";
        pausar();
        return;
    } catch (const out_of_range& e) {
        cout << "\n❌ El numero es demasiado grande.\n";
        pausar();
        return;
    }

    // Buscar álbum por código (usa codigoAlbum, que ahora es un int)
    Album* albumSeleccionado = nullptr;
    for (int i = 0; i < cantAlbumes; i++) {
        if (albumes[i].getCodigo() == codigoAlbum) {
            albumSeleccionado = &albumes[i];
            break;
        }
    }

    if (!albumSeleccionado) {
        cout << "\n❌ No se encontro ningun album con ese codigo.\n";
        pausar();
        return;
    }

    // Navegar a menú de canciones
    bool volverAlbumes = false;
    while (!volverAlbumes && !volverArtistas && !salir) {
        menuCanciones(albumSeleccionado, volverAlbumes, volverArtistas, salir);
    }
}

// ============================
// MENÚ: Canciones del Álbum
// ============================
void Plataforma::menuCanciones(Album* albumSeleccionado,
                               bool& volverAlbumes, bool& volverArtistas, bool& salir) {
    limpiarPantalla();

    cout << "=============================\n";
    cout << "    ALBUM SELECCIONADO\n";
    cout << "=============================\n";
    albumSeleccionado->mostrarInfo();

    cout << "\n=============================\n";
    cout << "    CANCIONES DEL ALBUM\n";
    cout << "=============================\n\n";

    int codigoAlbum = albumSeleccionado->getCodigo();
    bool tieneCanciones = false;
    int numCancion = 1;

    for (int i = 0; i < cantCanciones; i++) {
        if (canciones[i].getIdAlbum() == codigoAlbum) {
            cout << "  [" << numCancion++ << "] ";
            canciones[i].mostrarResumen();
            tieneCanciones = true;
        }
    }

    if (!tieneCanciones) {
        cout << "No se encontraron canciones para este album.\n";
        pausar();
        volverAlbumes = true;
        return;
    }

    cout << "\n━━━━━━━━━━━━━━━━━━━━━━━━━━━━━\n";
    cout << "  [A/a] Volver a albumes\n"; // Cambiado a letra
    cout << "  [T/t] Volver a artistas\n"; // Cambiado a letra
    cout << "  [S/s] Salir\n"; // Cambiado a letra
    cout << "  O ingrese # de cancion\n";
    cout << "━━━━━━━━━━━━━━━━━━━━━━━━━━━━━\n";
    cout << "→ Opcion: ";

    string entrada;
    cin >> entrada;
    cin.ignore(10000, '\n');

    // Control de opciones por letra
    if (entrada == "S" || entrada == "s") {
        salir = true;
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

    // Intentar convertir la entrada a un número de canción
    int opcion;
    try {
        opcion = stoi(entrada);
    } catch (const invalid_argument& e) {
        cout << "\n❌ Entrada invalida. Por favor ingrese un numero o una opcion de letra.\n";
        pausar();
        return;
    } catch (const out_of_range& e) {
        cout << "\n❌ El numero es demasiado grande.\n";
        pausar();
        return;
    }

    // Buscar canción por número
    int contador = 1;
    Cancion* cancionSeleccionada = nullptr;

    for (int i = 0; i < cantCanciones; i++) {
        if (canciones[i].getIdAlbum() == codigoAlbum) {
            if (contador == opcion) {
                cancionSeleccionada = &canciones[i];
                break;
            }
            contador++;
        }
    }

    if (!cancionSeleccionada) {
        cout << "\n❌ Numero de cancion invalido.\n";
        pausar();
        return;
    }

    // Mostrar detalles de la canción
    mostrarDetallesCancion(cancionSeleccionada, volverAlbumes, volverArtistas, salir);
}

// ============================
// MOSTRAR: Detalles de Canción
// ============================
void Plataforma::mostrarDetallesCancion(Cancion* cancion,
                                        bool& volverAlbumes, bool& volverArtistas, bool& salir) {
    limpiarPantalla();

    cout << "=============================\n";
    cout << "    DETALLES DE LA CANCION\n";
    cout << "=============================\n";
    cancion->mostrarInfo();

    cout << "\n━━━━━━━━━━━━━━━━━━━━━━━━━━━━━\n";
    cout << "  [C/c] Ver otras canciones\n"; // Cambiado a letra
    cout << "  [A/a] Ver albumes\n"; // Cambiado a letra
    cout << "  [T/t] Ver artistas\n"; // Cambiado a letra
    cout << "  [S/s] Salir\n"; // Cambiado a letra
    cout << "━━━━━━━━━━━━━━━━━━━━━━━━━━━━━\n";
    cout << "→ Opcion: ";

    string entrada;
    cin >> entrada;
    cin.ignore(10000, '\n');

    if (entrada == "S" || entrada == "s") {
        salir = true;
    } else if (entrada == "A" || entrada == "a") {
        volverAlbumes = true;
    } else if (entrada == "T" || entrada == "t") {
        volverAlbumes = true;
        volverArtistas = true;
    }
    // Si la opción es [C/c] o cualquier otra cosa no definida,
    // las flags de salida se mantienen en 'false', lo que
    // resulta en volver al menú de canciones por defecto.
}
