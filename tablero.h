#ifndef TABLERO_H
#define TABLERO_H

class Tablero
{
public:
    Tablero();
    ~Tablero();

    // Prepara la matriz para un nivel nuevo (libera la anterior si existía)
    void configurarNivel(int nuevasColumnas, int nuevasFilas, int nuevoTamanoCelda);

    int obtenerValor(int fila, int columna) const;
    void asignarValor(int fila, int columna, int valor);

    int getColumnas() const;
    int getFilas() const;
    int getTamanoCelda() const;

    bool dentroDelMapa(int fila, int columna) const;

    void generarMurosPerimetro();

private:
    int **mapa;        // matriz dinámica (puntero doble)
    int columnas;
    int filas;
    int tamanoCelda;    // tamaño de cada celda en píxeles

    void liberarMapa(); // libera la matriz actual, si existe
};

#endif // TABLERO_H