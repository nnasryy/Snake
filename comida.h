#ifndef COMIDA_H
#define COMIDA_H

#include "tablero.h"
#include "serpiente.h"

enum TipoComida { NORMAL, ESPECIAL };

class Comida
{
public:
    Comida();

    // Busca una posición válida (sin chocar con muros ni con el cuerpo)
    void generarNuevaPosicion(Tablero &tablero, Serpiente &serpiente);
    void generarNuevaPosicionForzada(Tablero &tablero, Serpiente &serpiente, TipoComida tipoForzado);

    int getX() const;
    int getY() const;
    TipoComida getTipo() const;

private:
    int x;
    int y;
    TipoComida tipo;

    bool posicionValida(int fila, int columna, Tablero &tablero, Serpiente &serpiente);
};

#endif // COMIDA_H