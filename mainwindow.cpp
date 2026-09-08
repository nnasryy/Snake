#include "mainwindow.h"
#include <QVBoxLayout>
#include <QLabel>
#include <QApplication>
#include <QDebug>
#include <QDirIterator>
#include <QFontDatabase>

using namespace std;

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
{
    QDirIterator it(":/", QDirIterator::Subdirectories);
    while (it.hasNext()) {
        QString ruta = it.next();
        if (ruta.contains("Pixellari", Qt::CaseInsensitive))
            qDebug() << "Encontrado:" << ruta;
    }
    int idFuente = QFontDatabase::addApplicationFont(":/Recursos/Pixellari.ttf");
    if (idFuente != -1) {
        familiaFuente = QFontDatabase::applicationFontFamilies(idFuente).at(0);
    } else {
        qDebug() << "No se pudo cargar Pixellari, usando fuente por defecto";
        familiaFuente = "Arial";
    }
//porque aqui es minuscula?
    gestorArchivos = new GestorArchivos("records.txt");

    stack = new QStackedWidget(this);
    setCentralWidget(stack);

    crearPaginaInicio();
     crearPaginaUsername();
    crearPaginaMenuPrincipal();


    resize(800, 700);
    setWindowTitle("Snake Avanzado");
}

void MainWindow::crearPaginaInicio()
{

    paginaInicio = new QWidget();
    paginaInicio->setFixedSize(800, 700);

    QLabel *fondo = new QLabel(paginaInicio);
    fondo->setPixmap(QPixmap(":/Recursos/SnakeInicio.png"));
    fondo->setGeometry(0, 0, 800, 700);
    fondo->lower();


    QPushButton *btnJugar = new QPushButton(paginaInicio);
    btnJugar->setIcon(QIcon(":/Recursos/PlayButton.png"));
    btnJugar->setIconSize(QSize(294, 124));
    btnJugar->setGeometry(96, 445, 294, 124);
    btnJugar->setFlat(true);
    btnJugar->setStyleSheet("border: none; background: transparent;");

    QPushButton *btnSalir = new QPushButton(paginaInicio);
    btnSalir->setIcon(QIcon(":/Recursos/ExitButton.png"));
    btnSalir->setIconSize(QSize(294, 124));
    btnSalir->setGeometry(432, 445, 294, 124);
    btnSalir->setFlat(true);
    btnSalir->setStyleSheet("border: none; background: transparent;");


    QPushButton *btnVolumen = new QPushButton(paginaInicio);
    btnVolumen->setCheckable(true);
    btnVolumen->setIcon(QIcon(":/Recursos/PlayVolumen.png"));
    btnVolumen->setIconSize(QSize(74, 74));
    btnVolumen->setGeometry(693, 83, 74, 74);
    btnVolumen->setFlat(true);
    btnVolumen->setStyleSheet("border: none; background: transparent;");

    connect(btnVolumen, &QPushButton::toggled, this, [btnVolumen](bool activado){
        if (activado)
            btnVolumen->setIcon(QIcon(":/Recursos/PauseVolumen.png"));
        else
            btnVolumen->setIcon(QIcon(":/Recursos/PlayVolumen.png"));
    });

    connect(btnSalir, &QPushButton::clicked, qApp, &QApplication::quit);
    connect(btnJugar, &QPushButton::clicked, this, [this](){
        stack->setCurrentWidget(paginaUsername);
    });

    stack->addWidget(paginaInicio);
    stack->setCurrentWidget(paginaInicio);
}
void MainWindow::crearPaginaUsername()
{
    paginaUsername = new QWidget();


    QLabel *fondo = new QLabel(paginaUsername);
    fondo->setPixmap(QPixmap(":/Recursos/UsernamePantalla.png"));
    fondo->setGeometry(0, 0, 800, 700);
    fondo->lower();


    campoNombre = new QLineEdit(paginaUsername);
    campoNombre->setGeometry(98, 318, 599, 93);
    campoNombre->setAlignment(Qt::AlignCenter);
    campoNombre->setMaxLength(15); // evita nombres absurdamente largos que rompan tu HUD
    campoNombre->setPlaceholderText("Escribe tu user");
    campoNombre->setStyleSheet(QString(
                                   "QLineEdit {"
                                   "  background-color: rgb(15, 58, 13);"
                                   "  border: 9px solid rgb(143, 208, 53);"
                                   "  color: rgb(143, 208, 53);"
                                   "  font-family: '%1';"
                                   "  font-size: 49px;"
                                   "}"
                                   ).arg(familiaFuente));


    QPushButton *btnSalir = new QPushButton(paginaUsername);
    btnSalir->setIcon(QIcon(":/Recursos/UsernameSalir.png"));
    btnSalir->setIconSize(QSize(278, 70));
    btnSalir->setGeometry(66, 462, 278, 70);
    btnSalir->setFlat(true);
    btnSalir->setStyleSheet("border: none; background: transparent;");

    connect(btnSalir, &QPushButton::clicked, this, [this](){
        stack->setCurrentWidget(paginaInicio);
    });


    QPushButton *btnConfirmar = new QPushButton(paginaUsername);
    btnConfirmar->setIcon(QIcon(":/Recursos/UsernameConfirmar.png"));
    btnConfirmar->setIconSize(QSize(278, 70));
    btnConfirmar->setGeometry(457, 462, 278, 70);
    btnConfirmar->setFlat(true);
    btnConfirmar->setStyleSheet("border: none; background: transparent;");

    connect(btnConfirmar, &QPushButton::clicked, this, &MainWindow::validarNombre);
    connect(campoNombre, &QLineEdit::returnPressed, this, &MainWindow::validarNombre); // Enter también confirma


    QPushButton *btnVolumen = new QPushButton(paginaUsername);
    btnVolumen->setCheckable(true);
    btnVolumen->setIcon(QIcon(":/Recursos/PlayVolumen.png"));
    btnVolumen->setIconSize(QSize(74, 74));
    btnVolumen->setGeometry(693, 83, 74, 74);
    btnVolumen->setFlat(true);
    btnVolumen->setStyleSheet("border: none; background: transparent;");

    connect(btnVolumen, &QPushButton::toggled, this, [btnVolumen](bool activado){
        btnVolumen->setIcon(QIcon(activado ? ":/Recursos/PauseVolumen.png" : ":/Recursos/PlayVolumen.png"));
    });

    stack->addWidget(paginaUsername);
}

