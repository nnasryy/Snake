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
#include <QCheckBox>
#include <QButtonGroup>
#include "gestorarchivos.h"
#include "tablero.h"
#include "serpiente.h"
#include "comida.h"
#include "usuario.h"
#include "jugador.h"
#include <QVBoxLayout>
#include "configuracionnivel.h"

#include "bloquemovil.h"

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
    QLineEdit *campoContrasena;
    QPushButton *btnMostrarPassword;
    QWidget *overlayErrorLogin;
    QLabel *lblErrorLogin;
    void togglePassword();
    void mostrarErrorLogin(QString mensaje);
    QWidget *paginaMenuPrincipal;
    QWidget *paginaJuego;
    QGraphicsScene *escenaJuego;
    QGraphicsView *vistaJuego;
    QTimer *timerReloj;
    int segundosTranscurridos;
    Jugador jugadorActual;

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
    QPushButton *btnNivel2;
    QPushButton *btnNivel3;
    QPushButton *btnSafari;
    void actualizarBotonesNiveles();
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
    bool celdaOcupadaPorSerpiente(int x, int y);
    bool modoInfinitoActual;
    void iniciarNivel2();
    QGraphicsPixmapItem **segmentosVisuales; // arreglo dinámico de sprites de la serpiente
    int cantidadSegmentosVisuales;
    QString rutaCabezaD;
    QString rutaCabezaI;
    QString rutaColaD;
    QString rutaColaI;
    QWidget *paginaSafariSetup;
    QPushButton *btnSkinSafari[4];
    QButtonGroup *grupoSkinSafari;
    QPushButton *btnMurosSafari[2];
    QButtonGroup *grupoMurosSafari;
    QPushButton *btnVidasSafari[2];
    QButtonGroup *grupoVidasSafari;
    QCheckBox *chkPowerUpSafari[4];
    QLabel *lblToastSafari;
    bool modoSafariActivo;
    int intervaloBaseNivel2;
    QString* rutasPowerUpVariantes = nullptr;
    int* tiposPowerUpVariantes = nullptr;
    int cantidadVariantesPowerUp = 0;    // velocidad "real" según el progreso, sin contar el power-up de ratón
    static const int INTERVALO_MINIMO_NIVEL2 = 90; // no dejar que se vuelva injugable
    void iniciarNivel3();
    QWidget *paginaInstruccionesNivel[3];
    QWidget *paginaInstruccionesSafari;

    void crearPaginaInstruccionesNivel(int nivel);
    void crearPaginaInstruccionesSafari();
    QGraphicsPixmapItem *itemComida;

    GestorArchivos *gestorArchivos;
    QPixmap obtenerSpriteDireccional(QString rutaDerecha, QString rutaIzquierda, Direccion direccion);
    QWidget *paginaNiveles;
    QWidget *paginaGanasteSafari;
    QWidget *paginaPerdisteSafari;

    QLabel *lblRazonPerdisteSafari;
    QLabel *lblManzanasPerdisteSafari;
    QLabel *lblVidasPerdisteSafari;
    QLabel *lblTiempoPerdisteSafari;

    void crearPaginaGanasteSafari();
    void crearPaginaPerdisteSafari();
    void mostrarGanasteSafari();
    void mostrarPerdisteSafari(QString razon);
    // --- Opciones ---
    QWidget *paginaOpciones;
    QLineEdit *campoOpcionesUsuario;
    QLineEdit *campoOpcionesPassword;
    QPushButton *btnOpcionesPassword;
    QPushButton *btnSelWasd;
    QPushButton *btnSelFlechas;
    QLabel *lblTeclasWasd;
    QLabel *lblTeclasFlechas;
    QPixmap teclasWasdNormal, teclasWasdGris, teclasFlechasNormal, teclasFlechasGris;
    bool controlesWASD = true;         // true = WASD (predeterminado), false = flechas
    void crearPaginaOpciones();
    void abrirOpciones();
    void actualizarTeclasOpciones();   // pone en gris las teclas que no se están usando
    static QPixmap pixmapEnGris(const QPixmap &original);

    // --- Álbums ---
    QWidget *paginaAlbums;             // pantalla principal con los 3 libros
    QWidget *paginaAlbumNivel[3];      // [0] = álbum nivel 1, [1] = nivel 2, [2] = nivel 3
    QPushButton *btnAlbumNivel[3];
    void crearPaginaAlbums();
    void crearPaginaAlbumNivel(int nivel);
    void actualizarBotonesAlbums();    // cambia cada libro entre con/sin candado
    void abrirAlbum(int nivel);
    bool nivelCompletado(int nivel) const;

    QWidget *paginaRanking;
    QWidget *contenedorFilasRanking;
    QVBoxLayout *layoutFilasRanking;
    QButtonGroup *grupoTabsRanking;
    QPushButton *btnTabRanking[4];
    void crearPaginaRanking();
    void mostrarRanking(int tab);

    void crearPaginaInicio();
    void iniciarNivelConConfiguracion(ConfiguracionNivel &config);
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
    void crearPaginaSafariSetup();
    void iniciarSafari();
    void mostrarToastSafari(QString texto);
    void finalizarSafariPorDerrota();
    void finalizarPartidaPorDerrota(QString razon);
    void mostrarVictoria(int nivel, int manzanas, int vidas, int segundos, bool esRecord);
    void mostrarDerrota(int nivel, QString razon, int manzanas, int segundos);

protected:
    void keyPressEvent(QKeyEvent *event) override;
    bool eventFilter(QObject *obj, QEvent *event) override; // captura teclas de juego sin importar el foco
};

#endif // MAINWINDOW_H