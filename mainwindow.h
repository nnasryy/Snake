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
#include <QTransform>

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
    QTimer *timerReloj;
    int segundosTranscurridos;

    QString familiaFuente;   // nombre real de la fuente Pixellari ya cargada
    QLineEdit *campoNombre;  // lo guardamos como miembro para validarlo desde el botón

    Tablero tableroJuego;
    Serpiente serpienteJuego;
    Comida comidaJuego;

    Comida ranaJuego;
    QGraphicsPixmapItem *itemRana;
    bool ranaVisible;
    int contadorRana;
    static const int TICKS_ESPERA_RANA = 40;   // 6 segundos a 150ms por tick
    static const int TICKS_DURACION_RANA = 20; // 3 segundos visible antes de desaparecer
    QWidget *paginaVictoria;
    QWidget *paginaDerrota;
    QLabel *fondoVictoria;
    QLabel *fondoDerrota;
    QLabel *lblManzanasVictoria;
    QLabel *lblVidasVictoria;
    QLabel *lblTiempoVictoria;
    QLabel *lblRazonDerrota;
    QLabel *lblManzanasDerrota;
    QLabel *lblTiempoDerrota;
    QPushButton *btnSiguienteNivel;
    int nivelJugadoActual;
    QTimer *timerJuego;
    QLabel *lblValorPuntos;
    QLabel *lblValorVidas;
    QLabel *lblValorTiempo;
    QWidget *overlayPausa; // pantalla emergente de pausa
    QString spriteCabezaDerecha, spriteCabezaIzquierda;
    QString spriteColaDerecha, spriteColaIzquierda;
    Direccion calcularDireccionEntreNodos(Nodo* desde, Nodo* hacia);
    QString spriteCuerpoNivel;
    QString* cicloColoresNivel = nullptr;
    int cantidadColoresCiclo = 0;

    int origenXCuadricula, origenYCuadricula, tamanoCeldaActual;
    int frutasComidas, segundosRestantes;
    int metaFrutasNivel;
    int vidasRestantes;
    static const int LONGITUD_MINIMA_SEGURA = 3; // si está en esta longitud o menos, la rana quita vida en vez de encoger

    QGraphicsPixmapItem **segmentosVisuales; // arreglo dinámico de sprites de la serpiente
    int cantidadSegmentosVisuales;

    QGraphicsPixmapItem *itemComida;

    GestorArchivos *gestorArchivos;
    DatosJugador jugadorActual;
    QPixmap obtenerSpriteDireccional(QString rutaDerecha, QString rutaIzquierda, Direccion direccion);
    QWidget *paginaNiveles;

    void crearPaginaInicio();
    void crearPaginaUsername();
    void validarNombre();
    void crearPaginaMenuPrincipal();
    void crearPaginaNiveles();
    void crearPaginaJuego();
    void iniciarNivel(int columnas, int filas, int tamanoCelda,int metaFrutas, bool modoInfinito, QString rutaFondo, QString rutaSpriteCabeza);
    void iniciarNivel1();
    void actualizarJuego();
    void redibujarSerpiente();    // reconstruye los sprites según la lista de Nodo
    void mostrarPausa();
    void ocultarPausa();
    void crearPaginaVictoria();
    void crearPaginaDerrota();
    void mostrarVictoria(int nivel, int manzanas, int vidas, int segundos, bool esRecord);
    void mostrarDerrota(int nivel, QString razon, int manzanas, int segundos);

protected:
    void keyPressEvent(QKeyEvent *event) override;
};

#endif // MAINWINDOW_H