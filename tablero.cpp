#include "tablero.h"

Tablero::Tablero()
{

    mapa = nullptr;
    columnas = 0;
    filas = 0;
    tamanoCelda = 0;
}

void Tablero::configurarNivel(int nuevasColumnas, int nuevasFilas, int nuevoTamanoCelda)
{

    liberarMapa();

    columnas = nuevasColumnas;
    filas = nuevasFilas;
    tamanoCelda = nuevoTamanoCelda;

    mapa = new int*[filas];


    for (int f = 0; f < filas; f++) {
        mapa[f] = new int[columnas];
    }

    for (int f = 0; f < filas; f++) {
        for (int c = 0; c < columnas; c++) {
            mapa[f][c] = 0;
        }
    }
}

void Tablero::liberarMapa()
{
    if (mapa != nullptr) {
        for (int f = 0; f < filas; f++) {
            delete[] mapa[f];
        }
        delete[] mapa;
        mapa = nullptr;
    }
}

bool Tablero::dentroDelMapa(int fila, int columna) const
{
    return (fila >= 0 && fila < filas && columna >= 0 && columna < columnas);
}

int Tablero::obtenerValor(int fila, int columna) const
{
    if (!dentroDelMapa(fila, columna)) {
        return -1;
    }
    return mapa[fila][columna];
}

void Tablero::asignarValor(int fila, int columna, int valor)
{
    if (dentroDelMapa(fila, columna)) {
        mapa[fila][columna] = valor;
    }
}
void Tablero::generarMurosPerimetro()
{
    for (int f = 0; f < filas; f++) {
        for (int c = 0; c < columnas; c++) {
            bool esBorde = (f == 0 || f == filas - 1 || c == 0 || c == columnas - 1);
            mapa[f][c] = esBorde ? 1 : 0;
        }
    }
}

int Tablero::getColumnas() const { return columnas; }
int Tablero::getFilas() const { return filas; }
int Tablero::getTamanoCelda() const { return tamanoCelda; }

Tablero::~Tablero()
{
    liberarMapa();
}