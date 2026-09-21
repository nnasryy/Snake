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

            int completado = 0;
            if (std::getline(ss, campo, '|') && !campo.empty()) completado = std::stoi(campo);
            if (nivel - 1 > completado) completado = nivel - 1;

            j.actualizarRecord(puntaje, tiempo);
            j.desbloquearNivel(nivel);
            j.registrarNivelCompletado(completado);

            std::getline(ss, campo, '|');
            int t1 = campo.empty() ? -1 : std::stoi(campo);
            std::getline(ss, campo, '|');
            int t2 = campo.empty() ? -1 : std::stoi(campo);
            std::getline(ss, campo, '|');
            int t3 = campo.empty() ? -1 : std::stoi(campo);
            std::getline(ss, campo, '|');
            int pSafari = campo.empty() ? 0 : std::stoi(campo);
            std::getline(ss, campo, '|');
            int safarisCompletados = campo.empty() ? 0 : std::stoi(campo);
            for (int i = 0; i < safarisCompletados; i++) j.registrarSafariCompletado();

            if (t1 != -1) j.actualizarRecordNivel(1, t1);
            if (t2 != -1) j.actualizarRecordNivel(2, t2);
            if (t3 != -1) j.actualizarRecordNivel(3, t3);
            j.actualizarRecordSafari(pSafari);

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
        int t1 = j.getTiempoNivel(1);
        int t2 = j.getTiempoNivel(2);
        int t3 = j.getTiempoNivel(3);
        archivo << j.getNombre() << "|" << j.getPuntajeMaximo() << "|"
                << j.getTiempoMaximo() << "|" << j.getNivelMaximoAlcanzado() << "|"
                << j.getContrasena() << "|" << j.getMejorNivelCompletado() << "|"
                << t1 << "|" << t2 << "|" << t3 << "|"
                << j.getPuntajeSafari() << "|" << j.getCantidadSafarisCompletados() << std::endl;

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
    std::vector<Jugador> conMedallas;
    for (const auto &j : jugadores) {
        if (j.getCantidadSafarisCompletados() > 0) conMedallas.push_back(j);
    }
    std::sort(conMedallas.begin(), conMedallas.end(), [](const Jugador &a, const Jugador &b) {
        return a.getCantidadSafarisCompletados() > b.getCantidadSafarisCompletados();
    });
    if ((int)conMedallas.size() > cantidad) conMedallas.resize(cantidad);
    return conMedallas;
}