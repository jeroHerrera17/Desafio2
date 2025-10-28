#include "album.h"
#include "publicidad.h"
#include <iostream>
#include <fstream>
#include <sstream>
#include <ctime>
#include <thread>
#include <chrono>
using namespace std;

// ==========================
// CONSTRUCTORES
// ==========================
Album::Album()
    : idAlbum(0), nombre(""), fechaLanzamiento(""),
    duracionTotal(0.0f), sello(""), portada(""), puntuacion(0),
    referenciasCanciones(nullptr), cantidadCanciones(0) {
    for (int i = 0; i < 4; i++) generos[i] = ""; // inicializamos generos vacios
    size_t memoria = sizeof(*this); // calculamos memoria usada

}

Album::Album(int idAlbum, const string& nombre, const string* generos,
             const string& fechaLanzamiento, float duracionTotal,
             const string& sello, const string& portada, int puntuacion)
    : idAlbum(idAlbum), nombre(nombre), fechaLanzamiento(fechaLanzamiento),
    duracionTotal(duracionTotal), sello(sello), portada(portada),
    puntuacion(puntuacion), referenciasCanciones(nullptr), cantidadCanciones(0)
{
    for (int i = 0; i < 4; i++) {
        this->generos[i] = generos[i]; // copiamos generos
    }
    size_t memoria = sizeof(*this) + sizeof(generos); // memoria estimada
}

// ==========================
// ASIGNAR CANCIONES
// ==========================
void Album::asignarCanciones(Cancion* todas, int total) {
    int iteraciones = 0; // contaremos cuantas iteraciones usamos
    size_t memoria = sizeof(total) + sizeof(cantidadCanciones) + sizeof(referenciasCanciones);

    cantidadCanciones = 0;

    for (int i = 0; i < total; i++) { // contamos canciones del album
        iteraciones++;
        if (todas[i].getIdAlbum() / 100 == idAlbum)
            cantidadCanciones++;
    }

    if (cantidadCanciones == 0) {
        referenciasCanciones = nullptr;
        cout << "[AsignarCanciones] Iteraciones: " << iteraciones << ", Memoria usada: " << memoria << " bytes\n";
        return;
    }

    referenciasCanciones = new Cancion[cantidadCanciones]; // arreglo dinamico de canciones
    memoria += sizeof(Cancion) * cantidadCanciones;

    int idx = 0;
    for (int i = 0; i < total; i++) {
        iteraciones++;
        if (todas[i].getIdAlbum() / 100 == idAlbum) {
            referenciasCanciones[idx] = todas[i];
            idx++;

        }
        else{
            return;
        }
    }

    cout << "[AsignarCanciones] Iteraciones: " << iteraciones << ", Memoria usada: " << memoria << " bytes\n";
}

// ==========================
// GETTERS
// ==========================
int Album::getIdAlbum() const { return idAlbum; }
string Album::getNombre() const { return nombre; }
int Album::getCantCanciones() const { return cantidadCanciones; }
Cancion* Album::getCanciones() const { return referenciasCanciones; }
string Album::getPortada() const { return portada; }

string Album::getGenero(int indice) const {
    if (indice >= 0 && indice < 4 && !generos[indice].empty())
        return generos[indice];
    return "Desconocido";
}

// ==========================
// MOSTRAR RESUMEN
// ==========================
void Album::mostrarResumen() const {
    int iteraciones = 0;
    size_t memoria = sizeof(idAlbum) + sizeof(nombre) + sizeof(generos) + sizeof(puntuacion);

    cout << "[" << idAlbum << "] " << nombre << " (";
    for (int i = 0; i < 4; i++) {
        iteraciones++;
        if (!generos[i].empty()) {
            cout << generos[i];
            if (i < 3 && !generos[i + 1].empty()) cout << ", ";
        }
    }
    cout << ") - Puntuación: " << puntuacion << "/10\n";

    cout << "[MostrarResumen] Iteraciones: " << iteraciones << ", Memoria usada: " << memoria << " bytes\n";
}

