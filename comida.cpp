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
    // 1. Revisamos que no caiga sobre un muro/bloque (asumimos que 1 = pared en la matriz)
    if (tablero.obtenerValor(fila, columna) == 1) {
        return false;
    }

    // 2. Revisamos que no caiga sobre algún segmento de la serpiente
    Nodo* actual = serpiente.getCabeza();
    while (actual != nullptr) {
        if (actual->y == fila && actual->x == columna) {
            return false;
        }
        actual = actual->siguiente;
    }

    return true; // no chocó con nada, es una posición válida
}

void Comida::generarNuevaPosicion(Tablero &tablero, Serpiente &serpiente)
{
    int columnas = tablero.getColumnas();
    int filas = tablero.getFilas();

    int nuevaX, nuevaY;

    // Probamos posiciones al azar hasta encontrar una libre
    do {
        nuevaX = rand() % columnas;
        nuevaY = rand() % filas;
    } while (!posicionValida(nuevaY, nuevaX, tablero, serpiente));

    x = nuevaX;
    y = nuevaY;

    // 20% de probabilidad de que sea comida especial
    int probabilidad = rand() % 100;
    tipo = (probabilidad < 20) ? ESPECIAL : NORMAL;
}

int Comida::getX() const { return x; }
int Comida::getY() const { return y; }
TipoComida Comida::getTipo() const { return tipo; }