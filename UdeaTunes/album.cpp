#include "album.h"
#include "usuario.h"
#include <iostream>
#include <thread>
#include <chrono>
#include <fstream>
#include <sstream>
#include <cstdlib>
#include <ctime>
#include <algorithm> // Necesario para std::min en una implementación completa (aunque no en este archivo, se mantiene)
using namespace std;

// ==========================
// CONSTRUCTORES
// ==========================
Album::Album() {
    idAlbum = 0;
    nombre = "";
    for (int i = 0; i < 4; i++) {
        generos[i] = "";
    }
    fechaLanzamiento = "";
    duracionTotal = 0.0f;
    sello = "";
    portada = "";
    puntuacion = 0;
    referenciasCanciones = nullptr;
    cantidadCanciones = 0;
}

Album::Album(int idAlbum, const string& nombre, const string generos[4],
             const string& fechaLanzamiento, float duracionTotal,
             const string& sello, const string& portada, int puntuacion)
    : idAlbum(idAlbum), nombre(nombre), fechaLanzamiento(fechaLanzamiento),
    duracionTotal(duracionTotal), sello(sello), portada(portada),
    puntuacion(puntuacion), referenciasCanciones(nullptr), cantidadCanciones(0) {
    for (int i = 0; i < 4; i++) {
        this->generos[i] = generos[i];
    }
}

// ==========================
// GETTERS
// ==========================
int Album::getIdAlbum() const { return idAlbum; }
string Album::getNombre() const { return nombre; }
string Album::getGenero(int i) const {
    if (i >= 0 && i < 4) return generos[i];
    return "";
}
string Album::getFechaLanzamiento() const { return fechaLanzamiento; }
float Album::getDuracionTotal() const { return duracionTotal; }
string Album::getSello() const { return sello; }
string Album::getPortada() const { return portada; }
int Album::getPuntuacion() const { return puntuacion; }
Cancion** Album::getCanciones() const { return referenciasCanciones; }
int Album::getCantCanciones() const { return cantidadCanciones; }

// ==========================
// SETTERS
// ==========================
void Album::setGenero(int i, const string& g) {
    if (i >= 0 && i < 4) {
        generos[i] = g;
    }
}

void Album::setCanciones(Cancion** canciones, int cantidad) {
    referenciasCanciones = canciones;
    cantidadCanciones = cantidad;
}

// ==========================
// VISUALIZACION
// ==========================
void Album::mostrarInfo() const {
    cout << "=============================\n";
    cout << "Album: " << nombre << endl;
    cout << "ID: " << idAlbum << endl;
    cout << "Generos: ";
    for (int i = 0; i < 4; i++) {
        if (!generos[i].empty()) {
            cout << generos[i];
            if (i < 3 && !generos[i+1].empty()) cout << ", ";
        }
    }
    cout << "\nFecha de lanzamiento: " << fechaLanzamiento << endl;
    cout << "Duracion total: " << duracionTotal << " min" << endl;
    cout << "Sello: " << sello << endl;
    cout << "Puntuacion: " << puntuacion << "/10" << endl;
    cout << "Canciones: " << cantidadCanciones << endl;
    cout << "=============================\n";
}

void Album::mostrarResumen() const {
    cout << "[" << idAlbum << "] " << nombre << " (";
    for (int i = 0; i < 4; i++) {
        if (!generos[i].empty()) {
            cout << generos[i];
            if (i < 3 && !generos[i+1].empty()) cout << ", ";
        }
    }
    cout << ") - Puntuacion: " << puntuacion << "/10\n";
}

void Album::mostrarCanciones() const {
    if (!referenciasCanciones || cantidadCanciones == 0) {
        cout << "No hay canciones en este album.\n";
        return;
    }

    cout << "\n=== Canciones de '" << nombre << "' ===\n";
    for (int i = 0; i < cantidadCanciones; i++) {
        cout << (i + 1) << ". ";
        referenciasCanciones[i]->mostrarResumen();
    }
}

// ==========================
// FUNCIONALIDAD
// ==========================
void Album::asignarCanciones(Cancion* todas, int total) {
    cantidadCanciones = 0;
    for (int i = 0; i < total; i++) {
        int idCancion = todas[i].getIdAlbum();
        int albumDeCancion = idCancion / 100;
        if (albumDeCancion == idAlbum) {
            cantidadCanciones++;
        }
    }

    if (cantidadCanciones == 0) {
        referenciasCanciones = nullptr;
        return;
    }

    referenciasCanciones = new Cancion*[cantidadCanciones];
    int idx = 0;
    for (int i = 0; i < total; i++) {
        int idCancion = todas[i].getIdAlbum();
        int albumDeCancion = idCancion / 100;
        if (albumDeCancion == idAlbum) {
            referenciasCanciones[idx] = &todas[i];
            idx++;
        }
    }
}