// ==========================
// REPRODUCCION SECUENCIAL
// ==========================
void Album::reproducirSecuencial(const string& nombreArtista, const string& rutaPortada) {
    int iteraciones = 0;
    size_t memoria = sizeof(cantidadCanciones) + sizeof(referenciasCanciones) + sizeof(nombreArtista) + sizeof(rutaPortada);

    if (cantidadCanciones == 0 || referenciasCanciones == nullptr) {
        cout << "Este álbum no tiene canciones.\n";
        cout << "[ReproducirSecuencial] Iteraciones: " << iteraciones << ", Memoria usada: " << memoria << " bytes\n";
        return;
    }

    int pos = 0;
    string entrada;
    int opcion;
    string estado = "Reproduciendo";
    string albumNombre = this->getNombre();

    while (true) {
        iteraciones++;
        Cancion* cancion = &referenciasCanciones[pos];
        string nombreCancion = cancion->getNombre();
        int duracion = cancion->getDuracion();
        string ruta = cancion->getRuta320();

        cout << "======================================================\n";
        cout << "| Artista: " << nombreArtista << "\n";
        cout << "| Álbum: " << albumNombre << "\n";
        cout << "| Portada: " << rutaPortada << "\n";
        cout << "| Nombre: " << nombreCancion << "\n";
        cout << "| Duración: " << duracion << " segundos\n";
        cout << "| Ruta: " << ruta << "\n";
        cout << "| Estado: " << estado << "\n";
        cout << "======================================================\n";

        cout << "\nOpciones:\n1: Reproducir\n2: Detener\n3: Siguiente canción\n4: Canción anterior\n5: Salir\nIngrese opción: ";
        cin >> entrada;
        cin.ignore(10000, '\n');

        try { opcion = stoi(entrada); } catch (...) { cout << "Entrada no válida\n"; continue; }

        switch (opcion) {
        case 1: estado = "Reproduciendo"; cout << "Reproduciendo \"" << nombreCancion << "\"...\n"; break;
        case 2: estado = "Detenida"; cout << "Reproducción detenida.\n"; break;
        case 3: if (pos < cantidadCanciones-1) { pos++; estado="Reproduciendo"; } else cout<<"Ultima canción\n"; break;
        case 4: if (pos>0) { pos--; estado="Reproduciendo"; } else cout<<"Primera canción\n"; break;
        case 5: cout<<"Saliendo...\n"; cout << "[ReproducirSecuencial] Iteraciones: " << iteraciones << ", Memoria usada: " << memoria << " bytes\n"; return;
        default: cout<<"Opción no válida\n"; break;
        }
    }
}

// ==========================
// REPRODUCCION ALEATORIA
// ==========================
void Album::reproducirAleatorio(const string& nombreArtista, const string& rutaPortada) {
    int iteraciones=0;
    size_t memoria=sizeof(cantidadCanciones)+sizeof(referenciasCanciones);

    if(cantidadCanciones==0||referenciasCanciones==nullptr){
        cout<<"Este álbum no tiene canciones.\n";
        cout<<"[ReproducirAleatorio] Iteraciones: "<<iteraciones<<", Memoria usada: "<<memoria<<" bytes\n";
        return;
    }

    int* indices=new int[cantidadCanciones];
    memoria+=sizeof(int)*cantidadCanciones;

    for(int i=0;i<cantidadCanciones;i++) indices[i]=i;

    srand(time(nullptr));
    for(int i=cantidadCanciones-1;i>0;i--){
        int j = rand()%(i+1);
        int temp=indices[i]; indices[i]=indices[j]; indices[j]=temp; // intercambio manual
    }

    int pos=0;
    string entrada;
    int opcion;
    string estado="Reproduciendo";
    string albumNombre=this->getNombre();

    while(true){
        iteraciones++;
        int idx=indices[pos];
        Cancion* cancion=&referenciasCanciones[idx];
        string nombreCancion=cancion->getNombre();
        int duracion=cancion->getDuracion();
        string ruta=cancion->getRuta320();

        cout<<"======================================================\n";
        cout<<"| Artista: "<<nombreArtista<<"\n";
        cout<<"| Álbum: "<<albumNombre<<"\n";
        cout<<"| Portada: "<<rutaPortada<<"\n";
        cout<<"| Nombre: "<<nombreCancion<<"\n";
        cout<<"| Duración: "<<duracion<<" segundos\n";
        cout<<"| Ruta: "<<ruta<<"\n";
        cout<<"| Estado: "<<estado<<"\n";
        cout<<"======================================================\n";

        cout<<"\nOpciones:\n1: Reproducir\n2: Detener\n3: Siguiente canción\n4: Canción anterior\n5: Salir\nIngrese opción: ";
        cin>>entrada; cin.ignore(10000,'\n');

        try{opcion=stoi(entrada);}catch(...){cout<<"Entrada no válida\n"; continue;}

        switch(opcion){
        case 1: estado="Reproduciendo"; cout<<"Reproduciendo \""<<nombreCancion<<"\"...\n"; break;
        case 2: estado="Detenida"; cout<<"Reproducción detenida.\n"; break;
        case 3: if(pos<cantidadCanciones-1){pos++; estado="Reproduciendo";} else cout<<"Última canción\n"; break;
        case 4: if(pos>0){pos--; estado="Reproduciendo";} else cout<<"Primera canción\n"; break;
        case 5: delete[] indices; cout<<"Saliendo...\n"; cout<<"[ReproducirAleatorio] Iteraciones: "<<iteraciones<<", Memoria usada: "<<memoria<<" bytes\n"; return;
        default: cout<<"Opción no válida\n"; break;
        }
    }
}

