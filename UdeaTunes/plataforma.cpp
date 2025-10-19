#include "plataforma.h"

Plataforma::Plataforma() {}
Plataforma::~Plataforma() {}

bool Plataforma::iniciarSesion(const string& archivo, const string& nickname, const string& clave, Usuario& usuario) {
    ifstream file(archivo);
    if (!file.is_open()) {
        cerr << "Error: no se pudo abrir el archivo " << archivo << endl;
        return false;
    }

    string linea;
    getline(file, linea); // saltar encabezado

    while (getline(file, linea)) {
        stringstream ss(linea);
        string nick, membresiaStr, ciudad, pais, fecha, claveArchivo;

        getline(ss, nick, ',');
        getline(ss, membresiaStr, ',');
        getline(ss, ciudad, ',');
        getline(ss, pais, ',');
        getline(ss, fecha, ',');
        getline(ss, claveArchivo, ',');

        if (nick == nickname && claveArchivo == clave) {
            usuario.setNickname(nick);
            usuario.setMembresia(stoi(membresiaStr));
            usuario.setCiudad(ciudad);
            usuario.setPais(pais);
            usuario.setFecha(fecha);
            usuario.setClave(claveArchivo);

            file.close();
            cout << "Inicio de sesión exitoso para: " << nick << endl;
            return true;
        }
    }

    cout << "Credenciales incorrectas." << endl;
    file.close();
    return false;
}
