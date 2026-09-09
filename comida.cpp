#include "comida.h"
#include <cstdlib> // rand()

Comida::Comida()
{
    x = 0;
    y = 0;
    tipo = NORMAL;
}

bool Comida::posicionValida(int fila, int columna, Tablero &tablero, Serpiente &serpiente)
{

    if (tablero.obtenerValor(fila, columna) == 1) {
        return false;
    }

    Nodo* actual = serpiente.getCabeza();
    while (actual != nullptr) {
        if (actual->y == fila && actual->x == columna) {
            return false;
        }
        actual = actual->siguiente;
    }

    return true;
}

void Comida::generarNuevaPosicion(Tablero &tablero, Serpiente &serpiente)
{
    int columnas = tablero.getColumnas();
    int filas = tablero.getFilas();

    int nuevaX, nuevaY;


    do {
        nuevaX = rand() % columnas;
        nuevaY = rand() % filas;
    } while (!posicionValida(nuevaY, nuevaX, tablero, serpiente));

    x = nuevaX;
    y = nuevaY;

    tipo = NORMAL;
}

void Comida::generarNuevaPosicionForzada(Tablero &tablero, Serpiente &serpiente, TipoComida tipoForzado)
{
    int columnas = tablero.getColumnas();
    int filas = tablero.getFilas();
    int nuevaX, nuevaY;

    do {
        nuevaX = rand() % columnas;
        nuevaY = rand() % filas;
    } while (!posicionValida(nuevaY, nuevaX, tablero, serpiente));

    x = nuevaX;
    y = nuevaY;
    tipo = tipoForzado;
}

int Comida::getX() const { return x; }
int Comida::getY() const { return y; }
TipoComida Comida::getTipo() const { return tipo; }