#ifndef MONITOR_H
#define MONITOR_H

#include <iostream>
#include <string>

class Monitor {
private:
    static const int MAX_ENTRADAS = 50;

    static std::string funciones[MAX_ENTRADAS];
    static long iteraciones[MAX_ENTRADAS];
    static int totalFunciones;

    static std::string variables[MAX_ENTRADAS];
    static long memoria[MAX_ENTRADAS];
    static int totalVariables;

public:
    static void registrarIteracion(const std::string& nombreFuncion);
    static void registrarMemoria(const std::string& nombreVariable, long bytes);

    static void mostrarResumen();
    static void reiniciar();
};

#endif