void MainWindow::validarNombre()
{
    QString nombreQt = campoNombre->text().trimmed();

    if (nombreQt.isEmpty()) {
        campoNombre->setStyleSheet(QString(
                                       "QLineEdit {"
                                       "  background-color: rgb(15, 58, 13);"
                                       "  border: 9px solid red;"
                                       "  color: red;"
                                       "  font-family: '%1';"
                                       "  font-size: 49px;"
                                       "}"
                                       ).arg(familiaFuente));
        campoNombre->setPlaceholderText("¡Ingresa un nombre!");
        return;
    }

    string nombre = nombreQt.toStdString(); // QString -> std::string, para GestorArchivos

    // READ: ¿este nombre ya existe?
    if (gestorArchivos->buscarJugadorPorNombre(nombre, jugadorActual)) {
        qDebug() << "Bienvenido de vuelta:" << QString::fromStdString(jugadorActual.nombre)
        << "| Puntaje máximo:" << jugadorActual.puntajeMaximo
        << "| Nivel alcanzado:" << jugadorActual.nivelMaximoAlcanzado;
    } else {
        // CREATE: jugador nuevo, con valores en cero
        jugadorActual.nombre = nombre;
        jugadorActual.puntajeMaximo = 0;
        jugadorActual.tiempoMaximo = 0;
        jugadorActual.nivelMaximoAlcanzado = 1; // arranca con Nivel 1 desbloqueado

        gestorArchivos->crearJugador(jugadorActual);
        qDebug() << "Nuevo jugador creado:" << QString::fromStdString(jugadorActual.nombre);
    }

    stack->setCurrentWidget(paginaMenuPrincipal);
}

