
QT -= gui

CONFIG += c++11 console
CONFIG -= app_bundle

# Definiciones adicionales
DEFINES += QT_DEPRECATED_WARNINGS

# Archivos fuente (.cpp)
SOURCES += \
    main.cpp \
    album.cpp \
    artista.cpp \
    cancion.cpp \
    plataforma.cpp \
    listafavoritos.cpp \
    creditos.cpp \
    usuario.cpp \
    publicidad.cpp

# Archivos de cabecera (.h)
HEADERS += \
    album.h \
    artista.h \
    cancion.h \
    plataforma.h \
    listafavoritos.h \
    creditos.h \
    usuario.h \
    publicidad.h
