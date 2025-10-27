#include "usuario.h"
#include "listafavoritos.h"
#include <string>
#include <iostream>
#include <fstream>
#include <sstream>

using namespace std;

Usuario::Usuario(string _nickname, string _password, string _ciudad, string _pais,
                 string _fechaRegistro, bool _membresia, int _contadorPublicidad, listaFavoritos _MiLista) {
    nickname = _nickname;
    password = _password;
    ciudad = _ciudad;
    pais = _pais;
    fechaRegistro = _fechaRegistro;
    membresia = _membresia;
    contadorPublicidad = _contadorPublicidad;
    if (!_membresia)
        MiLista = listaFavoritos();
    else
        MiLista = _MiLista;
    listaSeguida = nullptr;
}

Usuario::Usuario() {
    contadorPublicidad = 0;
    membresia = false;
    MiLista = listaFavoritos();
    listaSeguida = nullptr;
}

// =====================
// Setters
// =====================
void Usuario::setNickname(const string& n) { nickname = n; }
void Usuario::setMembresia(bool m) { membresia = m; }
void Usuario::setCiudad(const string& c) { ciudad = c; }
void Usuario::setPais(const string& p) { pais = p; }
void Usuario::setFecha(const string& f) { fechaRegistro = f; }
void Usuario::setPassword(const string& c) { password = c; }
void Usuario::setListaFavoritos(const string& n, Cancion** l, int nC) {
    MiLista = listaFavoritos(n, l, nC);
}
void Usuario::setContadorPublicidad(int CP) { contadorPublicidad = CP; }

// =====================
// Getters
// =====================
string Usuario::getNickname() const { return nickname; }
bool Usuario::getMembresia() const { return membresia; }
string Usuario::getCiudad() const { return ciudad; }
string Usuario::getPais() const { return pais; }
string Usuario::getFecha() const { return fechaRegistro; }
string Usuario::getPassword() const { return password; }

// AGREGADO: Implementación del getter que faltaba
listaFavoritos Usuario::getListaFavoritos() const {
    return MiLista;
}

/*
 *===========================
 * Seguir listas de favoritos
 *===========================
*/
void Usuario::seguirListaFavoritos(const string& nombreDueño,Usuario* todos, int total){
    if(listaSeguida != nullptr){
        cout << "YA SIGUES UNA LISTA DE FAVORITOS, DEJALA DE SEGUIR PARA PODER SEGUIR OTRA LISTA" << endl;
        return;
    }
    bool encontrado = false;
    for(int i = 0; i < total; i++){
        if(nombreDueño == todos[i].getNickname()){
            listaSeguida = &todos[i].MiLista;
            encontrado = true;
            break;
        }
    }
    if(encontrado == false){
        cout << "Este usuario no existe, intente de nuevo";
    }
}

/*
 *====================================
 * Dejar de seguir lista de favoritos
 *====================================
*/
void Usuario::dejarDeSeguirListaFavoritos()  {listaSeguida = nullptr;}


// =====================
// Mostrar información
// =====================
void Usuario::mostrarInfo() {
    cout << "Usuario: " << nickname << endl;
    cout << "Membresia: " << (membresia ? "Premium" : "Estandar") << endl;
    cout << "Ciudad: " << ciudad << endl;
    cout << "Pais: " << pais << endl;
    cout << "Fecha de inscripcion: " << fechaRegistro << endl;
}

// =====================
// Cargar usuarios
// =====================
Usuario* Usuario::cargarUsuarios(const string& rutaArchivoU, const string& rutaArchivoLF, int& cantidad,Cancion* todas, int total) {
    ifstream archivo(rutaArchivoU);
    if (!archivo.is_open()) {
        cerr << "Error: no se pudo abrir el archivo " << rutaArchivoU << endl;
        cantidad = 0;
        return nullptr;
    }

    ifstream archivoLF(rutaArchivoLF);
    if (!archivoLF.is_open()) {
        cerr << "Error: no se pudo abrir el archivo " << rutaArchivoLF << endl;
        archivo.close();
        cantidad = 0;
        return nullptr;
    }

    // Contar usuarios
    cantidad = 0;
    string linea;
    while (getline(archivo, linea)) {
        if (!linea.empty())
            cantidad++;
    }

    if (cantidad == 0) {
        cerr << "El archivo de usuarios está vacío." << endl;
        archivo.close();
        archivoLF.close();
        return nullptr;
    }

    Usuario* usuarios = new Usuario[cantidad];

    // Reiniciar lectura
    archivo.clear();
    archivo.seekg(0);

    int i = 0;
    string lineaLF;

    while (getline(archivo, linea) && i < cantidad) {
        stringstream ss(linea);
        string nicknameStr, membresiaStr, ciudadStr, paisStr, fechaRegistroStr, passwordStr;
        getline(ss, nicknameStr, ',');
        getline(ss, membresiaStr, ',');
        getline(ss, ciudadStr, ',');
        getline(ss, paisStr, ',');
        getline(ss, fechaRegistroStr, ',');
        getline(ss, passwordStr, ',');

        bool membresiaBool = (membresiaStr == "1");

        usuarios[i].setNickname(nicknameStr);
        usuarios[i].setMembresia(membresiaBool);
        usuarios[i].setCiudad(ciudadStr);
        usuarios[i].setPais(paisStr);
        usuarios[i].setFecha(fechaRegistroStr);
        usuarios[i].setPassword(passwordStr);

        // Si tiene membresía premium, cargar lista
        if (membresiaBool && getline(archivoLF, lineaLF)) {
            stringstream sLF(lineaLF);
            string campo;

            // Primer campo: nickname dueño
            getline(sLF, campo, ',');

            // Contar canciones
            int contador = 0;
            streampos posInicio = sLF.tellg();
            while (getline(sLF, campo, ',')) contador++;

            if (contador > 0) {
                // Volver al inicio de las canciones
                sLF.clear();
                sLF.seekg(posInicio);

                Cancion** _lista = new Cancion*[contador];
                int j = 0;
                while (getline(sLF, campo, ',') && j < contador) {
                    for(int i = 0; i < total; i++){
                        int idCancion = todas[i].getIdAlbum();
                        if (idCancion == stoi(campo)){
                            _lista[j] = &todas[i];
                            j++;
                        }
                    }
                }
                usuarios[i].setListaFavoritos(nicknameStr, _lista, contador);
                delete[] _lista;
            }
        }

        i++;
    }

    archivo.close();
    archivoLF.close();
    cantidad = i;
    return usuarios;
}