void MainWindow::crearPaginaMenuPrincipal()
{
    paginaMenuPrincipal = new QWidget();

    QLabel *fondo = new QLabel(paginaMenuPrincipal);
    fondo->setPixmap(QPixmap(":/Recursos/MenuPantalla.png"));
    fondo->setGeometry(0, 0, 800, 700);
    fondo->lower();


    QPushButton *btnJugar = new QPushButton(paginaMenuPrincipal);
    btnJugar->setIcon(QIcon(":/Recursos/MenuJugar.png"));
    btnJugar->setIconSize(QSize(278, 70));
    btnJugar->setGeometry(92, 302, 278, 70);
    btnJugar->setFlat(true);
    btnJugar->setStyleSheet("border: none; background: transparent;");

    connect(btnJugar, &QPushButton::clicked, this, [this](){
        stack->setCurrentWidget(paginaUsername);
    });


    QPushButton *btnInstrucciones = new QPushButton(paginaMenuPrincipal);
    btnInstrucciones->setIcon(QIcon(":/Recursos/MenuInstrucciones.png"));
    btnInstrucciones->setIconSize(QSize(278, 70));
    btnInstrucciones->setGeometry(428, 302, 278, 70);
    btnInstrucciones->setFlat(true);
    btnInstrucciones->setStyleSheet("border: none; background: transparent;");

    connect(btnInstrucciones, &QPushButton::clicked, this, [](){
        qDebug() << "Botón Instrucciones presionado (pantalla pendiente)";
    });


    QPushButton *btnRecords = new QPushButton(paginaMenuPrincipal);
    btnRecords->setIcon(QIcon(":/Recursos/MenuRecords.png"));
    btnRecords->setIconSize(QSize(278, 70));
    btnRecords->setGeometry(92, 390, 278, 70);
    btnRecords->setFlat(true);
    btnRecords->setStyleSheet("border: none; background: transparent;");

    connect(btnRecords, &QPushButton::clicked, this, [](){
        qDebug() << "Botón Récords presionado (pantalla pendiente)";
    });

    QPushButton *btnOpciones = new QPushButton(paginaMenuPrincipal);
    btnOpciones->setIcon(QIcon(":/Recursos/MenuOpciones.png"));
    btnOpciones->setIconSize(QSize(278, 70));
    btnOpciones->setGeometry(428, 390, 278, 70);
    btnOpciones->setFlat(true);
    btnOpciones->setStyleSheet("border: none; background: transparent;");

    connect(btnOpciones, &QPushButton::clicked, this, [](){
        qDebug() << "Botón Opciones presionado (pantalla pendiente)";
    });

    QPushButton *btnAlbum = new QPushButton(paginaMenuPrincipal);
    btnAlbum->setIcon(QIcon(":/Recursos/MenuAlbum.png"));
    btnAlbum->setIconSize(QSize(278, 70));
    btnAlbum->setGeometry(92, 480, 278, 70);
    btnAlbum->setFlat(true);
    btnAlbum->setStyleSheet("border: none; background: transparent;");

    connect(btnAlbum, &QPushButton::clicked, this, [](){
        qDebug() << "Botón Album presionado (pantalla pendiente)";
    });


    QPushButton *btnSalir = new QPushButton(paginaMenuPrincipal);
    btnSalir->setIcon(QIcon(":/Recursos/UsernameSalir.png"));
    btnSalir->setIconSize(QSize(278, 70));
    btnSalir->setGeometry(428, 480, 278, 70);
    btnSalir->setFlat(true);
    btnSalir->setStyleSheet("border: none; background: transparent;");

    connect(btnSalir, &QPushButton::clicked, this, [this](){
        stack->setCurrentWidget(paginaUsername);
    });

    QPushButton *btnVolumen = new QPushButton(paginaMenuPrincipal);
    btnVolumen->setCheckable(true);
    btnVolumen->setIcon(QIcon(":/Recursos/PlayVolumen.png"));
    btnVolumen->setIconSize(QSize(74, 74));
    btnVolumen->setGeometry(693, 83, 74, 74);
    btnVolumen->setFlat(true);
    btnVolumen->setStyleSheet("border: none; background: transparent;");

    connect(btnVolumen, &QPushButton::toggled, this, [btnVolumen](bool activado){
        btnVolumen->setIcon(QIcon(activado ? ":/Recursos/PauseVolumen.png" : ":/Recursos/PlayVolumen.png"));
    });

    stack->addWidget(paginaMenuPrincipal);
}

