#include "gestorarchivos.h"
#include <fstream>
#include <sstream>
#include <algorithm>
using namespace std;

const char DELIMITADOR = '|';

GestorArchivos::GestorArchivos(string rutaArchivo)
{
    ruta = rutaArchivo;
    cargarDesdeArchivo();
}

void GestorArchivos::cargarDesdeArchivo()
{
    ifstream archivo(ruta);
    if (!archivo.is_open()) {
        return;
    }

    string linea;
    while (getline(archivo, linea)) {
        stringstream ss(linea);
        string campo;
        DatosJugador jugador;

        getline(ss, jugador.nombre, DELIMITADOR);

        getline(ss, campo, DELIMITADOR);
        jugador.puntajeMaximo = std::stoi(campo);

        getline(ss, campo, DELIMITADOR);
        jugador.tiempoMaximo = std::stoi(campo);

        getline(ss, campo, DELIMITADOR);
        jugador.nivelMaximoAlcanzado = std::stoi(campo);

        jugadores.push_back(jugador);
    }
    archivo.close();
}

void GestorArchivos::guardarEnArchivo()
{
    ofstream archivo(ruta, std::ios::trunc);

    for (const auto &jugadorActual : jugadores) {
        archivo << jugadorActual.nombre << DELIMITADOR
                << jugadorActual.puntajeMaximo << DELIMITADOR
                << jugadorActual.tiempoMaximo << DELIMITADOR
                << jugadorActual.nivelMaximoAlcanzado << endl;
    }
    archivo.close();
}

int GestorArchivos::buscarPosicion(string nombre)
{
    for (int i = 0; i < (int)jugadores.size(); i++) {
        if (jugadores[i].nombre == nombre) {
            return i;
        }
    }
    return -1;
}

bool GestorArchivos::crearJugador(DatosJugador nuevoJugador)
{

    if (buscarPosicion(nuevoJugador.nombre) != -1) {
        return false;
    }

    jugadores.push_back(nuevoJugador);
    guardarEnArchivo();
    return true;
}

bool GestorArchivos::buscarJugadorPorNombre(string nombre, DatosJugador &resultado)
{
    int posicion = buscarPosicion(nombre);
    if (posicion == -1) {
        return false;
    }
    resultado = jugadores[posicion];
    return true;
}

bool GestorArchivos::actualizarJugador(DatosJugador datosActualizados)
{
    int posicion = buscarPosicion(datosActualizados.nombre);
    if (posicion == -1) {
        return false;
    }

    jugadores[posicion] = datosActualizados;
    guardarEnArchivo();
    return true;
}

bool GestorArchivos::eliminarJugador(string nombre)
{
    int posicion = buscarPosicion(nombre);
    if (posicion == -1) {
        return false;
    }

    jugadores.erase(jugadores.begin() + posicion);
    guardarEnArchivo();
    return true;
}

vector<DatosJugador> GestorArchivos::obtenerTopJugadores(int cantidad)
{
    vector<DatosJugador> copia = jugadores;

    sort(copia.begin(), copia.end(),
              [](const DatosJugador &a, const DatosJugador &b) {
                  return a.puntajeMaximo > b.puntajeMaximo;
              });

    if ((int)copia.size() > cantidad) {
        copia.resize(cantidad);
    }
    return copia;
}