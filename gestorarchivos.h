#ifndef GESTORARCHIVOS_H
#define GESTORARCHIVOS_H

#include <string>
#include "jugador.h"
#include <vector>

using namespace std;

struct DatosJugador
{
    string nombre;
    int puntajeMaximo;
    int tiempoMaximo;
    int nivelMaximoAlcanzado;
};

class GestorArchivos
{
public:
    GestorArchivos(std::string rutaArchivo);
    bool crearJugador(Jugador nuevoJugador);
    bool buscarJugadorPorNombre(std::string nombre, Jugador &resultado);
    std::vector<Jugador> obtenerTopJugadores(int cantidad);
    bool actualizarJugador(Jugador datosActualizados);
    bool eliminarJugador(std::string nombre);

private:
    std::string ruta;
    std::vector<Jugador> jugadores;
    void cargarDesdeArchivo();
    void guardarEnArchivo();
    int buscarPosicion(std::string nombre);
};

#endif // GESTORARCHIVOS_H