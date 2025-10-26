#include "album.h"
#include <iostream>
#include <fstream>
#include <sstream>
#include <cstdlib>
#include <ctime>
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

void Album::reproducirAleatorio() {
    if (!referenciasCanciones || cantidadCanciones == 0) {
        cout << "Este album no tiene canciones.\n";
        return;
    }

    srand(time(nullptr));
    int indice = rand() % cantidadCanciones;
    cout << "\nReproduciendo cancion aleatoria del album '" << nombre << "':\n";
    referenciasCanciones[indice]->reproducir();
}

// ==========================
// DEPURACION
// ==========================
void Album::mostrarDepuracion() const {
    cout << "\n====== DEPURACION DEL ALBUM ======\n";
    cout << "Direccion del objeto Album: " << this << endl;
    cout << "ID del Album: " << idAlbum << endl;
    cout << "Nombre: '" << nombre << "'" << endl;
    cout << "Generos: ";
    for (int i = 0; i < 4; i++) {
        cout << "[" << i << "]: '" << generos[i] << "' ";
    }
    cout << endl;
    cout << "Fecha de lanzamiento: '" << fechaLanzamiento << "'" << endl;
    cout << "Duracion total: " << duracionTotal << " min" << endl;
    cout << "Sello: '" << sello << "'" << endl;
    cout << "Portada: '" << portada << "'" << endl;
    cout << "Puntuacion: " << puntuacion << "/10" << endl;
    cout << "Cantidad de canciones: " << cantidadCanciones << endl;
    cout << "Direccion del arreglo de canciones: " << referenciasCanciones << endl;

    if (referenciasCanciones != nullptr && cantidadCanciones > 0) {
        cout << "\n--- Detalles de las canciones ---\n";
        for (int i = 0; i < cantidadCanciones; i++) {
            cout << "  Cancion[" << i << "] en direccion: " << referenciasCanciones[i];
            if (referenciasCanciones[i] != nullptr) {
                cout << "\n    - ID Cancion: " << referenciasCanciones[i]->getIdAlbum();
                cout << "\n    - Nombre: " << referenciasCanciones[i]->getNombre();
                cout << "\n    - Duracion: " << referenciasCanciones[i]->getDuracion() << "s";
            } else {
                cout << " -> PUNTERO NULO!";
            }
            cout << endl;
        }
    } else {
        cout << "  ADVERTENCIA: No hay canciones asignadas o el puntero es nulo.\n";
    }

    cout << "==================================\n\n";
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

    cout << "\n========================================\n";
    cout << "  RESUMEN DE CARGA DE ALBUMES\n";
    cout << "========================================\n";
    cout << "Total de albumes cargados: " << cantidad << endl;
    cout << "========================================\n\n";

    int totalCancionesAsignadas = 0;
    int albumesSinCanciones = 0;

    for (int i = 0; i < cantidad; i++) {
        int cant = lista[i].getCantCanciones();
        totalCancionesAsignadas += cant;

        cout << "\n--- Album #" << (i + 1) << " ---\n";
        cout << "  ID Album: " << lista[i].getIdAlbum() << endl;
        cout << "  Nombre: " << lista[i].getNombre() << endl;
        cout << "  Generos: ";
        for (int j = 0; j < 4; j++) {
            string gen = lista[i].getGenero(j);
            if (!gen.empty()) {
                cout << gen;
                if (j < 3 && !lista[i].getGenero(j+1).empty()) cout << ", ";
            }
        }
        cout << endl;
        cout << "  Fecha: " << lista[i].getFechaLanzamiento() << endl;
        cout << "  Duracion: " << lista[i].getDuracionTotal() << " min" << endl;
        cout << "  Sello: " << lista[i].getSello() << endl;
        cout << "  Portada: " << lista[i].getPortada() << endl;
        cout << "  Puntuacion: " << lista[i].getPuntuacion() << "/10" << endl;
        cout << "  >> Canciones asignadas: " << cant << endl;

        if (cant == 0) {
            albumesSinCanciones++;
            cout << "  ADVERTENCIA: Este album no tiene canciones asignadas!\n";
        } else {
            cout << "  OK: Canciones encontradas correctamente\n";
            Cancion** canciones = lista[i].getCanciones();
            int muestraMax = (cant < 3) ? cant : 3;
            cout << "  Muestra de canciones:\n";
            for (int k = 0; k < muestraMax; k++) {
                cout << "    " << (k+1) << ". " << canciones[k]->getNombre()
                << " (ID: " << canciones[k]->getIdAlbum() << ")\n";
            }
            if (cant > 3) {
                cout << "    ... y " << (cant - 3) << " canciones mas\n";
            }
        }
    }

    cout << "\n========================================\n";
    cout << "  ESTADISTICAS FINALES\n";
    cout << "========================================\n";
    cout << "Total de albumes: " << cantidad << endl;
    cout << "Total de canciones asignadas: " << totalCancionesAsignadas << endl;
    cout << "Albumes sin canciones: " << albumesSinCanciones << endl;
    cout << "Promedio de canciones por album: ";
    if (cantidad > 0) {
        cout << (float)totalCancionesAsignadas / cantidad << endl;
    } else {
        cout << "0" << endl;
    }
    cout << "========================================\n\n";

    return lista;
}