void MainWindow::crearPaginaJuego()
{
    paginaJuego = new QWidget();

    // --- Escena y vista ---
    escenaJuego = new QGraphicsScene(0, 0, 800, 700, this);
    vistaJuego = new QGraphicsView(escenaJuego, paginaJuego);
    vistaJuego->setGeometry(0, 0, 800, 700);
    vistaJuego->setStyleSheet("border: none;");
    vistaJuego->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    vistaJuego->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);

    // --- HUD (encima de la vista, como widgets normales) ---
    QLabel *lblTituloPuntos = new QLabel("PUNTOS:", paginaJuego);
    lblTituloPuntos->setGeometry(145, 56, 150, 40);
    lblTituloPuntos->setStyleSheet(QString(
                                       "color: rgb(143, 208, 53); font-family: '%1'; font-size: 27px;"
                                       ).arg(familiaFuente));

    lblValorPuntos = new QLabel("0", paginaJuego);
    lblValorPuntos->setGeometry(170, 56, 100, 40);
    lblValorPuntos->setStyleSheet(QString(
                                      "color: rgb(143, 208, 53); font-family: '%1'; font-size: 27px;"
                                      ).arg(familiaFuente));

    lblValorVidas = new QLabel("3", paginaJuego);
    lblValorVidas->setGeometry(311, 55, 60, 40);
    lblValorVidas->setStyleSheet(QString(
                                     "color: rgb(143, 208, 53); font-family: '%1'; font-size: 31px;"
                                     ).arg(familiaFuente));

    lblValorTiempo = new QLabel("00:00", paginaJuego);
    lblValorTiempo->setGeometry(639, 59, 100, 40);
    lblValorTiempo->setStyleSheet(QString(
                                      "color: rgb(143, 208, 53); font-family: '%1'; font-size: 26px;"
                                      ).arg(familiaFuente));

    QPushButton *btnPausa = new QPushButton(paginaJuego);
    btnPausa->setIcon(QIcon(":/Recursos/PauseVolumen.png")); // ícono de pausa, no el de volumen
    btnPausa->setIconSize(QSize(40, 40));
    btnPausa->setGeometry(735, 53, 40, 40);
    btnPausa->setFlat(true);
    btnPausa->setStyleSheet("border: none; background: transparent;");
    connect(btnPausa, &QPushButton::clicked, this, &MainWindow::mostrarPausa);

    // --- Overlay de pausa (oculto por defecto) ---
    overlayPausa = new QWidget(paginaJuego);
    overlayPausa->setGeometry(200, 250, 400, 200);
    overlayPausa->setStyleSheet("background-color: rgba(15, 58, 13, 230); border: 5px solid rgb(143, 208, 53);");
    overlayPausa->setVisible(false);

    QPushButton *btnReanudar = new QPushButton("Reanudar", overlayPausa);
    btnReanudar->setGeometry(50, 50, 300, 50);
    connect(btnReanudar, &QPushButton::clicked, this, &MainWindow::ocultarPausa);

    QPushButton *btnVolverMenu = new QPushButton("Volver al menú", overlayPausa);
    btnVolverMenu->setGeometry(50, 120, 300, 50);
    connect(btnVolverMenu, &QPushButton::clicked, this, [this](){
        timerJuego->stop();
        ocultarPausa();
        stack->setCurrentWidget(paginaMenuPrincipal);
    });

    // --- Timer del juego ---
    timerJuego = new QTimer(this);
    connect(timerJuego, &QTimer::timeout, this, &MainWindow::actualizarJuego);

    stack->addWidget(paginaJuego);
}

void MainWindow::iniciarNivel1()
{
    // --- Configurar datos del nivel ---
    origenXCuadricula = 49;
    origenYCuadricula = 115;
    tamanoCeldaActual = 50;
    metaFrutasNivel = 10;
    puntosActuales = 0;
    segundosRestantes = 0;

    tableroJuego.configurarNivel(14, 10, 50); // sin muros, Nivel 1 es infinito

    serpienteJuego.inicializar(3, 5); // posición inicial dentro de la matriz 14x10
    comidaJuego.generarNuevaPosicion(tableroJuego, serpienteJuego);

    // --- Limpiar la escena de cualquier partida anterior ---
    escenaJuego->clear();

    // --- Fondo del nivel ---
    QGraphicsPixmapItem *fondo = escenaJuego->addPixmap(QPixmap(":/Recursos/nivel1background.png"));
    fondo->setPos(0, 0);
    fondo->setZValue(-1);

    // --- Comida ---
    itemComida = escenaJuego->addPixmap(QPixmap(":/Recursos/rana.png")); // ajusta el nombre real
    itemComida->setPos(
        origenXCuadricula + comidaJuego.getX() * tamanoCeldaActual,
        origenYCuadricula + comidaJuego.getY() * tamanoCeldaActual
        );

    // --- Serpiente: arrancamos con arreglo dinámico vacío, se llena en redibujarSerpiente() ---
    segmentosVisuales = nullptr;
    cantidadSegmentosVisuales = 0;
    redibujarSerpiente();

    timerJuego->start(150); // 150ms, velocidad constante del Nivel 1
    vistaJuego->setFocus(); // para que capture el teclado de inmediato

    stack->setCurrentWidget(paginaJuego);
}

