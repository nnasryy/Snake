#include "gestorarchivos.h"
#include <fstream>
#include <sstream>
#include <algorithm>
using namespace std;

const char DELIMITADOR = '|'; // misma práctica que tu hoja: evita que espacios rompan el archivo

GestorArchivos::GestorArchivos(string rutaArchivo)
{
    ruta = rutaArchivo;
    cargarDesdeArchivo(); // Carga Automática al iniciar, como en el ejemplo de clase
}

void GestorArchivos::cargarDesdeArchivo()
{
    ifstream archivo(ruta);
    if (!archivo.is_open()) {
        return; // primera vez que corre el juego, el archivo aún no existe
    }

    string linea;
    while (getline(archivo, linea)) {
        stringstream ss(linea);
        string campo;
        DatosJugador jugador;

        getline(ss, jugador.nombre, DELIMITADOR);

        getline(ss, campo, DELIMITADOR);
        jugador.puntajeMaximo = std::stoi(campo); // string → int, igual que tu hoja

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
    // ios::trunc: borra el contenido anterior y lo reescribe desde cero,
    // así el archivo siempre queda igual al estado actual del vector
    ofstream archivo(ruta, std::ios::trunc);

    for (const auto &jugadorActual : jugadores) { // range-based for, evita copias
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
            return i; // encontrado, regresamos su posición
        }
    }
    return -1; // no encontrado
}

bool GestorArchivos::crearJugador(DatosJugador nuevoJugador)
{
    // Validación de Unicidad: no permitir nombres duplicados
    if (buscarPosicion(nuevoJugador.nombre) != -1) {
        return false; // ya existe, no se crea de nuevo
    }

    jugadores.push_back(nuevoJugador);
    guardarEnArchivo(); // sincroniza con disco inmediatamente
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
        return false; // no existe, no hay nada que actualizar
    }

    jugadores[posicion] = datosActualizados;
    guardarEnArchivo(); // sincroniza el cambio con el disco
    return true;
}

bool GestorArchivos::eliminarJugador(string nombre)
{
    int posicion = buscarPosicion(nombre);
    if (posicion == -1) {
        return false;
    }

    jugadores.erase(jugadores.begin() + posicion); // elimina y recorre lo siguiente
    guardarEnArchivo();
    return true;
}

vector<DatosJugador> GestorArchivos::obtenerTopJugadores(int cantidad)
{
    vector<DatosJugador> copia = jugadores; // copiamos para no alterar el orden original

    // Mismo patrón de sort + lambda que tu hoja de Semana 7
    sort(copia.begin(), copia.end(),
              [](const DatosJugador &a, const DatosJugador &b) {
                  return a.puntajeMaximo > b.puntajeMaximo; // mayor puntaje primero
              });

    if ((int)copia.size() > cantidad) {
        copia.resize(cantidad); // solo nos quedamos con los primeros N
    }
    return copia;
}