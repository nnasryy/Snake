#include "mainwindow.h"
#include <QVBoxLayout>
#include <QLabel>
#include <QApplication>
#include <QDebug>
#include <QDirIterator>
#include <QFontDatabase>
#include <QTransform>
#include <algorithm>
const int MainWindow::INTERVALO_MINIMO_NIVEL2;

using namespace std;

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
{
    cicloColoresNivel = nullptr;
    cantidadColoresCiclo = 0;

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
    crearPaginaNiveles();
    crearPaginaJuego();
    crearPaginaVictoria();
    crearPaginaDerrota();

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
        stack->setCurrentWidget(paginaNiveles);
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

    lblValorPuntos = new QLabel("0", paginaJuego);
    lblValorPuntos->setGeometry(131, 59, 100, 40);
    lblValorPuntos->setStyleSheet(QString(
                                      "color: rgb(143, 208, 53); font-family: '%1'; font-size: 27px;"
                                      ).arg(familiaFuente));

    lblValorPuntos->setText("0/" + QString::number(metaFrutasNivel));
    lblValorVidas = new QLabel("3", paginaJuego);
    lblValorVidas->setGeometry(306, 59, 60, 40);
    lblValorVidas->setStyleSheet(QString(
                                     "color: rgb(143, 208, 53); font-family: '%1'; font-size: 31px;"
                                     ).arg(familiaFuente));

    lblValorTiempo = new QLabel("00:00", paginaJuego);
    lblValorTiempo->setGeometry(665, 59, 100, 40);
    lblValorTiempo->setStyleSheet(QString(
                                      "color: rgb(143, 208, 53); font-family: '%1'; font-size: 26px;"
                                      ).arg(familiaFuente));

    btnPausaJuego = new QPushButton(paginaJuego);
    btnPausaJuego->setIcon(QIcon(":/Recursos/PauseVolumen.png"));
    btnPausaJuego->setIconSize(QSize(40, 40));
    btnPausaJuego->setGeometry(740, 45, 40, 40);
    btnPausaJuego->setFlat(true);
    btnPausaJuego->setStyleSheet("border: none; background: transparent;");
    connect(btnPausaJuego, &QPushButton::clicked, this, &MainWindow::mostrarPausa);
    // --- Overlay de pausa (oculto por defecto) ---
    overlayPausa = new QWidget(paginaJuego);
    overlayPausa->setGeometry(200, 200, 400, 260);
    overlayPausa->setStyleSheet("background-color: rgba(15, 58, 13, 230); border: 5px solid rgb(143, 208, 53);");
    overlayPausa->setVisible(false);

    btnToggleMusica = new QPushButton(overlayPausa);
    btnToggleMusica->setCheckable(true);
    btnToggleMusica->setIconSize(QSize(45, 45));
    btnToggleMusica->setGeometry(45, 5, 45, 45); // esquina superior derecha del overlay
    btnToggleMusica->setFlat(true);
    btnToggleMusica->setStyleSheet("border: none; background: transparent;");
    btnToggleMusica->setIcon(QIcon(":/Recursos/PlayVolumenLvl2.png"));
    connect(btnToggleMusica, &QPushButton::toggled, this, [this](bool activado){
        btnToggleMusica->setIcon(QIcon(activado ? rutaVolumenPause : rutaVolumenPlay));
    });

    QPushButton *btnReanudar = new QPushButton(overlayPausa);
    btnReanudar->setIcon(QIcon(":/Recursos/SeguirJugandoPJ.png"));
    btnReanudar->setIconSize(QSize(300, 100));
    btnReanudar->setGeometry(50, 30, 300, 100);
    btnReanudar->setFlat(true);
    btnReanudar->setStyleSheet("border: none; background: transparent;");
    connect(btnReanudar, &QPushButton::clicked, this, &MainWindow::ocultarPausa);

    QPushButton *btnVolverMenu = new QPushButton(overlayPausa);
    btnVolverMenu->setIcon(QIcon(":/Recursos/VolverAMenuPJ.png"));
    btnVolverMenu->setIconSize(QSize(300, 100));
    btnVolverMenu->setGeometry(50, 140, 300, 100);
    btnVolverMenu->setFlat(true);
    btnVolverMenu->setStyleSheet("border: none; background: transparent;");
    connect(btnVolverMenu, &QPushButton::clicked, this, [this](){
        timerJuego->stop();
        timerReloj->stop();
        ocultarPausa();
        stack->setCurrentWidget(paginaMenuPrincipal);
    });
    // --- Timer del juego ---
    timerJuego = new QTimer(this);
    connect(timerJuego, &QTimer::timeout, this, &MainWindow::actualizarJuego);
    timerReloj = new QTimer(this);
    timerReloj->setInterval(1000); // exactamente 1 segundo, sin importar la velocidad del juego
    connect(timerReloj, &QTimer::timeout, this, [this](){
        segundosTranscurridos++;
        int minutos = segundosTranscurridos / 60;
        int segundos = segundosTranscurridos % 60;
        lblValorTiempo->setText(QString("%1:%2")
                                    .arg(minutos, 2, 10, QChar('0'))
                                    .arg(segundos, 2, 10, QChar('0')));
    });


    stack->addWidget(paginaJuego);
}

void MainWindow::dibujarGridPermanente(QColor colorLinea)
{
    QPen lapizGrid(colorLinea);
    lapizGrid.setWidth(1);

    for (int col = 0; col <= tableroJuego.getColumnas(); col++) {
        int x = origenXCuadricula + col * tamanoCeldaActual;
        escenaJuego->addLine(x, origenYCuadricula, x, origenYCuadricula + tableroJuego.getFilas() * tamanoCeldaActual, lapizGrid);
    }
    for (int fila = 0; fila <= tableroJuego.getFilas(); fila++) {
        int y = origenYCuadricula + fila * tamanoCeldaActual;
        escenaJuego->addLine(origenXCuadricula, y, origenXCuadricula + tableroJuego.getColumnas() * tamanoCeldaActual, y, lapizGrid);
    }
}

