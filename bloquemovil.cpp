#include "bloquemovil.h"

BloqueMovil::BloqueMovil()
{
    x = 0; y = 0; dx = 1; dy = 1;
    sprite = nullptr;
}

void BloqueMovil::inicializar(int xInicial, int yInicial, int dxInicial, int dyInicial, QGraphicsPixmapItem* spriteAsociado)
{
    x = xInicial;
    y = yInicial;
    dx = dxInicial;
    dy = dyInicial;
    sprite = spriteAsociado;
}

void BloqueMovil::mover(int limiteColumnas, int limiteFilas)
{
    int nuevoX = x + dx;
    int nuevoY = y + dy;

    if (nuevoX <= 1 || nuevoX >= limiteColumnas - 2) {
        dx *= -1;
        nuevoX = x + dx;
    }
    if (nuevoY <= 1 || nuevoY >= limiteFilas - 2) {
        dy *= -1;
        nuevoY = y + dy;
    }

    x = nuevoX;
    y = nuevoY;
}

int BloqueMovil::getX() const { return x; }
int BloqueMovil::getY() const { return y; }
QGraphicsPixmapItem* BloqueMovil::getSprite() const { return sprite; }