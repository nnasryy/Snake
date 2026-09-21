#ifndef SERPIENTE_H
#define SERPIENTE_H

#include "nodo.h"

enum Direccion { ARRIBA, ABAJO, IZQUIERDA, DERECHA };

class Serpiente
{
public:
    Serpiente();
    ~Serpiente();
    void inicializar(int xInicial, int yInicial);
    void mover(int columnas, int filas, bool modoInfinito);
    void crecer();
    bool chocaConsigoMisma() const;
    void cambiarDireccion(Direccion nuevaDireccion);
    void activarEscudo();
    bool consumirEscudo();
    void encoger(int cantidad);
    Direccion getDireccion() const;
    Nodo* getCabeza() const;
    int getLongitud() const;
    bool escudoActivo;

private:
    Nodo* cabeza;
    Direccion direccionActual;
    int longitud;
    bool creceProximoMovimiento;

};

#endif // SERPIENTE_H