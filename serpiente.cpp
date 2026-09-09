#include "serpiente.h"

Serpiente::Serpiente()
{
    cabeza = nullptr;
    direccionActual = DERECHA;
    longitud = 0;
    creceProximoMovimiento = false;
     escudoActivo = false;
}

void Serpiente::inicializar(int xInicial, int yInicial)
{

    Nodo* actual = cabeza;
    while (actual != nullptr) {
        Nodo* siguienteBorrar = actual->siguiente;
        delete actual;
        actual = siguienteBorrar;
    }

    cabeza = new Nodo();
    cabeza->x = xInicial;
    cabeza->y = yInicial;
    cabeza->siguiente = nullptr;

    longitud = 1;
    direccionActual = DERECHA;
    creceProximoMovimiento = false;
}

void Serpiente::cambiarDireccion(Direccion nuevaDireccion)
{
    if (direccionActual == ARRIBA && nuevaDireccion == ABAJO) return;
    if (direccionActual == ABAJO && nuevaDireccion == ARRIBA) return;
    if (direccionActual == IZQUIERDA && nuevaDireccion == DERECHA) return;
    if (direccionActual == DERECHA && nuevaDireccion == IZQUIERDA) return;

    direccionActual = nuevaDireccion;
}

void Serpiente::mover(int columnas, int filas, bool modoInfinito)
{
    if (cabeza == nullptr) return;

    int nuevaX = cabeza->x;
    int nuevaY = cabeza->y;

    switch (direccionActual) {
    case ARRIBA:     nuevaY -= 1; break;
    case ABAJO:      nuevaY += 1; break;
    case IZQUIERDA:  nuevaX -= 1; break;
    case DERECHA:    nuevaX += 1; break;
    }

    if (modoInfinito) {
        nuevaX = (nuevaX + columnas) % columnas;
        nuevaY = (nuevaY + filas) % filas;
    }

    Nodo* nuevaCabeza = new Nodo();
    nuevaCabeza->x = nuevaX;
    nuevaCabeza->y = nuevaY;
    nuevaCabeza->siguiente = cabeza;

    cabeza = nuevaCabeza;
    longitud++;

    if (creceProximoMovimiento) {

        creceProximoMovimiento = false;
    } else {
        if (cabeza->siguiente != nullptr) {
            Nodo* actual = cabeza;

            while (actual->siguiente->siguiente != nullptr) {
                actual = actual->siguiente;
            }
            delete actual->siguiente;
            actual->siguiente = nullptr;
            longitud--;
        }
    }
}

void Serpiente::crecer()
{
    creceProximoMovimiento = true;
}

bool Serpiente::chocaConsigoMisma() const
{
    if (cabeza == nullptr) return false;

    Nodo* actual = cabeza->siguiente;
    while (actual != nullptr) {
        if (actual->x == cabeza->x && actual->y == cabeza->y) {
            return true;
        }
        actual = actual->siguiente;
    }
    return false;
}

Nodo* Serpiente::getCabeza() const { return cabeza; }
int Serpiente::getLongitud() const { return longitud; }

Serpiente::~Serpiente()
{
    Nodo* actual = cabeza;
    while (actual != nullptr) {
        Nodo* siguienteBorrar = actual->siguiente;
        delete actual;
        actual = siguienteBorrar;
    }
    cabeza = nullptr;
}
void Serpiente::encoger(int cantidad)
{
    for (int i = 0; i < cantidad; i++) {
        if (longitud <= 1) {
            break;
        }
        Nodo* actual = cabeza;
        while (actual->siguiente->siguiente != nullptr) {
            actual = actual->siguiente;
        }
        delete actual->siguiente;
        actual->siguiente = nullptr;
        longitud--;
    }
}

void Serpiente::activarEscudo()
{
    escudoActivo = true;
}
Direccion Serpiente::getDireccion() const {
    return direccionActual;
}
bool Serpiente::consumirEscudo()
{
    if (escudoActivo) {
        escudoActivo = false;
        return true;
    }
    return false;
}