// ==========================
// REPRODUCCION ALEATORIA ESTANDAR
// ==========================
void Album::reproducirAleatorioEstandar(publicidad* mensajesPublicitarios, int cantidadPublicidad,
                                        const string& nombreArtista, const string& rutaPortada){
    int iteraciones=0;
    size_t memoria=sizeof(cantidadCanciones)+sizeof(referenciasCanciones);

    if(cantidadCanciones==0||referenciasCanciones==nullptr){
        cout<<"Este álbum no tiene canciones.\n";
        cout<<"[ReproducirAleatorioEstandar] Iteraciones: "<<iteraciones<<", Memoria usada: "<<memoria<<" bytes\n";
        return;
    }

    int* indices=new int[cantidadCanciones];
    memoria+=sizeof(int)*cantidadCanciones;
    for(int i=0;i<cantidadCanciones;i++) indices[i]=i;

    srand(time(nullptr));
    for(int i=cantidadCanciones-1;i>0;i--){
        int j=rand()%(i+1);
        int temp=indices[i]; indices[i]=indices[j]; indices[j]=temp; // intercambio manual
    }

    cout<<"-> Usuario estándar: las canciones avanzan automáticamente cada 5 segundos.\n";
    int pos=0;
    string estado="Reproduciendo";
    int contadorPublicidad=0;
    string albumNombre=this->getNombre();

    while(pos<cantidadCanciones){
        iteraciones++;
        int idx=indices[pos];
        Cancion* cancion=&referenciasCanciones[idx];
        string nombreCancion=cancion->getNombre();
        int duracion=cancion->getDuracion();
        string ruta=cancion->getRuta128();

        cout<<"======================================================\n";
        cout<<"| Artista: "<<nombreArtista<<"\n";
        cout<<"| Álbum: "<<albumNombre<<"\n";
        cout<<"| Portada: "<<rutaPortada<<"\n";
        cout<<"| Nombre: "<<nombreCancion<<"\n";
        cout<<"| Duración: "<<duracion<<" segundos\n";
        cout<<"| Ruta: "<<ruta<<"\n";
        cout<<"| Estado: "<<estado<<"\n";
        cout<<"======================================================\n";

        this_thread::sleep_for(chrono::seconds(5));
        pos++;
        contadorPublicidad++;

        if(contadorPublicidad==3 && cantidadPublicidad>0){
            publicidad pub;
            pub.verPublicidad(mensajesPublicitarios,cantidadPublicidad);
            contadorPublicidad=0;
        }
    }

    delete[] indices;
    cout<<"Fin del álbum.\n";
    cout<<"[ReproducirAleatorioEstandar] Iteraciones: "<<iteraciones<<", Memoria usada: "<<memoria<<" bytes\n";
}

// ==========================
// FUNCIONES AUXILIARES
// ==========================
static string limpiar(const string& str){
    size_t inicio=0, fin=str.size();
    while(inicio<fin && isspace(str[inicio])) inicio++;
    while(fin>inicio && isspace(str[fin-1])) fin--;
    return str.substr(inicio, fin-inicio);
}

