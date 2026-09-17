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
#include <QString>
#include <QTransform>
#include <algorithm>
#include "gestorarchivos.h"
#include "tablero.h"
#include "serpiente.h"
#include "comida.h"

struct BloqueMovil {
    int x, y;
    int dx, dy; // dirección de movimiento: -1, 0 o 1 en cada eje
    QGraphicsPixmapItem* sprite;
};

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
    static const int CANTIDAD_BLOQUES_MOVILES = 3;
    BloqueMovil bloquesMoviles[CANTIDAD_BLOQUES_MOVILES];
    int contadorMovimientoBloques;
    static const int TICKS_MOVIMIENTO_BLOQUE = 18; // se mueven cada 5 ticks (más lento que la serpiente, para que sea justo)
    void inicializarBloquesMoviles();
    void moverBloquesMoviles();
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
    QPushButton *btnPausaJuego;       // el de arriba a la derecha
    QPushButton *btnToggleMusica;     // dentro del overlay de pausa
    QString rutaIconoPausa;           // ícono del botón de pausa, cambia por nivel
    QString rutaVolumenPlay, rutaVolumenPause; // íconos del toggle, cambian por nivel
    void actualizarIconosPausa(QString iconoPausa, QString volumenPlay, QString volumenPause);
    int origenXCuadricula, origenYCuadricula, tamanoCeldaActual;
    int frutasComidas, segundosRestantes;
    int metaFrutasNivel;
    int vidasRestantes;
    static const int LONGITUD_MINIMA_SEGURA = 3; // si está en esta longitud o menos, la rana quita vida en vez de encoger
    Comida powerUpJuego;
    QGraphicsPixmapItem *itemPowerUp;
    bool powerUpVisible;
    int contadorPowerUp;
    QString rutaSpritePowerUp;
    int tipoEfectoPowerUp; // 0 = encoger (rana), 1 = ralentizar (ratón), 2 = escudo (pez)
    int intervaloOriginalJuego;
    int contadorRalentizado;
    bool ralentizadoActivo;
    bool modoInfinitoActual;
    void iniciarNivel2();
    QGraphicsPixmapItem **segmentosVisuales; // arreglo dinámico de sprites de la serpiente
    int cantidadSegmentosVisuales;
    QString rutaCabezaD;
    QString rutaCabezaI;
    QString rutaColaD;
    QString rutaColaI;
    int intervaloBaseNivel2;      // velocidad "real" según el progreso, sin contar el power-up de ratón
    static const int INTERVALO_MINIMO_NIVEL2 = 90; // no dejar que se vuelva injugable
    void iniciarNivel3();


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
    void dibujarGridPermanente(QColor colorLinea);
    void dibujarMuros(QString rutaSpriteMuro);
    void iniciarNivel(int columnas, int filas, int tamanoCelda,int metaFrutas, bool modoInfinito, QString rutaFondo, QString rutaSpriteCabeza);
    void iniciarNivel1();
    void actualizarJuego();
    void redibujarSerpiente();    // reconstruye los sprites según la lista de Nodo
    void mostrarPausa();
    void ocultarPausa();
    void crearPaginaVictoria();
    void crearPaginaDerrota();
    void finalizarPartidaPorDerrota(QString razon);
    void mostrarVictoria(int nivel, int manzanas, int vidas, int segundos, bool esRecord);
    void mostrarDerrota(int nivel, QString razon, int manzanas, int segundos);

protected:
    void keyPressEvent(QKeyEvent *event) override;
};

#endif // MAINWINDOW_H