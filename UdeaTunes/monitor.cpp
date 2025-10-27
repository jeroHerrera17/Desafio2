#include "monitor.h"

std::string Monitor::funciones[MAX_ENTRADAS];
long Monitor::iteraciones[MAX_ENTRADAS];
int Monitor::totalFunciones = 0;

std::string Monitor::variables[MAX_ENTRADAS];
long Monitor::memoria[MAX_ENTRADAS];
int Monitor::totalVariables = 0;

void Monitor::registrarIteracion(const std::string& nombreFuncion) {
    for (int i = 0; i < totalFunciones; ++i) {
        if (funciones[i] == nombreFuncion) {
            iteraciones[i]++;
            return;
        }
    }
    if (totalFunciones < MAX_ENTRADAS) {
        funciones[totalFunciones] = nombreFuncion;
        iteraciones[totalFunciones] = 1;
        totalFunciones++;
    }
}

void Monitor::registrarMemoria(const std::string& nombreVariable, long bytes) {
    for (int i = 0; i < totalVariables; ++i) {
        if (variables[i] == nombreVariable) {
            memoria[i] = bytes;
            return;
        }
    }
    if (totalVariables < MAX_ENTRADAS) {
        variables[totalVariables] = nombreVariable;
        memoria[totalVariables] = bytes;
        totalVariables++;
    }
}

void Monitor::mostrarResumen() {
    std::cout << "\n===== RESUMEN DEL MONITOR =====\n";
    std::cout << "Funciones registradas: " << totalFunciones << "\n";
    for (int i = 0; i < totalFunciones; ++i) {
        std::cout << " - " << funciones[i] << ": "
                  << iteraciones[i] << " iteraciones\n";
    }

    std::cout << "\nVariables registradas: " << totalVariables << "\n";
    long totalMemoria = 0;
    for (int i = 0; i < totalVariables; ++i) {
        std::cout << " - " << variables[i] << ": "
                  << memoria[i] << " bytes\n";
        totalMemoria += memoria[i];
    }

    std::cout << "\nMemoria total registrada: " << totalMemoria << " bytes\n";
    std::cout << "=================================\n";
}

void Monitor::reiniciar() {
    totalFunciones = 0;
    totalVariables = 0;
}