void MainWindow::redibujarSerpiente()
{
    // Liberamos los sprites del tick anterior
    if (segmentosVisuales != nullptr) {
        for (int i = 0; i < cantidadSegmentosVisuales; i++) {
            escenaJuego->removeItem(segmentosVisuales[i]);
            delete segmentosVisuales[i];
        }
        delete[] segmentosVisuales;
    }

    int longitudActual = serpienteJuego.getLongitud();
    segmentosVisuales = new QGraphicsPixmapItem*[longitudActual];
    cantidadSegmentosVisuales = longitudActual;

    Nodo* actual = serpienteJuego.getCabeza();
    int indice = 0;

    // Patrón de colores: cabeza, luego negro-amarillo-naranja repitiendo
    while (actual != nullptr) {
        QString rutaSprite;

        if (indice == 0) {
            rutaSprite = ":/Recursos/BoaHead.png";
        } else if (actual->siguiente == nullptr) {
            rutaSprite = ":/Recursos/BoaCola.png";
        } else {
            int patron = (indice - 1) % 3;
            if (patron == 0) rutaSprite = ":/Recursos/NodoNegro.png";
            else if (patron == 1) rutaSprite = ":/Recursos/NodoAmarillo.png";
            else rutaSprite = ":/Recursos/NodoNaranja.png";
        }

        QGraphicsPixmapItem *sprite = escenaJuego->addPixmap(QPixmap(rutaSprite));
        sprite->setPos(
            origenXCuadricula + actual->x * tamanoCeldaActual,
            origenYCuadricula + actual->y * tamanoCeldaActual
            );

        segmentosVisuales[indice] = sprite;
        indice++;
        actual = actual->siguiente;
    }
}

//captura del teclado
void MainWindow::keyPressEvent(QKeyEvent *event)
{
    if (stack->currentWidget() != paginaJuego) {
        QMainWindow::keyPressEvent(event);
        return;
    }

    switch (event->key()) {
    case Qt::Key_Up:    case Qt::Key_W: serpienteJuego.cambiarDireccion(ARRIBA); break;
    case Qt::Key_Down:  case Qt::Key_S: serpienteJuego.cambiarDireccion(ABAJO); break;
    case Qt::Key_Left:  case Qt::Key_A: serpienteJuego.cambiarDireccion(IZQUIERDA); break;
    case Qt::Key_Right: case Qt::Key_D: serpienteJuego.cambiarDireccion(DERECHA); break;
    default: QMainWindow::keyPressEvent(event); break;
    }
}

void MainWindow::actualizarJuego()
{
    serpienteJuego.mover(tableroJuego.getColumnas(), tableroJuego.getFilas(), true); // true = Nivel 1 infinito

    Nodo* cabeza = serpienteJuego.getCabeza();

    if (cabeza->x == comidaJuego.getX() && cabeza->y == comidaJuego.getY()) {
        serpienteJuego.crecer();
        puntosActuales += 10;
        lblValorPuntos->setText(QString::number(puntosActuales));
        comidaJuego.generarNuevaPosicion(tableroJuego, serpienteJuego);

        itemComida->setPos(
            origenXCuadricula + comidaJuego.getX() * tamanoCeldaActual,
            origenYCuadricula + comidaJuego.getY() * tamanoCeldaActual
            );
    }

    if (serpienteJuego.chocaConsigoMisma()) {
        timerJuego->stop();
        qDebug() << "Game Over: chocó consigo misma";
        // más adelante: stack->setCurrentWidget(paginaGameOver);
    }

    redibujarSerpiente();
}

void MainWindow::mostrarPausa()
{
    timerJuego->stop();
    overlayPausa->setVisible(true);
    overlayPausa->raise();
}

void MainWindow::ocultarPausa()
{
    overlayPausa->setVisible(false);
    timerJuego->start();
}

MainWindow::~MainWindow()
{
    delete gestorArchivos;
}