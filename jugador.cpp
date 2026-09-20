#include "jugador.h"
#include <iostream>

Jugador::Jugador() : Usuario() // llama al constructor de la clase base
{
    puntajeMaximo = 0;
    tiempoMaximo = 0;
    nivelMaximoAlcanzado = 1;
    mejorNivelCompletado = 0;
}

Jugador::Jugador(std::string nombreInicial) : Usuario(nombreInicial)
{
    puntajeMaximo = 0;
    tiempoMaximo = 0;
    nivelMaximoAlcanzado = 1;
    mejorNivelCompletado = 0;
}

int Jugador::getPuntajeMaximo() const { return puntajeMaximo; }
int Jugador::getTiempoMaximo() const { return tiempoMaximo; }
int Jugador::getNivelMaximoAlcanzado() const { return nivelMaximoAlcanzado; }

void Jugador::actualizarRecord(int frutasNuevas, int tiempoNuevo)
{
    if (frutasNuevas > puntajeMaximo) puntajeMaximo = frutasNuevas;
    if (tiempoNuevo > tiempoMaximo) tiempoMaximo = tiempoNuevo;
}

void Jugador::desbloquearNivel(int nivel)
{
    if (nivel > nivelMaximoAlcanzado) nivelMaximoAlcanzado = nivel;
}

bool Jugador::tieneNivelDesbloqueado(int nivel) const
{
    return nivelMaximoAlcanzado >= nivel;
}

int Jugador::getMejorNivelCompletado() const { return mejorNivelCompletado; }

void Jugador::registrarNivelCompletado(int nivel)
{
    if (nivel > mejorNivelCompletado) mejorNivelCompletado = nivel;
}

bool Jugador::haCompletadoNivel(int nivel) const
{
    return mejorNivelCompletado >= nivel;
}

void Jugador::mostrarInfo() const
{
    std::cout << "Jugador: " << nombre << " | Puntaje: " << puntajeMaximo
              << " | Nivel: " << nivelMaximoAlcanzado << std::endl;
}