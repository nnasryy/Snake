#ifndef NODO_H
#define NODO_H

// Representa un solo segmento del cuerpo de la serpiente
struct Nodo
{
    int x;          // columna dentro de la matriz del Tablero
    int y;          // fila dentro de la matriz del Tablero
    Nodo* siguiente; // apunta al próximo segmento (o nullptr si es el último)
};

#endif // NODO_H