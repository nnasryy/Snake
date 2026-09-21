#ifndef JUGADOR_H
#define JUGADOR_H

#include <QString>
#include "usuario.h"

class Jugador : public Usuario // herencia pública
{
public:
    Jugador();
    Jugador(std::string nombreInicial);

    int getPuntajeMaximo() const;
    int getTiempoMaximo() const;
    int getNivelMaximoAlcanzado() const;

    void actualizarRecord(int frutasNuevas, int tiempoNuevo); // solo actualiza si supera el record anterior
    void desbloquearNivel(int nivel);
    bool tieneNivelDesbloqueado(int nivel) const;
    void actualizarRecordNivel(int nivel, int tiempo); // guarda si es más rápido que el anterior
    int getTiempoNivel(int nivel) const;
    bool haCompletadoNivelConTiempo(int nivel) const;

    void actualizarRecordSafari(int frutas);
    int getPuntajeSafari() const;

    int getMejorNivelCompletado() const;
    void registrarNivelCompletado(int nivel);
    bool haCompletadoNivel(int nivel) const;
    void registrarSafariCompletado();
    int getCantidadSafarisCompletados() const;
    QString getMedallaSafari() const;


    void mostrarInfo() const override; // polimorfismo

private:
    int puntajeMaximo;
    int tiempoMaximo;
    int nivelMaximoAlcanzado;
    int mejorNivelCompletado; // 0 = ninguno, 1..3 = ultimo nivel ganado
    int tiempoPorNivel[3]; // índice 0,1,2 para nivel 1,2,3
    int puntajeSafari;
    int cantidadSafarisCompletados;
};

#endif