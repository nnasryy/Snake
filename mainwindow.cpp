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
    crearPaginaNiveles();
    crearPaginaJuego();

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

    QPushButton *btnPausa = new QPushButton(paginaJuego);
    btnPausa->setIcon(QIcon(":/Recursos/PauseVolumen.png")); // ícono de pausa, no el de volumen
    btnPausa->setIconSize(QSize(40, 40));
    btnPausa->setGeometry(740, 45, 40, 40);
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
            if (patron == 0) rutaSprite = ":/Recursos/NodoNegroLvl1.png";
            else if (patron == 1) rutaSprite = ":/Recursos/NodoAmarilloLvl1.png";
            else rutaSprite = ":/Recursos/NodoNaranjaLvl1.png";
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
    serpienteJuego.mover(tableroJuego.getColumnas(), tableroJuego.getFilas(), true);

    Nodo* cabeza = serpienteJuego.getCabeza();

    if (cabeza->x == comidaJuego.getX() && cabeza->y == comidaJuego.getY()) {
        serpienteJuego.crecer();
        frutasComidas++;
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
            qDebug() << "¡Nivel completado!";
        }
    }

    contadorRana++;

    if (!ranaVisible && contadorRana >= TICKS_ESPERA_RANA) {
        ranaJuego.generarNuevaPosicionForzada(tableroJuego, serpienteJuego, ESPECIAL);

        int intentos = 0;
        while (ranaJuego.getX() == comidaJuego.getX() && ranaJuego.getY() == comidaJuego.getY() && intentos < 10) {
            ranaJuego.generarNuevaPosicionForzada(tableroJuego, serpienteJuego, ESPECIAL);
            intentos++;
        }

        QPixmap pixmapRana(":/Recursos/RanaLvl1.png");
        pixmapRana = pixmapRana.scaled(tamanoCeldaActual, tamanoCeldaActual, Qt::KeepAspectRatio, Qt::SmoothTransformation);
        itemRana = escenaJuego->addPixmap(pixmapRana);
        itemRana->setZValue(1);
        itemRana->setPos(
            origenXCuadricula + ranaJuego.getX() * tamanoCeldaActual,
            origenYCuadricula + ranaJuego.getY() * tamanoCeldaActual
            );

        ranaVisible = true;
        contadorRana = 0;

    } else if (ranaVisible && contadorRana >= TICKS_DURACION_RANA) {
        // Se acabó su tiempo: desaparece sin haber sido comida
        escenaJuego->removeItem(itemRana);
        delete itemRana;
        itemRana = nullptr;
        ranaVisible = false;
        contadorRana = 0;
    }

    if (ranaVisible && cabeza->x == ranaJuego.getX() && cabeza->y == ranaJuego.getY()) {

        if (serpienteJuego.getLongitud() <= LONGITUD_MINIMA_SEGURA) {
            vidasRestantes--;
            lblValorVidas->setText(QString::number(vidasRestantes));
            qDebug() << "¡Rana comida con serpiente muy pequeña! Vida perdida.";
        } else {
            serpienteJuego.encoger(2);
        }

        escenaJuego->removeItem(itemRana);
        delete itemRana;
        itemRana = nullptr;
        ranaVisible = false;
        contadorRana = 0;
    }

    if (serpienteJuego.chocaConsigoMisma()) {
        vidasRestantes--;
        lblValorVidas->setText(QString::number(vidasRestantes));

        if (vidasRestantes <= 0) {
            timerJuego->stop();
            timerReloj->stop();
            qDebug() << "Game Over definitivo: sin vidas restantes";
            // más adelante: stack->setCurrentWidget(paginaGameOver);
        } else {
            // Reinicia la serpiente sin perder el progreso de frutasComidas
            serpienteJuego.inicializar(tableroJuego.getColumnas() / 2, tableroJuego.getFilas() / 2);
            qDebug() << "Chocó, pero le quedan vidas. Vidas restantes:" << vidasRestantes;
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
        connect(btnNivel2, &QPushButton::clicked, this, [this](){
            qDebug() << "Iniciando Nivel 2 (pendiente de implementar)";
            // más adelante: iniciarNivel2();
        });
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
        connect(btnNivel3, &QPushButton::clicked, this, [this](){
            qDebug() << "Iniciando Nivel 3 (pendiente de implementar)";
            // más adelante: iniciarNivel3();
        });
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
void MainWindow::iniciarNivel1()
{
    ranaVisible = false;
    contadorRana = 0;
    itemRana = nullptr;
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

    tableroJuego.configurarNivel(14, 10, 50);

    serpienteJuego.inicializar(7, 5); // centrado en la matriz (14/2, 10/2)
    comidaJuego.generarNuevaPosicion(tableroJuego, serpienteJuego);

    escenaJuego->clear();

    QGraphicsPixmapItem *fondo = escenaJuego->addPixmap(QPixmap(":/Recursos/nivel1background.png"));
    fondo->setPos(0, 0);
    fondo->setZValue(-1);

    QPen lapizGrid(QColor(143, 208, 53, 100));
    lapizGrid.setWidth(1);

    for (int col = 0; col <= tableroJuego.getColumnas(); col++) {
        int x = origenXCuadricula + col * tamanoCeldaActual;
        escenaJuego->addLine(x, origenYCuadricula, x, origenYCuadricula + tableroJuego.getFilas() * tamanoCeldaActual, lapizGrid);
    }
    for (int fila = 0; fila <= tableroJuego.getFilas(); fila++) {
        int y = origenYCuadricula + fila * tamanoCeldaActual;
        escenaJuego->addLine(origenXCuadricula, y, origenXCuadricula + tableroJuego.getColumnas() * tamanoCeldaActual, y, lapizGrid);
    }


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
void MainWindow::mostrarPausa()
{
    timerJuego->stop();
    timerReloj->stop(); // <- agrega esta línea
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