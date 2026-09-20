#ifndef BLOQUEMOVIL_H
#define BLOQUEMOVIL_H

#include <QGraphicsPixmapItem>

class BloqueMovil
{
public:
    BloqueMovil();

    void inicializar(int xInicial, int yInicial, int dxInicial, int dyInicial, QGraphicsPixmapItem* spriteAsociado);
    void mover(int limiteColumnas, int limiteFilas); // solo calcula la nueva posición lógica

    int getX() const;
    int getY() const;
    QGraphicsPixmapItem* getSprite() const;

private:
    int x, y;
    int dx, dy; // dirección de movimiento en cada eje (-1 o 1)
    QGraphicsPixmapItem* sprite; // no es dueño de la memoria, solo la referencia
};

#endif