void MainWindow::actualizarIconosPausa(QString iconoPausa, QString volumenPlay, QString volumenPause)
{
    rutaIconoPausa = iconoPausa;
    rutaVolumenPlay = volumenPlay;
    rutaVolumenPause = volumenPause;

    btnPausaJuego->setIcon(QIcon(rutaIconoPausa));
    btnToggleMusica->setIcon(QIcon(btnToggleMusica->isChecked() ? rutaVolumenPause : rutaVolumenPlay));
}


void MainWindow::iniciarNivel(int columnas, int filas, int tamanoCelda,
                              int metaFrutas, bool modoInfinito,
                              QString rutaFondo, QString rutaSpriteCabeza)
{
    origenXCuadricula = 49;  // ajusta si cambia por nivel
    origenYCuadricula = 115; // ajusta si cambia por nivel
    tamanoCeldaActual = tamanoCelda;
    metaFrutasNivel = metaFrutas;
    frutasComidas = 0;

    tableroJuego.configurarNivel(columnas, filas, tamanoCelda);
    if (!modoInfinito) {
        tableroJuego.generarMurosPerimetro();
    }

    serpienteJuego.inicializar(columnas / 2, filas / 2); // centrado, más seguro que fijo en (3,5)
    comidaJuego.generarNuevaPosicion(tableroJuego, serpienteJuego);

    escenaJuego->clear();

    QGraphicsPixmapItem *fondo = escenaJuego->addPixmap(QPixmap(rutaFondo));
    fondo->setPos(0, 0);
    fondo->setZValue(-1);

    itemComida = escenaJuego->addPixmap(QPixmap(":/Recursos/Manzana.png"));
    itemComida->setZValue(1); // siempre por encima de la serpiente (zValue 0 por defecto)
    itemComida->setPos(
        origenXCuadricula + comidaJuego.getX() * tamanoCeldaActual,
        origenYCuadricula + comidaJuego.getY() * tamanoCeldaActual
        );

    segmentosVisuales = nullptr;
    cantidadSegmentosVisuales = 0;
    redibujarSerpiente();

    lblValorPuntos->setText("0/" + QString::number(metaFrutasNivel));

    int intervalo = modoInfinito ? 150 : 100; // ajusta según velocidad real de cada nivel
    timerJuego->start(intervalo);
    vistaJuego->setFocus();

    stack->setCurrentWidget(paginaJuego);
}