//Función para reproducir en secuencia
void Album::reproducirSecuencial() {
    if (!referenciasCanciones || cantidadCanciones == 0) {
        cout << "Este álbum no tiene canciones.\n";
        return;
    }

    int pos = 0;
    string entrada;
    int opcion;
    string estado = "Reproduciendo";

    while (true) {
        // Acceder directamente sin usar índices auxiliares
        string nombre    = referenciasCanciones[pos]->getNombre();
        int duracion     = referenciasCanciones[pos]->getDuracion();
        string ruta      = referenciasCanciones[pos]->getRuta320();

        cout << "======================================================\n";
        cout << "| Nombre:   " << nombre << "\n";
        cout << "| Duración: " << duracion << " segundos\n";
        cout << "| Ruta:     " << ruta << "\n";
        cout << "| Estado:   " << estado << "\n";
        cout << "======================================================\n";
        cout << "\nOpciones:\n";
        cout << "1: Reproducir\n";
        cout << "2: Detener\n";
        cout << "3: Siguiente canción\n";
        cout << "4: Canción anterior\n";
        cout << "5: Salir\n";
        cout << "Ingrese opción: ";
        cin >> entrada;

        try {
            opcion = stoi(entrada);
        } catch (...) {
            cout << "Entrada no válida. Intente de nuevo.\n";
            continue;
        }

        switch (opcion) {
        case 1:
            estado = "Reproduciendo";
            cout << "|> Reproduciendo \"" << nombre << "\"...\n";
            break;

        case 2:
            estado = "Detenida";
            cout << " Reproducción detenida.\n";
            break;

        case 3: // Siguiente canción
            if (pos < cantidadCanciones - 1) {
                pos++;
                estado = "Reproduciendo";
            } else {
                cout << "  Esta es la última canción del álbum.\n";
            }
            break;

        case 4: // Canción anterior
            if (pos > 0) {
                pos--;
                estado = "Reproduciendo";
            } else {
                cout << "  Esta es la primera canción.\n";
            }
            break;

        case 5:
            cout << "== Saliendo del reproductor secuencial... ==\n";
            return;

        default:
            cout << "Opción no válida.\n";
            break;
        }
    }
}

void Album::reproducirAleatorio() {
    if (!referenciasCanciones || cantidadCanciones == 0) {
        cout << "Este album no tiene canciones.\n";
        return;
    }

    // Crear arreglo de indices 0..cantidadCanciones-1
    int* indices = new int[cantidadCanciones];
    for (int i = 0; i < cantidadCanciones; i++) {
        indices[i] = i;
    }

    // Mezclar los indices usando Fisher-Yates
    srand(time(nullptr));
    for (int i = cantidadCanciones - 1; i > 0; i--) {
        int j = rand() % (i + 1);
        swap(indices[i], indices[j]);
    }

    int pos = 0;
    string entrada;
    int opcion;
    string estado="Reproduciendo";
    while (true) {
        int indiceActual = indices[pos];
        string nombre = referenciasCanciones[indiceActual]->getNombre();
        int duracion = referenciasCanciones[indiceActual]->getDuracion();
        string ruta = referenciasCanciones[indiceActual]->getRuta320();

        cout << "======================================================\n";
        cout << "|----------------------------------------------------\n";
        cout << "| Nombre:   " << nombre << "\n";
        cout << "| Duracion: " << duracion << " segundos\n";
        cout << "| Ruta:     " << ruta << "\n";
        cout << "| Estado:   " << estado << "\n";
        cout << "======================================================\n";
        cout << "\nOpciones:\n";
        cout << "1: Reproducir\n";
        cout << "2: Detener\n";
        cout << "3: Siguiente cancion\n";
        cout << "4: Cancion anterior\n";
        cout << "5: Salir\n";
        cout << "Ingrese opcion: ";
        cin >> entrada;

        try {
            opcion = stoi(entrada);
        } catch (...) {
            cout << "Entrada no valida. Intente de nuevo.\n";
            continue;
        }

        switch (opcion) {
        case 1:
            estado = "Reproduciendo";
            cout << "|>  Reproduciendo \"" << nombre << "\"...\n";
            break;

        case 2:
            estado = "||Detenida";
            cout << "  Reproduccion detenida.\n";
            break;

        case 3: // Siguiente canción
            if (pos < cantidadCanciones - 1) {
                pos++;
                estado = "|>Reproduciendo";
            } else {
                cout << "  Esta es la última canción.\n";
                estado = "|>Reproduciendo";
            }
            break;

        case 4: // Canción anterior
            if (pos > 0) {
                pos--;
                estado = "Reproduciendo";
            } else {
                cout << "  Esta es la primera canción.!\n";
                estado = "Reproduciendo";
            }
            break;

        case 5:
            delete[] indices;
            cout << "==Saliendo del reproductor...==\n";
            return;

        default:
            cout << "Opcion no valida.\n";
            break;
        }
    }
}

