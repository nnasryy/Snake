#ifndef GESTORARCHIVOS_H
#define GESTORARCHIVOS_H

#include <string>
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
    GestorArchivos(string rutaArchivo);

    // --- CREATE ---
    bool crearJugador(DatosJugador nuevoJugador);
    // --- READ ---
    bool buscarJugadorPorNombre(string nombre, DatosJugador &resultado);
    vector<DatosJugador> obtenerTopJugadores(int cantidad);

    // --- UPDATE ---
    bool actualizarJugador(DatosJugador datosActualizados);

    // --- DELETE ---
    bool eliminarJugador(string nombre);

private:
    string ruta;
    vector<DatosJugador> jugadores;

    void cargarDesdeArchivo();
    void guardarEnArchivo();

    int buscarPosicion(string nombre);
};

#endif // GESTORARCHIVOS_H