#ifndef SERPIENTE_H
#define SERPIENTE_H

#include "nodo.h"

enum Direccion { ARRIBA, ABAJO, IZQUIERDA, DERECHA };

class Serpiente
{
public:
    Serpiente();
    ~Serpiente();
    //Cambios de Direccion/Control de la Serpiente
    void inicializar(int xInicial, int yInicial); // crea la cabeza en una posición
    void mover(int columnas, int filas, bool modoInfinito);                                  // avanza un paso en la dirección actual
    void crecer();                                 // agrega un segmento nuevo tras comer
    bool chocaConsigoMisma() const;                // revisa auto-colisión
    void cambiarDireccion(Direccion nuevaDireccion);

    void encoger(int cantidad);          // quita segmentos del final (rana)
    Direccion getDireccion() const;    // revisa y gasta el escudo si estaba activo
    Nodo* getCabeza() const;
    int getLongitud() const;

private:
    Nodo* cabeza;
    Direccion direccionActual;
    int longitud;
    bool creceProximoMovimiento; // controla si el próximo mover() debe agregar segmento

};

#endif // SERPIENTE_H