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
    bool crearJugador(DatosJugador nuevoJugador); // false si el nombre ya existe

    // --- READ ---
    bool buscarJugadorPorNombre(string nombre, DatosJugador &resultado);
    std::vector<DatosJugador> obtenerTopJugadores(int cantidad); // para pantalla de Récords

    // --- UPDATE ---
    bool actualizarJugador(DatosJugador datosActualizados);

    // --- DELETE ---
    bool eliminarJugador(string nombre);

private:
    string ruta;
    vector<DatosJugador> jugadores; // se carga en memoria al construir el gestor

    void cargarDesdeArchivo();  // "Carga Automática", como en tu hoja de Semana 6
    void guardarEnArchivo();    // reescribe TODO el archivo con ios::trunc

    int buscarPosicion(string nombre); // equivalente a buscarContactoPorId
};

#endif // GESTORARCHIVOS_H