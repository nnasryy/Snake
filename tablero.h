#ifndef TABLERO_H
#define TABLERO_H

class Tablero
{
public:
    Tablero();
    ~Tablero();


    void configurarNivel(int nuevasColumnas, int nuevasFilas, int nuevoTamanoCelda);

    int obtenerValor(int fila, int columna) const;
    void asignarValor(int fila, int columna, int valor);

    int getColumnas() const;
    int getFilas() const;
    int getTamanoCelda() const;

    bool dentroDelMapa(int fila, int columna) const;

    void generarMurosPerimetro();

private:
    int **mapa;
    int columnas;
    int filas;
    int tamanoCelda;

    void liberarMapa();
};

#endif // TABLERO_H