static void parsearGeneros(const string& texto, string generos[4]){
    for(int i=0;i<4;i++) generos[i]="";
    string temp=texto;
    if(!temp.empty() && temp.front()=='[') temp=temp.substr(1);
    if(!temp.empty() && temp.back()==']') temp.pop_back();
    stringstream ss(temp);
    string genero;
    int i=0;
    while(getline(ss,genero,',') && i<4) generos[i++]=limpiar(genero);
}

// ==========================
// CARGA DESDE ARCHIVO
// ==========================
Album* Album::cargarTodos(const string& rutaArchivo, int& cantidad,
                          Cancion* todasCanciones, int totalCanciones){
    int iteraciones=0;
    size_t memoria=sizeof(cantidad)+sizeof(totalCanciones)+sizeof(todasCanciones);

    ifstream file(rutaArchivo);
    if(!file.is_open()){
        cerr<<"Error al abrir archivo: "<<rutaArchivo<<"\n";
        cantidad=0;
        return nullptr;
    }

    cantidad=0;
    Album* lista=new Album[1000];
    memoria+=sizeof(Album)*1000;

    string linea;
    while(getline(file,linea)){
        iteraciones++;
        if(linea.empty()) continue;

        try{
            size_t pos=0;
            size_t finId=linea.find(',',pos);
            string idStr=limpiar(linea.substr(pos,finId-pos));
            pos=finId+1;

            size_t iniGen=linea.find('[',pos);
            size_t finGen=linea.find(']',iniGen);
            string generosStr=linea.substr(iniGen,finGen-iniGen+1);
            pos=finGen+2;

            size_t finFecha=linea.find(',',pos);
            string fecha=limpiar(linea.substr(pos,finFecha-pos));
            pos=finFecha+1;

            size_t finDur=linea.find(',',pos);
            string durStr=limpiar(linea.substr(pos,finDur-pos));
            pos=finDur+1;

            size_t finSello=linea.find(',',pos);
            string sello=limpiar(linea.substr(pos,finSello-pos));
            pos=finSello+1;

            size_t finPortada=linea.find(',',pos);
            string portada=limpiar(linea.substr(pos,finPortada-pos));
            pos=finPortada+1;

            string puntStr=limpiar(linea.substr(pos));

            int id=stoi(idStr);
            float dur=stof(durStr);
            int punt=stoi(puntStr);

            string generos[4];
            parsearGeneros(generosStr,generos);
            string nombreAlbum=sello+" - Album "+idStr;

            lista[cantidad]=Album(id,nombreAlbum,generos,fecha,dur,sello,portada,punt);
            if(todasCanciones!=nullptr && totalCanciones>0){
                lista[cantidad].asignarCanciones(todasCanciones,totalCanciones);
            }
            cantidad++;
        }catch(...){
            cerr<<"Error procesando linea: "<<linea<<"\n";
        }
    }

    // mostrar resumen de la carga
    cout << "--- Resumen carga de álbumes ---\n";
    cout << "Total cargados: " << cantidad << "\n";
    cout << "Iteraciones: " << iteraciones << "\n";

    // mostrar la memoria estimada usada
    cout << "Memoria estimada utilizada: " << memoria << " bytes";

    // si la memoria supera 1 MB, mostrar también en MB
    if (memoria >= 1024 * 1024) {
        double mb = (double)memoria / (1024.0 * 1024.0);

        // mostrar con 2 decimales manualmente
        long parteEntera = (long)mb;
        long parteDecimal = (long)((mb - parteEntera) * 100); // solo dos decimales

        cout << " (" << parteEntera << "." << parteDecimal << " MB)";
    }
    // si es menor que 1 MB pero mayor que 1 KB, mostrar en KB
    else if (memoria >= 1024) {
        double kb = (double)memoria / 1024.0;

        // mostrar con 2 decimales manualmente
        long parteEntera = (long)kb;
        long parteDecimal = (long)((kb - parteEntera) * 100);

        cout << " (" << parteEntera << "." << parteDecimal << " KB)";
    }

    cout << "\n==============================================\n";

    return lista;
    return lista;
}
