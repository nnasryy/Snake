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
    // Si ya había una serpiente de una partida anterior, liberamos todo primero
    Nodo* actual = cabeza;
    while (actual != nullptr) {
        Nodo* siguienteBorrar = actual->siguiente;
        delete actual;
        actual = siguienteBorrar;
    }

    // Creamos el primer nodo (la cabeza) con new
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
    // Bloqueo de giros opuestos instantáneos, tal como pide el PDF
    if (direccionActual == ARRIBA && nuevaDireccion == ABAJO) return;
    if (direccionActual == ABAJO && nuevaDireccion == ARRIBA) return;
    if (direccionActual == IZQUIERDA && nuevaDireccion == DERECHA) return;
    if (direccionActual == DERECHA && nuevaDireccion == IZQUIERDA) return;

    direccionActual = nuevaDireccion;
}

void Serpiente::mover(int columnas, int filas, bool modoInfinito)
{
    if (cabeza == nullptr) return; // seguridad: no hay serpiente que mover

    // Calculamos la nueva posición de la cabeza según la dirección actual
    int nuevaX = cabeza->x;
    int nuevaY = cabeza->y;

    switch (direccionActual) {
    case ARRIBA:     nuevaY -= 1; break;
    case ABAJO:      nuevaY += 1; break;
    case IZQUIERDA:  nuevaX -= 1; break;
    case DERECHA:    nuevaX += 1; break;
    }

    if (modoInfinito) {
        // Nivel 1: si se sale por un lado, aparece en el lado contrario
        nuevaX = (nuevaX + columnas) % columnas;
        nuevaY = (nuevaY + filas) % filas;
    }
    // Si NO es modo infinito (Nivel 2/3), no ajustamos nada aquí:
    // la cabeza puede quedar "fuera" temporalmente, y es el motor
    // del juego quien decide que eso significa game over (ver abajo)

    // Creamos el nuevo nodo-cabeza
    Nodo* nuevaCabeza = new Nodo();
    nuevaCabeza->x = nuevaX;
    nuevaCabeza->y = nuevaY;
    nuevaCabeza->siguiente = cabeza; // la cabeza vieja pasa a ser el segundo segmento

    cabeza = nuevaCabeza;
    longitud++;

    if (creceProximoMovimiento) {
        // La serpiente debía crecer: dejamos el último nodo tal cual (no lo borramos)
        creceProximoMovimiento = false;
    } else {
        // Comportamiento normal: quitamos el último nodo (la cola se mueve)
        if (cabeza->siguiente != nullptr) {
            Nodo* actual = cabeza;
            // Avanzamos hasta el penúltimo nodo
            while (actual->siguiente->siguiente != nullptr) {
                actual = actual->siguiente;
            }
            delete actual->siguiente; // liberamos el último nodo (la cola vieja)
            actual->siguiente = nullptr;
            longitud--;
        }
    }
}

void Serpiente::crecer()
{
    // La próxima vez que se llame mover(), no se borrará la cola
    creceProximoMovimiento = true;
}

bool Serpiente::chocaConsigoMisma() const
{
    if (cabeza == nullptr) return false;

    Nodo* actual = cabeza->siguiente; // empezamos desde el segundo segmento
    while (actual != nullptr) {
        if (actual->x == cabeza->x && actual->y == cabeza->y) {
            return true; // la cabeza coincide con algún segmento del cuerpo
        }
        actual = actual->siguiente;
    }
    return false;
}

Nodo* Serpiente::getCabeza() const { return cabeza; }
int Serpiente::getLongitud() const { return longitud; }

Serpiente::~Serpiente()
{
    // Destructor explícito: liberamos todos los nodos, uno por uno
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
        // Nunca dejamos que la serpiente se quede sin ningún nodo
        if (longitud <= 1) {
            break;
        }

        // Buscamos el penúltimo nodo para eliminar el último
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

bool Serpiente::consumirEscudo()
{
    if (escudoActivo) {
        escudoActivo = false; // se gasta con un solo uso
        return true;          // "sí tenía escudo, ignora este golpe"
    }
    return false; // no tenía escudo, el golpe cuenta normal
}