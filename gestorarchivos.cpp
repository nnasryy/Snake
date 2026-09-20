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
    std::ifstream archivo(ruta);
    if (!archivo.is_open()) return;

    std::string linea;
    while (std::getline(archivo, linea)) {
        if (linea.empty()) continue; // ignora líneas vacías

        try {
            std::stringstream ss(linea);
            std::string campo, nombreLeido;
            std::string passLeida;

            std::getline(ss, nombreLeido, '|');
            if (nombreLeido.empty()) continue;
            Jugador j(nombreLeido);

            std::getline(ss, campo, '|');
            int puntaje = std::stoi(campo);
            std::getline(ss, campo, '|');
            int tiempo = std::stoi(campo);
            std::getline(ss, campo, '|');
            int nivel = std::stoi(campo);
            std::getline(ss, passLeida, '|');
            j.setContrasena(passLeida);



            j.actualizarRecord(puntaje, tiempo);
            j.desbloquearNivel(nivel);

            jugadores.push_back(j);
        } catch (...) {
            continue; // línea corrupta, la saltamos en vez de tronar la app
        }
    }
    archivo.close();
}

void GestorArchivos::guardarEnArchivo()
{
    std::ofstream archivo(ruta, std::ios::trunc);
    for (const auto &j : jugadores) {
        archivo << j.getNombre() << "|" << j.getPuntajeMaximo() << "|"
                << j.getTiempoMaximo() << "|" << j.getNivelMaximoAlcanzado() << "|"
                << j.getContrasena() << std::endl;
    }
    archivo.close();
}


int GestorArchivos::buscarPosicion(std::string nombre)
{
    for (int i = 0; i < (int)jugadores.size(); i++) {
        if (jugadores[i].getNombre() == nombre) return i;
    }
    return -1;
}

bool GestorArchivos::crearJugador(Jugador nuevoJugador)
{
    if (buscarPosicion(nuevoJugador.getNombre()) != -1) return false;
    jugadores.push_back(nuevoJugador);
    guardarEnArchivo();
    return true;
}

bool GestorArchivos::buscarJugadorPorNombre(std::string nombre, Jugador &resultado)
{
    int pos = buscarPosicion(nombre);
    if (pos == -1) return false;
    resultado = jugadores[pos];
    return true;
}

bool GestorArchivos::actualizarJugador(Jugador datosActualizados)
{
    int pos = buscarPosicion(datosActualizados.getNombre());
    if (pos == -1) return false;
    jugadores[pos] = datosActualizados;
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

std::vector<Jugador> GestorArchivos::obtenerTopJugadores(int cantidad)
{
    std::vector<Jugador> copia = jugadores;
    std::sort(copia.begin(), copia.end(), [](const Jugador &a, const Jugador &b) {
        return a.getPuntajeMaximo() > b.getPuntajeMaximo();
    });
    if ((int)copia.size() > cantidad) copia.resize(cantidad);
    return copia;
}