void MainWindow::redibujarSerpiente()
{
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
    Nodo* anterior = nullptr; // necesario para calcular dirección de la cola
    int indice = 0;

    while (actual != nullptr) {
        QPixmap pixmapSprite;

        if (indice == 0) {
            Direccion dirCabeza = serpienteJuego.getDireccion();
            // Antes: obtenerSpriteDireccional(":/Recursos/BoaHead.png", ":/Recursos/BoaHeadLeft.png", dirCabeza);
            pixmapSprite = obtenerSpriteDireccional(rutaCabezaD, rutaCabezaI, dirCabeza);
        } else if (actual->siguiente == nullptr) {
            // Cola: dirección calculada entre el nodo anterior y este
            Direccion dirCola = calcularDireccionEntreNodos(anterior, actual);
            pixmapSprite = obtenerSpriteDireccional(rutaColaD, rutaColaI, dirCola);

        } else {
            int patron = (indice - 1) % cantidadColoresCiclo;
            pixmapSprite = QPixmap(cicloColoresNivel[patron]);
        }

        // Escala al tamaño de celda actual (por si la rotación cambia dimensiones)
        pixmapSprite = pixmapSprite.scaled(tamanoCeldaActual, tamanoCeldaActual,
                                           Qt::KeepAspectRatio, Qt::SmoothTransformation);

        QGraphicsPixmapItem *sprite = escenaJuego->addPixmap(pixmapSprite);
        sprite->setPos(
            origenXCuadricula + actual->x * tamanoCeldaActual,
            origenYCuadricula + actual->y * tamanoCeldaActual
            );

        segmentosVisuales[indice] = sprite;
        indice++;
        anterior = actual;
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
    serpienteJuego.mover(tableroJuego.getColumnas(), tableroJuego.getFilas(), modoInfinitoActual);
    if (nivelJugadoActual == 3) {
        contadorMovimientoBloques++;
        if (contadorMovimientoBloques >= TICKS_MOVIMIENTO_BLOQUE) {
            moverBloquesMoviles();
            contadorMovimientoBloques = 0;
        }
    }

    Nodo* cabeza = serpienteJuego.getCabeza();

    if (cabeza->x == comidaJuego.getX() && cabeza->y == comidaJuego.getY()) {
        if (nivelJugadoActual == 3) {
            serpienteJuego.crecer();
            serpienteJuego.crecer(); // dos segmentos de golpe
        } else {
            serpienteJuego.crecer();
        }
        frutasComidas++;
        if (nivelJugadoActual == 2 && frutasComidas % 2 == 0) {
            intervaloBaseNivel2 = std::max(INTERVALO_MINIMO_NIVEL2, intervaloBaseNivel2 - 10);

            if (!ralentizadoActivo) {
                // Solo aplicamos el cambio de inmediato si el ratón no está alterando la velocidad ahora mismo
                timerJuego->setInterval(intervaloBaseNivel2);
            }
        }
        lblValorPuntos->setText(QString::number(frutasComidas) + "/" + QString::number(metaFrutasNivel));

        comidaJuego.generarNuevaPosicion(tableroJuego, serpienteJuego); // siempre NORMAL ahora

        QPixmap pixmapComida(":/Recursos/Manzana.png");
        pixmapComida = pixmapComida.scaled(tamanoCeldaActual, tamanoCeldaActual, Qt::KeepAspectRatio, Qt::SmoothTransformation);
        itemComida->setPixmap(pixmapComida);
        itemComida->setPos(
            origenXCuadricula + comidaJuego.getX() * tamanoCeldaActual,
            origenYCuadricula + comidaJuego.getY() * tamanoCeldaActual
            );

        if (frutasComidas >= metaFrutasNivel) {
            timerJuego->stop();
            timerReloj->stop();
            bool esRecordNuevo = (frutasComidas > jugadorActual.puntajeMaximo);
            if (frutasComidas > jugadorActual.puntajeMaximo) jugadorActual.puntajeMaximo = frutasComidas;
            if (segundosTranscurridos > jugadorActual.tiempoMaximo) jugadorActual.tiempoMaximo = segundosTranscurridos;
            if (jugadorActual.nivelMaximoAlcanzado < 2) jugadorActual.nivelMaximoAlcanzado = 2;
            gestorArchivos->actualizarJugador(jugadorActual);
            mostrarVictoria(1, frutasComidas, vidasRestantes, segundosTranscurridos, esRecordNuevo);
        }
    }

    contadorPowerUp++;

    if (!powerUpVisible && contadorPowerUp >= TICKS_ESPERA_RANA) {
        powerUpJuego.generarNuevaPosicionForzada(tableroJuego, serpienteJuego, ESPECIAL);
        int intentos = 0;
        while (powerUpJuego.getX() == comidaJuego.getX() && powerUpJuego.getY() == comidaJuego.getY() && intentos < 10) {
            powerUpJuego.generarNuevaPosicionForzada(tableroJuego, serpienteJuego, ESPECIAL);
            intentos++;
        }

        QPixmap pixmapPowerUp(rutaSpritePowerUp);
        pixmapPowerUp = pixmapPowerUp.scaled(tamanoCeldaActual, tamanoCeldaActual, Qt::KeepAspectRatio, Qt::SmoothTransformation);
        itemPowerUp = escenaJuego->addPixmap(pixmapPowerUp);
        itemPowerUp->setZValue(1);
        itemPowerUp->setPos(
            origenXCuadricula + powerUpJuego.getX() * tamanoCeldaActual,
            origenYCuadricula + powerUpJuego.getY() * tamanoCeldaActual
            );

        powerUpVisible = true;
        contadorPowerUp = 0;

    } else if (powerUpVisible && contadorPowerUp >= TICKS_DURACION_RANA) {
        escenaJuego->removeItem(itemPowerUp);
        delete itemPowerUp;
        itemPowerUp = nullptr;
        powerUpVisible = false;
        contadorPowerUp = 0;
    }

    if (powerUpVisible && cabeza->x == powerUpJuego.getX() && cabeza->y == powerUpJuego.getY()) {

        if (tipoEfectoPowerUp == 0) { // Rana: encoge, o quita vida si ya está muy pequeña
            if (serpienteJuego.getLongitud() <= LONGITUD_MINIMA_SEGURA) {
                vidasRestantes--;
                lblValorVidas->setText(QString::number(vidasRestantes));
                if (vidasRestantes <= 0) {
                    finalizarPartidaPorDerrota("Te quedaste sin vidas comiendo ranas");
                    return;
                }
            } else {
                serpienteJuego.encoger(2);
            }
        } else if (tipoEfectoPowerUp == 1) { //RATON DEL DESIERTO
            if (!ralentizadoActivo) {
                intervaloOriginalJuego = timerJuego->interval();
                timerJuego->setInterval(intervaloOriginalJuego + 80); // VA LENTO
                ralentizadoActivo = true;
                contadorRalentizado = 0;
            }
        } else if (tipoEfectoPowerUp == 3) { // PEZ GLOBO CRECE O ENCOGE SNAKE
            if (rand() % 2 == 0) {
                serpienteJuego.crecer();
                serpienteJuego.crecer();
                qDebug() << "¡Pez globo! Creciste 2 segmentos";
            } else {
                if (serpienteJuego.getLongitud() > LONGITUD_MINIMA_SEGURA) {
                    serpienteJuego.encoger(2);
                    qDebug() << "¡Pez globo! Encogiste 2 segmentos";
                }
            }
        }

        escenaJuego->removeItem(itemPowerUp);
        delete itemPowerUp;
        itemPowerUp = nullptr;
        powerUpVisible = false;
        contadorPowerUp = 0;
    }

    // Restaurar velocidad después de un tiempo, si el ratón activó el ralentizado
    if (ralentizadoActivo) {
        contadorRalentizado++;
        if (contadorRalentizado >= 20) {
            timerJuego->setInterval((nivelJugadoActual == 2) ? intervaloBaseNivel2 : intervaloOriginalJuego);
            ralentizadoActivo = false;
        }
    }

    bool chocoConMuro = (!modoInfinitoActual && tableroJuego.obtenerValor(cabeza->y, cabeza->x) == 1);
    bool chocoConsigoMisma = serpienteJuego.chocaConsigoMisma();
    if (chocoConMuro || chocoConsigoMisma) {
        if (nivelJugadoActual == 1) {
            // Nivel 1: sistema de vidas (el muro nunca dispara aquí, es modo infinito)
            vidasRestantes--;
            lblValorVidas->setText(QString::number(vidasRestantes));

            if (vidasRestantes <= 0) {
                finalizarPartidaPorDerrota("Chocaste contigo mismo");
                return;
            } else {
                serpienteJuego.inicializar(tableroJuego.getColumnas() / 2, tableroJuego.getFilas() / 2);
            }
        } else {
            // Nivel 2 y 3: muerte instantánea, sin vidas de por medio
            QString razon = chocoConMuro ? "Chocaste contra un muro" : "Chocaste contigo mismo";
            finalizarPartidaPorDerrota(razon);
            return;
        }
    }
    redibujarSerpiente();
}

void MainWindow::crearPaginaNiveles()
{
    paginaNiveles = new QWidget();

    // --- Fondo ---
    QLabel *fondo = new QLabel(paginaNiveles);
    fondo->setPixmap(QPixmap(":/Recursos/BackgroundNiveles.png"));
    fondo->setGeometry(0, 0, 800, 700);
    fondo->lower();


    QPushButton *btnNivel1 = new QPushButton(paginaNiveles);
    btnNivel1->setIcon(QIcon(":/Recursos/nivel1boton.png"));
    btnNivel1->setIconSize(QSize(96, 96));
    btnNivel1->setGeometry(159, 172, 96, 96);
    btnNivel1->setFlat(true);
    btnNivel1->setStyleSheet("border: none; background: transparent;");
    connect(btnNivel1, &QPushButton::clicked, this, &MainWindow::iniciarNivel1);

    // --- Botón Nivel 2 (requiere haber alcanzado nivel 2) ---
    QPushButton *btnNivel2 = new QPushButton(paginaNiveles);
    btnNivel2->setIcon(QIcon(":/Recursos/nivel2boton.png"));
    btnNivel2->setIconSize(QSize(96, 96));
    btnNivel2->setGeometry(558, 172, 96, 96);
    btnNivel2->setFlat(true);
    btnNivel2->setStyleSheet("border: none; background: transparent;");

    bool nivel2Desbloqueado = (jugadorActual.nivelMaximoAlcanzado >= 2);
    btnNivel2->setEnabled(nivel2Desbloqueado);
    if (nivel2Desbloqueado) {
        connect(btnNivel2, &QPushButton::clicked, this, &MainWindow::iniciarNivel2);
    }

    // --- Botón Nivel 3 (requiere haber alcanzado nivel 3) ---
    QPushButton *btnNivel3 = new QPushButton(paginaNiveles);
    btnNivel3->setIcon(QIcon(":/Recursos/nivel3boton.png"));
    btnNivel3->setIconSize(QSize(96, 96));
    btnNivel3->setGeometry(157, 510, 96, 96);
    btnNivel3->setFlat(true);
    btnNivel3->setStyleSheet("border: none; background: transparent;");

    bool nivel3Desbloqueado = (jugadorActual.nivelMaximoAlcanzado >= 3);
    btnNivel3->setEnabled(nivel3Desbloqueado);
    if (nivel3Desbloqueado) {
        connect(btnNivel3, &QPushButton::clicked, this, &MainWindow::iniciarNivel3);
    }

    // --- Botón Safari (requiere al menos 1 serpiente capturada) ---
    QPushButton *btnSafari = new QPushButton(paginaNiveles);
    btnSafari->setIcon(QIcon(":/Recursos/safariboton.png"));
    btnSafari->setIconSize(QSize(96, 96));
    btnSafari->setGeometry(557, 510, 96, 96);
    btnSafari->setFlat(true);
    btnSafari->setStyleSheet("border: none; background: transparent;");

    bool safariDesbloqueado = (jugadorActual.nivelMaximoAlcanzado >= 2); // completó al menos Nivel 1
    btnSafari->setEnabled(safariDesbloqueado);
    if (safariDesbloqueado) {
        connect(btnSafari, &QPushButton::clicked, this, [this](){
            qDebug() << "Abriendo Safari (pendiente de implementar)";
            // más adelante: stack->setCurrentWidget(paginaSafari);
        });
    }
    QPushButton *btnSalir = new QPushButton(paginaNiveles);
    btnSalir->setIcon(QIcon(":/Recursos/UsernameSalir.png"));
    btnSalir->setIconSize(QSize(278, 70));
    btnSalir->setGeometry(267, 618, 278, 70);
    btnSalir->setFlat(true);
    btnSalir->setStyleSheet("border: none; background: transparent;");

    connect(btnSalir, &QPushButton::clicked, this, [this](){
        stack->setCurrentWidget(paginaMenuPrincipal);
    });

    QPushButton *btnVolumen = new QPushButton(paginaNiveles);
    btnVolumen->setCheckable(true);
    btnVolumen->setIcon(QIcon(":/Recursos/PlayVolumen.png"));
    btnVolumen->setIconSize(QSize(50, 50));
    btnVolumen->setGeometry(741, 8, 50, 50);
    btnVolumen->setFlat(true);
    btnVolumen->setStyleSheet("border: none; background: transparent;");

    connect(btnVolumen, &QPushButton::toggled, this, [btnVolumen](bool activado){
        btnVolumen->setIcon(QIcon(activado ? ":/Recursos/PauseVolumen.png" : ":/Recursos/PlayVolumen.png"));
    });

    stack->addWidget(paginaNiveles);
}

QPixmap MainWindow::obtenerSpriteDireccional(QString rutaDerecha, QString rutaIzquierda, Direccion direccion)
{
    switch (direccion) {
    case DERECHA:
        return QPixmap(rutaDerecha);

    case IZQUIERDA:
        return QPixmap(rutaIzquierda);

    case ARRIBA: {
        QPixmap base(rutaDerecha);
        QTransform rotacion;
        rotacion.rotate(-90); // derecha -> arriba (antihorario)
        return base.transformed(rotacion, Qt::SmoothTransformation);
    }

    case ABAJO: {
        QPixmap base(rutaDerecha);
        QTransform rotacion;
        rotacion.rotate(90); // derecha -> abajo (horario)
        return base.transformed(rotacion, Qt::SmoothTransformation);
    }
    }
    return QPixmap(rutaDerecha); // fallback, no debería llegar aquí
}

Direccion MainWindow::calcularDireccionEntreNodos(Nodo* desde, Nodo* hacia)
{
    int dx = hacia->x - desde->x;
    int dy = hacia->y - desde->y;

    // Corrige el salto falso que provoca el wraparound
    if (dx > 1) dx = -1;
    else if (dx < -1) dx = 1;

    if (dy > 1) dy = -1;
    else if (dy < -1) dy = 1;

    if (dx == 1) return DERECHA;
    if (dx == -1) return IZQUIERDA;
    if (dy == 1) return ABAJO;
    if (dy == -1) return ARRIBA;

    return DERECHA; // no debería pasar si desde != hacia
}


void MainWindow::iniciarNivel1()
{
    modoInfinitoActual = true;
    nivelJugadoActual = 1;
    rutaCabezaD = ":/Recursos/BoaHead.png";
    rutaCabezaI = ":/Recursos/BoaHeadLeft.png";
    rutaColaD=":/Recursos/BoaColaLeft.png";
    rutaColaI=":/Recursos/BoaCola.png";
    if (cicloColoresNivel != nullptr) {
        delete[] cicloColoresNivel;
    }
    powerUpVisible = false;
    contadorPowerUp = 0;
    itemPowerUp = nullptr;
    rutaSpritePowerUp = ":/Recursos/RanaLvl1.png";
    tipoEfectoPowerUp = 0; // 0 = encoge (rana)
    origenXCuadricula = 50;   // centrado exacto
    origenYCuadricula = 115;  // confirmar con prueba de líneas rojas
    tamanoCeldaActual = 50;
    metaFrutasNivel = 10;
    frutasComidas = 0;
    vidasRestantes = 3;
    lblValorVidas->setText("3");
    segundosTranscurridos = 0;
    lblValorTiempo->setText("00:00");
    timerReloj->start();
    cantidadColoresCiclo = 3;
    cicloColoresNivel = new QString[3];
    cicloColoresNivel[0] = ":/Recursos/NodoNegroLvl1.png";
    cicloColoresNivel[1] = ":/Recursos/NodoAmarilloLvl1.png";
    cicloColoresNivel[2] = ":/Recursos/NodoNaranjaLvl1.png";

    tableroJuego.configurarNivel(14, 10, 50);

    serpienteJuego.inicializar(7, 5); // centrado en la matriz (14/2, 10/2)
    comidaJuego.generarNuevaPosicion(tableroJuego, serpienteJuego);

    escenaJuego->clear();

    QGraphicsPixmapItem *fondo = escenaJuego->addPixmap(QPixmap(":/Recursos/nivel1background.png"));
    fondo->setPos(0, 0);
    dibujarGridPermanente(QColor(143, 208, 53, 100));
    fondo->setZValue(-1);

    QPixmap pixmapComidaInicial(":/Recursos/Manzana.png");
    pixmapComidaInicial = pixmapComidaInicial.scaled(tamanoCeldaActual, tamanoCeldaActual, Qt::KeepAspectRatio, Qt::SmoothTransformation);
    itemComida = escenaJuego->addPixmap(pixmapComidaInicial);
    itemComida->setZValue(1);
    itemComida->setPos(
        origenXCuadricula + comidaJuego.getX() * tamanoCeldaActual,
        origenYCuadricula + comidaJuego.getY() * tamanoCeldaActual
        );

    segmentosVisuales = nullptr;
    cantidadSegmentosVisuales = 0;
    redibujarSerpiente();

    lblValorPuntos->setText("0/10"); // formato nuevo, ya no es un número suelto

    timerJuego->start(150);
    vistaJuego->setFocus();

    stack->setCurrentWidget(paginaJuego);
}

void MainWindow::dibujarMuros(QString rutaSpriteMuro)
{
    for (int fila = 0; fila < tableroJuego.getFilas(); fila++) {
        for (int col = 0; col < tableroJuego.getColumnas(); col++) {
            if (tableroJuego.obtenerValor(fila, col) == 1) {
                QPixmap pixmapMuro(rutaSpriteMuro);
                pixmapMuro = pixmapMuro.scaled(tamanoCeldaActual, tamanoCeldaActual,
                                               Qt::KeepAspectRatio, Qt::SmoothTransformation);
                QGraphicsPixmapItem *muro = escenaJuego->addPixmap(pixmapMuro);
                muro->setPos(
                    origenXCuadricula + col * tamanoCeldaActual,
                    origenYCuadricula + fila * tamanoCeldaActual
                    );
            }
        }
    }
}


void MainWindow::iniciarNivel2()
{
    intervaloBaseNivel2=170;
    nivelJugadoActual = 2;
    modoInfinitoActual = false;
    rutaCabezaD = ":/Recursos/CascabelHead.png";
    rutaCabezaI = ":/Recursos/CascabelHeadLeft.png";
    rutaColaD = ":/Recursos/CascabelColaLeft.png";
    rutaColaI = ":/Recursos/CascabelCola.png";

    if (cicloColoresNivel != nullptr) delete[] cicloColoresNivel;
    cantidadColoresCiclo = 1;
    cicloColoresNivel = new QString[1];
    cicloColoresNivel[0] = ":/Recursos/NodoLvl2.png";

    ranaVisible = false; powerUpVisible = false; contadorPowerUp = 0; itemPowerUp = nullptr;
    ralentizadoActivo = false; contadorRalentizado = 0;
    rutaSpritePowerUp = ":/Recursos/RatonLvl2.png";
    tipoEfectoPowerUp = 1;

    origenXCuadricula = 11;
    origenYCuadricula = 101;
    tamanoCeldaActual = 45;
    metaFrutasNivel = 10;
    frutasComidas = 0;
    lblValorVidas->setText("-");
    segundosTranscurridos = 0;
    lblValorTiempo->setText("00:00");
    timerReloj->start();

    tableroJuego.configurarNivel(17, 12, 45); // <- cambiado (17 en vez de 16)
    tableroJuego.generarMurosPerimetro();
    tableroJuego.generarBloquesInternos(); // <- nueva línea

    serpienteJuego.inicializar(8, 6);
    comidaJuego.generarNuevaPosicion(tableroJuego, serpienteJuego);

    escenaJuego->clear();
    QGraphicsPixmapItem *fondo = escenaJuego->addPixmap(QPixmap(":/Recursos/nivel2background.png"));
    fondo->setPos(0, 0);
    dibujarGridPermanente(QColor(255, 255, 255, 100));
    dibujarMuros(":/Recursos/RocaLvl2.png"); // <- nueva línea
    fondo->setZValue(-1);

    QPixmap pixmapComidaInicial(":/Recursos/ManzanaLvl2.png");
    pixmapComidaInicial = pixmapComidaInicial.scaled(tamanoCeldaActual, tamanoCeldaActual, Qt::KeepAspectRatio, Qt::SmoothTransformation);
    itemComida = escenaJuego->addPixmap(pixmapComidaInicial);
    itemComida->setZValue(1);
    itemComida->setPos(
        origenXCuadricula + comidaJuego.getX() * tamanoCeldaActual,
        origenYCuadricula + comidaJuego.getY() * tamanoCeldaActual
        );

    segmentosVisuales = nullptr;
    cantidadSegmentosVisuales = 0;
    redibujarSerpiente();

    lblValorPuntos->setGeometry(122, 39, 100, 40);
    lblValorPuntos->setStyleSheet(QString("color: rgb(52,16,3); font-family: '%1'; font-size: 25px;").arg(familiaFuente));
    lblValorPuntos->setText("0/10");

    lblValorVidas->setGeometry(302, 39, 60, 40);
    lblValorVidas->setStyleSheet(QString("color: rgb(52,16,3); font-family: '%1'; font-size: 25px;").arg(familiaFuente));

    lblValorTiempo->setGeometry(640, 40, 100, 40);
    lblValorTiempo->setStyleSheet(QString("color: rgb(52,16,3); font-family: '%1'; font-size: 25px;").arg(familiaFuente));

    btnPausaJuego->setGeometry(740, 25, 40, 40);
    actualizarIconosPausa(":/Recursos/PausaVolumenLvl2.png", ":/Recursos/PlayVolumenLvl2.png", ":/Recursos/PauseVolumenLvl2.png");

    timerJuego->start(intervaloBaseNivel2);
    vistaJuego->setFocus();
    stack->setCurrentWidget(paginaJuego);
}
void MainWindow::inicializarBloquesMoviles()
{
    contadorMovimientoBloques = 0;

    for (int i = 0; i < CANTIDAD_BLOQUES_MOVILES; i++) {
        int x, y;
        // Busca una posición libre lejos del centro (donde arranca la serpiente)
        do {
            x = 2 + rand() % (tableroJuego.getColumnas() - 4);
            y = 2 + rand() % (tableroJuego.getFilas() - 4);
        } while (tableroJuego.obtenerValor(y, x) == 1 || (abs(x - 9) < 3 && abs(y - 7) < 3));

        bloquesMoviles[i].x = x;
        bloquesMoviles[i].y = y;
        bloquesMoviles[i].dx = (rand() % 2 == 0) ? 1 : -1;
        bloquesMoviles[i].dy = (rand() % 2 == 0) ? 1 : -1;

        tableroJuego.asignarValor(y, x, 1); // se marca como muro en la matriz lógica

        QPixmap pixmapBloque(":/Recursos/PiedraLvl3.png");
        pixmapBloque = pixmapBloque.scaled(tamanoCeldaActual, tamanoCeldaActual, Qt::KeepAspectRatio, Qt::SmoothTransformation);
        bloquesMoviles[i].sprite = escenaJuego->addPixmap(pixmapBloque);
        bloquesMoviles[i].sprite->setZValue(1);
        bloquesMoviles[i].sprite->setPos(
            origenXCuadricula + x * tamanoCeldaActual,
            origenYCuadricula + y * tamanoCeldaActual
            );
    }
}

void MainWindow::moverBloquesMoviles()
{
    for (int i = 0; i < CANTIDAD_BLOQUES_MOVILES; i++) {
        // Libera la celda vieja en la matriz
        tableroJuego.asignarValor(bloquesMoviles[i].y, bloquesMoviles[i].x, 0);

        int nuevoX = bloquesMoviles[i].x + bloquesMoviles[i].dx;
        int nuevoY = bloquesMoviles[i].y + bloquesMoviles[i].dy;

        // Rebota si golpea el borde interior (deja 1 celda de margen contra el muro real)
        if (nuevoX <= 1 || nuevoX >= tableroJuego.getColumnas() - 2) {
            bloquesMoviles[i].dx *= -1;
            nuevoX = bloquesMoviles[i].x + bloquesMoviles[i].dx;
        }
        if (nuevoY <= 1 || nuevoY >= tableroJuego.getFilas() - 2) {
            bloquesMoviles[i].dy *= -1;
            nuevoY = bloquesMoviles[i].y + bloquesMoviles[i].dy;
        }

        bloquesMoviles[i].x = nuevoX;
        bloquesMoviles[i].y = nuevoY;

        tableroJuego.asignarValor(nuevoY, nuevoX, 1); // vuelve a marcar como muro en la nueva posición

        bloquesMoviles[i].sprite->setPos(
            origenXCuadricula + nuevoX * tamanoCeldaActual,
            origenYCuadricula + nuevoY * tamanoCeldaActual
            );
    }
}

void MainWindow::iniciarNivel3()
{
    nivelJugadoActual = 3;
    modoInfinitoActual = false;

    if (cicloColoresNivel != nullptr) delete[] cicloColoresNivel;
    cantidadColoresCiclo = 2;
    cicloColoresNivel = new QString[2];
    cicloColoresNivel[0] = ":/Recursos/NodoAzulLvl3.png";
    cicloColoresNivel[1] = ":/Recursos/NodoNegroLvl3.png";

    rutaCabezaD = ":/Recursos/MarinaHead.png";
    rutaCabezaI = ":/Recursos/MarinaHeadLeft.png";
    rutaColaD = ":/Recursos/MarinaTailLeft.png";
    rutaColaI = ":/Recursos/MarinaTail.png";

    ranaVisible = false; powerUpVisible = false; contadorPowerUp = 0; itemPowerUp = nullptr;
    rutaSpritePowerUp = ":/Recursos/PezLvl3.png";
    tipoEfectoPowerUp = 2; // escudo
    rutaSpritePowerUp = ":/Recursos/PezGloboLvl3.png"; // usa el nombre real de tu archivo
    tipoEfectoPowerUp = 3; // pez globo: cambia tamaño al azar

    origenXCuadricula = 12;
    origenYCuadricula = 104;
    tamanoCeldaActual = 40;
    metaFrutasNivel = 10;
    frutasComidas = 0;
    lblValorVidas->setText("-");
    segundosTranscurridos = 0;
    lblValorTiempo->setText("00:00");
    timerReloj->start();

    tableroJuego.configurarNivel(19, 14, 40);
    tableroJuego.generarMurosPerimetro();
    // TODO: bloques móviles internos, pendiente para una siguiente sesión

    serpienteJuego.inicializar(9, 7);
    comidaJuego.generarNuevaPosicion(tableroJuego, serpienteJuego);

    escenaJuego->clear();
    QGraphicsPixmapItem *fondo = escenaJuego->addPixmap(QPixmap(":/Recursos/nivel3background.png"));
    fondo->setPos(0, 0);
    dibujarGridPermanente(QColor(255, 255, 255, 100));
    dibujarMuros(":/Recursos/PiedraLvl3.png");
    fondo->setZValue(-1);

    QPixmap pixmapComidaInicial(":/Recursos/ManzanaLvl3.png");
    pixmapComidaInicial = pixmapComidaInicial.scaled(tamanoCeldaActual, tamanoCeldaActual, Qt::KeepAspectRatio, Qt::SmoothTransformation);
    itemComida = escenaJuego->addPixmap(pixmapComidaInicial);
    itemComida->setZValue(1);
    itemComida->setPos(
        origenXCuadricula + comidaJuego.getX() * tamanoCeldaActual,
        origenYCuadricula + comidaJuego.getY() * tamanoCeldaActual
        );

    segmentosVisuales = nullptr;
    cantidadSegmentosVisuales = 0;
    redibujarSerpiente();

    lblValorPuntos->setGeometry(114, 40, 100, 40);
    lblValorPuntos->setStyleSheet(QString("color: white; font-family: '%1'; font-size: 29px;").arg(familiaFuente));
    lblValorPuntos->setText("0/10");

    lblValorVidas->setGeometry(275, 40, 60, 40);
    lblValorVidas->setStyleSheet(QString("color: white; font-family: '%1'; font-size: 31px;").arg(familiaFuente));

    lblValorTiempo->setGeometry(664, 41, 100, 40);
    lblValorTiempo->setStyleSheet(QString("color: white; font-family: '%1'; font-size: 25px;").arg(familiaFuente));

    actualizarIconosPausa(":/Recursos/PausaVolumenLvl3.png", ":/Recursos/PlayVolumenLvl3.png", ":/Recursos/PauseVolumenLvl3.png");

    timerJuego->start(100);
    vistaJuego->setFocus();
    inicializarBloquesMoviles();
    stack->setCurrentWidget(paginaJuego);
}


void MainWindow::crearPaginaVictoria()
{
    paginaVictoria = new QWidget();

    fondoVictoria = new QLabel(paginaVictoria);
    fondoVictoria->setGeometry(0, 0, 800, 700);
    fondoVictoria->lower();

    lblVidasVictoria = new QLabel(paginaVictoria);
    lblVidasVictoria->setGeometry(436, 346, 150, 50);
    lblVidasVictoria->setStyleSheet(QString("color: white; font-family: '%1'; font-size: 30px;").arg(familiaFuente));

    lblManzanasVictoria = new QLabel(paginaVictoria);
    lblManzanasVictoria->setGeometry(199, 346, 150, 50);
    lblManzanasVictoria->setStyleSheet(QString("color: white; font-family: '%1'; font-size: 30px;").arg(familiaFuente));

    lblTiempoVictoria = new QLabel(paginaVictoria);
    lblTiempoVictoria->setGeometry(598, 344, 180, 50);
    lblTiempoVictoria->setStyleSheet(QString("color: white; font-family: '%1'; font-size: 32px;").arg(familiaFuente));

    QPushButton *btnVerAlbum = new QPushButton(paginaVictoria);
    btnVerAlbum->setIcon(QIcon(":/Recursos/VerEnAlbum.png"));
    btnVerAlbum->setIconSize(QSize(218, 55));
    btnVerAlbum->setGeometry(38, 590, 218, 55);
    btnVerAlbum->setFlat(true);
    btnVerAlbum->setStyleSheet("border: none; background: transparent;");
    connect(btnVerAlbum, &QPushButton::clicked, this, [this](){
        qDebug() << "Abrir álbum (pendiente)";
    });

    QPushButton *btnVolverMenuVictoria = new QPushButton(paginaVictoria);
    btnVolverMenuVictoria->setIcon(QIcon(":/Recursos/VolverAMenu.png"));
    btnVolverMenuVictoria->setIconSize(QSize(218, 55));
    btnVolverMenuVictoria->setGeometry(291, 590, 218, 55);
    btnVolverMenuVictoria->setFlat(true);
    btnVolverMenuVictoria->setStyleSheet("border: none; background: transparent;");
    connect(btnVolverMenuVictoria, &QPushButton::clicked, this, [this](){
        stack->setCurrentWidget(paginaMenuPrincipal);
    });

    btnSiguienteNivel = new QPushButton(paginaVictoria);
    btnSiguienteNivel->setIcon(QIcon(":/Recursos/SiguienteNivel.png"));
    btnSiguienteNivel->setIconSize(QSize(218, 55));
    btnSiguienteNivel->setGeometry(554, 590, 218, 55);
    btnSiguienteNivel->setFlat(true);
    btnSiguienteNivel->setStyleSheet("border: none; background: transparent;");
    connect(btnSiguienteNivel, &QPushButton::clicked, this, [this](){
        if (nivelJugadoActual == 1) iniciarNivel2();
        else if (nivelJugadoActual == 2) iniciarNivel3();
    });

    QPushButton *btnVolumenVictoria = new QPushButton(paginaVictoria);
    btnVolumenVictoria->setCheckable(true);
    btnVolumenVictoria->setIcon(QIcon(":/Recursos/PlayVolumen.png"));
    btnVolumenVictoria->setIconSize(QSize(74, 74));
    btnVolumenVictoria->setGeometry(693, 83, 74, 74);
    btnVolumenVictoria->setFlat(true);
    btnVolumenVictoria->setStyleSheet("border: none; background: transparent;");
    connect(btnVolumenVictoria, &QPushButton::toggled, this, [btnVolumenVictoria](bool activado){
        btnVolumenVictoria->setIcon(QIcon(activado ? ":/Recursos/PauseVolumen.png" : ":/Recursos/PlayVolumen.png"));
    });

    stack->addWidget(paginaVictoria);
}
void MainWindow::mostrarVictoria(int nivel, int manzanas, int vidas, int segundos, bool esRecord)
{
    nivelJugadoActual = nivel;

    QString rutaFondo = (nivel == 1) ? ":/Recursos/GanasteLvl1.png"
                        : (nivel == 2) ? ":/Recursos/GanasteLvl2.png"
                                       : ":/Recursos/GanasteLvl3.png";

    fondoVictoria->setPixmap(QPixmap(rutaFondo));
    fondoVictoria->setGeometry(0, 0, 800, 700);

    lblManzanasVictoria->setText(QString::number(manzanas));
    lblVidasVictoria->setText(QString::number(vidas));

    int minutos = segundos / 60;
    int segs = segundos % 60;
    QString textoTiempo = QString("%1:%2").arg(minutos, 2, 10, QChar('0')).arg(segs, 2, 10, QChar('0'));
    if (esRecord) textoTiempo += " !";
    lblTiempoVictoria->setText(textoTiempo);

    btnSiguienteNivel->setVisible(nivel < 3);

    stack->setCurrentWidget(paginaVictoria);
}
void MainWindow::crearPaginaDerrota()
{
    paginaDerrota = new QWidget();

    fondoDerrota = new QLabel(paginaDerrota);
    fondoDerrota->setGeometry(0, 0, 800, 700);
    fondoDerrota->lower();

    lblRazonDerrota = new QLabel(paginaDerrota);
    lblRazonDerrota->setGeometry(101, 324, 600, 40);
    lblRazonDerrota->setStyleSheet(QString("color: white; font-family: '%1'; font-size: 33px;").arg(familiaFuente));

    lblManzanasDerrota = new QLabel(paginaDerrota);
    lblManzanasDerrota->setGeometry(534, 53, 300, 50);
    lblManzanasDerrota->setStyleSheet(QString("color: white; font-family: '%1'; font-size: 40px;").arg(familiaFuente));

    lblTiempoDerrota = new QLabel(paginaDerrota);
    lblTiempoDerrota->setGeometry(534, 416, 200, 50);
    lblTiempoDerrota->setStyleSheet(QString("color: white; font-family: '%1'; font-size: 40px;").arg(familiaFuente));

    QPushButton *btnReintentar = new QPushButton(paginaDerrota);
    btnReintentar->setIcon(QIcon(":/Recursos/Reintentar.png"));
    btnReintentar->setIconSize(QSize(218, 55));
    btnReintentar->setGeometry(101, 554, 218, 55);
    btnReintentar->setFlat(true);
    btnReintentar->setStyleSheet("border: none; background: transparent;");
    connect(btnReintentar, &QPushButton::clicked, this, [this](){
        if (nivelJugadoActual == 1) iniciarNivel1();
        else if (nivelJugadoActual == 2) iniciarNivel2();
        else iniciarNivel3();
    });

    QPushButton *btnVolverMenuDerrota = new QPushButton(paginaDerrota);
    btnVolverMenuDerrota->setIcon(QIcon(":/Recursos/VolverAMenu.png"));
    btnVolverMenuDerrota->setIconSize(QSize(218, 55));
    btnVolverMenuDerrota->setGeometry(463, 554, 218, 55);
    btnVolverMenuDerrota->setFlat(true);
    btnVolverMenuDerrota->setStyleSheet("border: none; background: transparent;");
    connect(btnVolverMenuDerrota, &QPushButton::clicked, this, [this](){
        stack->setCurrentWidget(paginaMenuPrincipal);
    });

    stack->addWidget(paginaDerrota);
}

void MainWindow::mostrarDerrota(int nivel, QString razon, int manzanas, int segundos)
{
    nivelJugadoActual = nivel;

    QString rutaFondo = (nivel == 1) ? ":/Recursos/PerdisteLvl1.png"
                        : (nivel == 2) ? ":/Recursos/PerdisteLvl2.png"
                                       : ":/Recursos/PerdisteLvl3.png";

    fondoDerrota->setPixmap(QPixmap(rutaFondo));
    fondoDerrota->setGeometry(0, 0, 800, 700);

    lblRazonDerrota->setText(razon);
    lblManzanasDerrota->setText(QString::number(manzanas));

    int minutos = segundos / 60;
    int segs = segundos % 60;
    lblTiempoDerrota->setText(QString("%1:%2").arg(minutos, 2, 10, QChar('0')).arg(segs, 2, 10, QChar('0')));

    stack->setCurrentWidget(paginaDerrota);
}
void MainWindow::finalizarPartidaPorDerrota(QString razon)
{
    timerJuego->stop();
    timerReloj->stop();

    if (frutasComidas > jugadorActual.puntajeMaximo) {
        jugadorActual.puntajeMaximo = frutasComidas;
    }
    gestorArchivos->actualizarJugador(jugadorActual);

    mostrarDerrota(nivelJugadoActual, razon, frutasComidas, segundosTranscurridos);
}

void MainWindow::mostrarPausa()
{
    timerJuego->stop();
    timerReloj->stop();
    overlayPausa->setVisible(true);
    overlayPausa->raise();
}

void MainWindow::ocultarPausa()
{
    overlayPausa->setVisible(false);
    timerJuego->start();
    timerReloj->start();
}
MainWindow::~MainWindow()
{
    delete gestorArchivos;
}