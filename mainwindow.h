#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QStackedWidget>
#include <QPushButton>
#include <QLineEdit>
#include <QGraphicsScene>
#include <QGraphicsView>
#include <QGraphicsPixmapItem>
#include <QTimer>
#include <QKeyEvent>
#include <QLabel>
#include "gestorarchivos.h"
#include "tablero.h"
#include "serpiente.h"
#include "comida.h"


class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private:
    QStackedWidget *stack;
    QWidget *paginaInicio;
    QWidget *paginaUsername;
    QWidget *paginaMenuPrincipal;
    QWidget *paginaJuego;
    QGraphicsScene *escenaJuego;
    QGraphicsView *vistaJuego;

    QString familiaFuente;   // nombre real de la fuente Pixellari ya cargada
    QLineEdit *campoNombre;  // lo guardamos como miembro para validarlo desde el botón

    Tablero tableroJuego;
    Serpiente serpienteJuego;
    Comida comidaJuego;

    QTimer *timerJuego;

    QLabel *lblValorPuntos;
    QLabel *lblValorVidas;
    QLabel *lblValorTiempo;

    QWidget *overlayPausa; // pantalla emergente de pausa

    int origenXCuadricula, origenYCuadricula, tamanoCeldaActual;
    int puntosActuales, segundosRestantes;
    int metaFrutasNivel;

    QGraphicsPixmapItem **segmentosVisuales; // arreglo dinámico de sprites de la serpiente
    int cantidadSegmentosVisuales;

    QGraphicsPixmapItem *itemComida;

    GestorArchivos *gestorArchivos;
    DatosJugador jugadorActual;

    void crearPaginaInicio();
    void crearPaginaUsername();
    void validarNombre();
    void crearPaginaMenuPrincipal();

    void crearPaginaJuego();
    void iniciarNivel1();
    void actualizarJuego();       // se llama en cada "tick" del QTimer
    void redibujarSerpiente();    // reconstruye los sprites según la lista de Nodo
    void mostrarPausa();
    void ocultarPausa();

protected:
    void keyPressEvent(QKeyEvent *event) override;
};

#endif // MAINWINDOW_H