void Album::reproducirAleatorioEstandar() {
    if (!referenciasCanciones || cantidadCanciones == 0) {
        cout << "Este álbum no tiene canciones.\n";
        return;
    }

    int* indices = new int[cantidadCanciones];
    for (int i = 0; i < cantidadCanciones; i++) indices[i] = i;

    srand(time(nullptr));
    for (int i = cantidadCanciones - 1; i > 0; i--) {
        int j = rand() % (i + 1);
        swap(indices[i], indices[j]);
    }

    cout << "->Usuario estándar: las canciones avanzan automáticamente cada 5 segundos.\n";
    int pos = 0;
    string estado = "Reproduciendo";

    while (pos < cantidadCanciones) {
        int idx = indices[pos];
        cout << "======================================================\n";
        cout << "| Nombre:   " << referenciasCanciones[idx]->getNombre() << "\n";
        cout << "| Duración: " << referenciasCanciones[idx]->getDuracion() << " segundos\n";
        cout << "| Estado:   " << estado << "\n";
        cout << "======================================================\n";
        std::this_thread::sleep_for(std::chrono::seconds(5));
        pos++;
    }

    cout << " Fin del álbum.\n";
    delete[] indices;
}


// ==========================
// ARCHIVOS
// ==========================
static string limpiar(const string& str) {
    size_t inicio = 0;
    size_t fin = str.size();
    while (inicio < fin && (str[inicio] == ' ' || str[inicio] == '\t' ||
                            str[inicio] == '\n' || str[inicio] == '\r'))
        inicio++;
    while (fin > inicio && (str[fin - 1] == ' ' || str[fin - 1] == '\t' ||
                            str[fin - 1] == '\n' || str[fin - 1] == '\r'))
        fin--;
    return str.substr(inicio, fin - inicio);
}

static void parsearGeneros(const string& texto, string generos[4]) {
    for (int i = 0; i < 4; i++) generos[i] = "";
    string temp = texto;
    if (!temp.empty() && temp.front() == '[') temp = temp.substr(1);
    if (!temp.empty() && temp.back() == ']') temp.pop_back();
    stringstream ss(temp);
    string genero;
    int i = 0;
    while (getline(ss, genero, ',') && i < 4) {
        generos[i++] = limpiar(genero);
    }
}

// LÓGICA DE CARGA SIN MENSAJES DE DEPURACIÓN
Album* Album::cargarTodos(const string& rutaArchivo, int& cantidad,
                          Cancion* todasCanciones, int totalCanciones) {
    ifstream file(rutaArchivo);
    if (!file.is_open()) {
        cerr << "Error al abrir el archivo de albumes: " << rutaArchivo << endl;
        cantidad = 0;
        return nullptr;
    }

    string linea;
    cantidad = 0;
    // Se asume que el tamaño fijo de 1000 es suficiente
    Album* lista = new Album[1000];

    while (getline(file, linea)) {
        if (linea.empty()) continue;

        try {
            size_t pos = 0;
            size_t finIdAlbum = linea.find(',', pos);
            string idStr = limpiar(linea.substr(pos, finIdAlbum - pos));
            pos = finIdAlbum + 1;

            size_t inicioCorcheteGeneros = linea.find('[', pos);
            size_t finCorcheteGeneros = linea.find(']', inicioCorcheteGeneros);
            string generosStr = linea.substr(inicioCorcheteGeneros, finCorcheteGeneros - inicioCorcheteGeneros + 1);
            pos = finCorcheteGeneros + 2;

            size_t finFecha = linea.find(',', pos);
            string fecha = limpiar(linea.substr(pos, finFecha - pos));
            pos = finFecha + 1;

            size_t finDuracion = linea.find(',', pos);
            string durStr = limpiar(linea.substr(pos, finDuracion - pos));
            pos = finDuracion + 1;

            size_t finSello = linea.find(',', pos);
            string sello = limpiar(linea.substr(pos, finSello - pos));
            pos = finSello + 1;

            size_t finPortada = linea.find(',', pos);
            string portada = limpiar(linea.substr(pos, finPortada - pos));
            pos = finPortada + 1;

            string puntStr = limpiar(linea.substr(pos));

            if (idStr.empty() || durStr.empty() || puntStr.empty()) {
                cerr << "Advertencia: Linea con campos vacios: " << linea << endl;
                continue;
            }

            int id = stoi(idStr);
            float duracion = stof(durStr);
            int punt = stoi(puntStr);

            string generos[4];
            parsearGeneros(generosStr, generos);
            string nombreAlbum = sello + " - Album " + idStr;

            lista[cantidad] = Album(id, nombreAlbum, generos, fecha, duracion,
                                    sello, portada, punt);

            if (todasCanciones != nullptr && totalCanciones > 0) {
                lista[cantidad].asignarCanciones(todasCanciones, totalCanciones);
            }

            cantidad++;
        } catch (...) {
            cerr << "Error al parsear linea: " << linea << endl;
        }
    }

    file.close();

    // NOTA: Se eliminaron las variables 'totalCancionesAsignadas' y
    // 'albumesSinCanciones' ya que solo se utilizaban para imprimir estadísticas.

    // La lógica principal de carga de datos finaliza aquí.
    return lista;
}
