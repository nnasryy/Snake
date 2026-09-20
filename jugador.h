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

    void mostrarInfo() const override; // polimorfismo: versión especializada

private:
    int puntajeMaximo;
    int tiempoMaximo;
    int nivelMaximoAlcanzado;
};

#endif