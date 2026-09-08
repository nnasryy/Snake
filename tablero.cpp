#include "tablero.h"

Tablero::Tablero()
{
    // Al construirse, todavía no hay matriz reservada
    mapa = nullptr;
    columnas = 0;
    filas = 0;
    tamanoCelda = 0;
}

void Tablero::configurarNivel(int nuevasColumnas, int nuevasFilas, int nuevoTamanoCelda)
{
    // Primero liberamos la matriz del nivel anterior (si la había)
    liberarMapa();

    columnas = nuevasColumnas;
    filas = nuevasFilas;
    tamanoCelda = nuevoTamanoCelda;

    // Reservamos un arreglo de punteros: una entrada por cada fila
    mapa = new int*[filas];

    // Por cada fila, reservamos su propio arreglo de columnas
    for (int f = 0; f < filas; f++) {
        mapa[f] = new int[columnas];
    }

    // Inicializamos todas las celdas en 0 (0 = celda vacía)
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
            delete[] mapa[f];   // libera cada fila individual
        }
        delete[] mapa;           // libera el arreglo de punteros
        mapa = nullptr;          // evita que quede un puntero colgante
    }
}

bool Tablero::dentroDelMapa(int fila, int columna) const
{
    return (fila >= 0 && fila < filas && columna >= 0 && columna < columnas);
}

int Tablero::obtenerValor(int fila, int columna) const
{
    if (!dentroDelMapa(fila, columna)) {
        return -1; // valor inválido: fuera de los límites del mapa
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
    liberarMapa(); // destructor explícito, como pide el PDF
}