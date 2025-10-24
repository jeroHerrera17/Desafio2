#include <iostream>
#include "plataforma.h"
#include "usuario.h"
using namespace std;

int main() {
    // ========================================
    // 1. CREAR INSTANCIA DE PLATAFORMA
    // ========================================
    Plataforma plataforma;
    Usuario usuario;

    // ========================================
    // 2. INICIO DE SESIÓN
    // ========================================
    string nickname, clave;
    cout << "=============================\n";
    cout << "      INICIO DE SESION \n";
    cout << "      UDEA TUNES\n";
    cout << "=============================\n\n";
    cout << "Usuario: ";
    cin >> nickname;
    cout << "Clave: ";
    cin >> clave;

    string rutaUsuarios = "../../Datos/usuarios.txt";
    string rutaArtistas = "../../Datos/artistas.txt";
    string rutaAlbumes  = "../../Datos/albumes.txt";
    string rutaCanciones = "../../Datos/canciones.txt";

    if (!plataforma.iniciarSesion(rutaUsuarios, nickname, clave, usuario)) {
        cout << "\nNo se pudo iniciar sesion. Verifica tus datos.\n";
        return 1;
    }

    cout << "\n✓ Sesion iniciada correctamente.\n";
    usuario.mostrarInfo();

    // ========================================
    // 3. CARGAR DATOS EN MEMORIA
    // ========================================
    if (!plataforma.cargarDatos(rutaArtistas, rutaAlbumes, rutaCanciones)) {
        cout << "\nError al cargar los datos.\n";
        return 1;
    }

    // ========================================
    // 4. EJECUTAR LA APLICACIÓN
    // ========================================
    plataforma.ejecutar(usuario);

    return 0;
}
