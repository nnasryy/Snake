#ifndef JUGADOR_H
#define JUGADOR_H

#include "usuario.h"

class Jugador : public Usuario // herencia pública
{
public:
    Jugador();
    Jugador(std::string nombreInicial);

    int getPuntajeMaximo() const;
    int getTiempoMaximo() const;
    int getNivelMaximoAlcanzado() const;

    void actualizarRecord(int frutasNuevas, int tiempoNuevo); // solo actualiza si supera el récord anterior
    void desbloquearNivel(int nivel); // solo sube, nunca baja
    bool tieneNivelDesbloqueado(int nivel) const;

    int getMejorNivelCompletado() const;
    void registrarNivelCompletado(int nivel); // solo sube, nunca baja
    bool haCompletadoNivel(int nivel) const;  // true si ya ganó ese nivel al menos una vez

    void mostrarInfo() const override; // polimorfismo: versión especializada

private:
    int puntajeMaximo;
    int tiempoMaximo;
    int nivelMaximoAlcanzado;
    int mejorNivelCompletado; // 0 = ninguno, 1..3 = último nivel ganado
};

#endif