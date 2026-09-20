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

            // campo nuevo: mejor nivel completado. Si el archivo es viejo (no existe), se deduce del nivel alcanzado
            int completado = 0;
            if (std::getline(ss, campo, '|') && !campo.empty()) completado = std::stoi(campo);
            if (nivel - 1 > completado) completado = nivel - 1;

            j.actualizarRecord(puntaje, tiempo);
            j.desbloquearNivel(nivel);
            j.registrarNivelCompletado(completado);

            jugadores.push_back(j);
            std::getline(ss, campo, '|');
            int t1 = campo.empty() ? -1 : std::stoi(campo);
            std::getline(ss, campo, '|');
            int t2 = campo.empty() ? -1 : std::stoi(campo);
            std::getline(ss, campo, '|');
            int t3 = campo.empty() ? -1 : std::stoi(campo);
            std::getline(ss, campo, '|');
            int pSafari = campo.empty() ? 0 : std::stoi(campo);

            if (t1 != -1) j.actualizarRecordNivel(1, t1);
            if (t2 != -1) j.actualizarRecordNivel(2, t2);
            if (t3 != -1) j.actualizarRecordNivel(3, t3);
            j.actualizarRecordSafari(pSafari);

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
                << j.getContrasena() << "|" << j.getMejorNivelCompletado() << std::endl;
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

std::vector<Jugador> GestorArchivos::obtenerRankingNivel(int nivel, int cantidad)
{
    std::vector<Jugador> completaron;
    for (const auto &j : jugadores) {
        if (j.haCompletadoNivelConTiempo(nivel)) completaron.push_back(j);
    }
    std::sort(completaron.begin(), completaron.end(), [nivel](const Jugador &a, const Jugador &b) {
        return a.getTiempoNivel(nivel) < b.getTiempoNivel(nivel); // menor tiempo primero
    });
    if ((int)completaron.size() > cantidad) completaron.resize(cantidad);
    return completaron;
}

std::vector<Jugador> GestorArchivos::obtenerRankingSafari(int cantidad)
{
    std::vector<Jugador> conPuntaje;
    for (const auto &j : jugadores) {
        if (j.getPuntajeSafari() > 0) conPuntaje.push_back(j);
    }
    std::sort(conPuntaje.begin(), conPuntaje.end(), [](const Jugador &a, const Jugador &b) {
        return a.getPuntajeSafari() > b.getPuntajeSafari(); // mayor puntaje primero
    });
    if ((int)conPuntaje.size() > cantidad) conPuntaje.resize(cantidad);
    return conPuntaje;
}