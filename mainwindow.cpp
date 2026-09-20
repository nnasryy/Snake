#include "mainwindow.h"
#include <QVBoxLayout>
#include <QLabel>
#include <QApplication>
#include <QDebug>
#include <QDirIterator>
#include <QFontDatabase>
#include <QTransform>
#include <algorithm>
#include <QRegularExpressionValidator>
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
    crearPaginaAlbums();
    crearPaginaSafariSetup();
    crearPaginaRanking();
    for (int nivel = 1; nivel <= 3; nivel++) crearPaginaAlbumNivel(nivel);


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

    QString estiloCampo = QString(
                              "QLineEdit {"
                              "  background-color: rgb(10, 54, 8);"
                              "  border: 5px solid rgb(177, 215, 77);"
                              "  color: rgb(177, 215, 77);"
                              "  font-family: '%1';"
                              "  font-size: 28px;"
                              "  padding-left: 10px;"
                              "}"
                              ).arg(familiaFuente);

    // Solo letras y números, sin símbolos ni espacios
    QRegularExpression regexAlfanumerico("[A-Za-z0-9]*");

    // --- Campo de usuario ---
    campoNombre = new QLineEdit(paginaUsername);
    campoNombre->setGeometry(70, 230, 591, 75);
    campoNombre->setAlignment(Qt::AlignCenter);
    campoNombre->setMaxLength(12); // mínimo 6 se valida al confirmar, máximo razonable para no romper el HUD
    campoNombre->setPlaceholderText("Usuario (mín. 6 caracteres)");
    campoNombre->setValidator(new QRegularExpressionValidator(regexAlfanumerico, campoNombre));
    campoNombre->setStyleSheet(estiloCampo);

    // --- Campo de contraseña ---
    campoContrasena = new QLineEdit(paginaUsername);
    campoContrasena->setGeometry(69, 420, 591, 75);
    campoContrasena->setAlignment(Qt::AlignCenter);
    campoContrasena->setMaxLength(20);
    campoContrasena->setPlaceholderText("Contraseña (letras y números)");
    campoContrasena->setEchoMode(QLineEdit::Password);
    campoContrasena->setValidator(new QRegularExpressionValidator(regexAlfanumerico, campoContrasena));
    campoContrasena->setStyleSheet(estiloCampo);

    // --- Botón mostrar/ocultar contraseña ---
    btnMostrarPassword = new QPushButton(paginaUsername);
    btnMostrarPassword->setCheckable(true);
    btnMostrarPassword->setIcon(QIcon(":/Recursos/PassClosed.png"));
    btnMostrarPassword->setIconSize(QSize(65, 65));
    btnMostrarPassword->setGeometry(690, 427, 65, 65);
    btnMostrarPassword->setFlat(true);
    btnMostrarPassword->setStyleSheet("border: none; background: transparent;");
    connect(btnMostrarPassword, &QPushButton::clicked, this, &MainWindow::togglePassword);

    // --- Botón salir ---
    QPushButton *btnSalir = new QPushButton(paginaUsername);
    btnSalir->setIcon(QIcon(":/Recursos/UsernameSalir.png"));
    btnSalir->setIconSize(QSize(278, 70));
    btnSalir->setGeometry(82, 549, 278, 70);
    btnSalir->setFlat(true);
    btnSalir->setStyleSheet("border: none; background: transparent;");
    connect(btnSalir, &QPushButton::clicked, this, [this](){
        stack->setCurrentWidget(paginaInicio);
    });

    // --- Botón confirmar ---
    QPushButton *btnConfirmar = new QPushButton(paginaUsername);
    btnConfirmar->setIcon(QIcon(":/Recursos/UsernameConfirmar.png"));
    btnConfirmar->setIconSize(QSize(278, 70));
    btnConfirmar->setGeometry(463, 553, 278, 70);
    btnConfirmar->setFlat(true);
    btnConfirmar->setStyleSheet("border: none; background: transparent;");
    connect(btnConfirmar, &QPushButton::clicked, this, &MainWindow::validarNombre);
    connect(campoContrasena, &QLineEdit::returnPressed, this, &MainWindow::validarNombre);

    // --- Botón de volumen (igual al resto de pantallas) ---
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

    // --- Overlay de error (oculto por defecto) ---
    overlayErrorLogin = new QWidget(paginaUsername);
    overlayErrorLogin->setGeometry(150, 260, 500, 160);
    overlayErrorLogin->setStyleSheet("background-color: rgba(15, 58, 13, 230); border: 5px solid rgb(143, 208, 53);");
    overlayErrorLogin->setVisible(false);

    lblErrorLogin = new QLabel(overlayErrorLogin);
    lblErrorLogin->setGeometry(20, 15, 460, 80);
    lblErrorLogin->setWordWrap(true);
    lblErrorLogin->setAlignment(Qt::AlignCenter);
    lblErrorLogin->setStyleSheet(QString("color: white; font-family: '%1'; font-size: 18px;").arg(familiaFuente));

    QPushButton *btnCerrarError = new QPushButton("Entendido", overlayErrorLogin);
    btnCerrarError->setGeometry(175, 105, 150, 40);
    connect(btnCerrarError, &QPushButton::clicked, this, [this](){
        overlayErrorLogin->setVisible(false);
    });

    stack->addWidget(paginaUsername);
}

void MainWindow::togglePassword()
{
    if (btnMostrarPassword->isChecked()) {
        campoContrasena->setEchoMode(QLineEdit::Normal);
        btnMostrarPassword->setIcon(QIcon(":/Recursos/PassOpened.png"));
    } else {
        campoContrasena->setEchoMode(QLineEdit::Password);
        btnMostrarPassword->setIcon(QIcon(":/Recursos/PassClosed.png"));
    }
}

void MainWindow::mostrarErrorLogin(QString mensaje)
{
    lblErrorLogin->setText(mensaje);
    overlayErrorLogin->setVisible(true);
    overlayErrorLogin->raise();
}
void MainWindow::validarNombre()
{
    QString nombreQt = campoNombre->text().trimmed();
    QString passwordQt = campoContrasena->text();

    if (nombreQt.length() < 6) {
        mostrarErrorLogin("El usuario debe tener al menos 6 caracteres.");
        return;
    }

    if (!Usuario::esContrasenaValida(passwordQt.toStdString())) {
        mostrarErrorLogin("La contraseña debe tener al menos 6 caracteres,\ncon letras y números.");
        return;
    }

    string nombre = nombreQt.toStdString();
    string password = passwordQt.toStdString();

    if (gestorArchivos->buscarJugadorPorNombre(nombre, jugadorActual)) {
        // Usuario existente: la contraseña debe coincidir
        if (!jugadorActual.verificarContrasena(password)) {
            mostrarErrorLogin("Contraseña incorrecta para ese usuario.");
            return;
        }
        qDebug() << "Bienvenido de vuelta:" << QString::fromStdString(jugadorActual.getNombre())
                 << "| Puntaje máximo:" << jugadorActual.getPuntajeMaximo()
                 << "| Nivel alcanzado:" << jugadorActual.getNivelMaximoAlcanzado();
    } else {
        // Usuario nuevo
        jugadorActual = Jugador(nombre);
        jugadorActual.setContrasena(password);
        gestorArchivos->crearJugador(jugadorActual);
        qDebug() << "Nuevo jugador creado:" << QString::fromStdString(jugadorActual.getNombre());
    }

    campoNombre->clear();
    campoContrasena->clear();
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
        actualizarBotonesNiveles();
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

    connect(btnRecords, &QPushButton::clicked, this, [this](){
        mostrarRanking(0);
        btnTabRanking[0]->setChecked(true);
        stack->setCurrentWidget(paginaRanking);
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

    connect(btnAlbum, &QPushButton::clicked, this, [this](){
        actualizarBotonesAlbums();
        stack->setCurrentWidget(paginaAlbums);
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
    lblToastSafari = new QLabel(paginaJuego);
    lblToastSafari->setGeometry(150, 300, 500, 100);
    lblToastSafari->setAlignment(Qt::AlignCenter);
    lblToastSafari->setWordWrap(true);
    lblToastSafari->setStyleSheet(QString(
                                      "background-color: rgba(15,58,13,230); color: white; border: 4px solid rgb(143,208,53);"
                                      "font-family: '%1'; font-size: 20px;"
                                      ).arg(familiaFuente));
    lblToastSafari->setVisible(false);

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

void MainWindow::iniciarNivelConConfiguracion(ConfiguracionNivel &config)
{
    nivelJugadoActual = config.getNivel();
    modoInfinitoActual = config.getModoInfinito();
    rutaCabezaD = config.getCabezaD();
    rutaCabezaI = config.getCabezaI();
    rutaColaD = config.getColaD();
    rutaColaI = config.getColaI();
    origenXCuadricula = config.getOrigenX();
    origenYCuadricula = config.getOrigenY();
    tamanoCeldaActual = config.getTamanoCelda();
    metaFrutasNivel = config.getMetaFrutas();
    frutasComidas = 0;
    segundosTranscurridos = 0;
    lblValorTiempo->setText("00:00");

    if (cicloColoresNivel != nullptr) delete[] cicloColoresNivel;
    cantidadColoresCiclo = config.getCantidadColores();
    cicloColoresNivel = new QString[cantidadColoresCiclo];
    for (int i = 0; i < cantidadColoresCiclo; i++) {
        cicloColoresNivel[i] = config.getColoresCuerpo()[i];
    }

    if (rutasPowerUpVariantes != nullptr) { delete[] rutasPowerUpVariantes; delete[] tiposPowerUpVariantes; }
    cantidadVariantesPowerUp = config.getCantidadPowerUps();
    rutasPowerUpVariantes = new QString[cantidadVariantesPowerUp];
    tiposPowerUpVariantes = new int[cantidadVariantesPowerUp];
    for (int i = 0; i < cantidadVariantesPowerUp; i++) {
        rutasPowerUpVariantes[i] = config.getRutasPowerUp()[i];
        tiposPowerUpVariantes[i] = config.getTiposPowerUp()[i];
    }
    powerUpVisible = false;
    contadorPowerUp = 0;
    itemPowerUp = nullptr;
    ranaVisible = false;
    ralentizadoActivo = false;
    contadorRalentizado = 0;

    vidasRestantes = config.getVidasIniciales();
    lblValorVidas->setText(config.getTieneVidas() ? QString::number(vidasRestantes) : "-");

    tableroJuego.configurarNivel(config.getColumnas(), config.getFilas(), config.getTamanoCelda());
    if (!modoInfinitoActual) {
        tableroJuego.generarMurosPerimetro();
    }
    if (config.getGeneraBloquesInternos()) {
        tableroJuego.generarBloquesInternos();
    }

    serpienteJuego.inicializar(config.getColumnas() / 2, config.getFilas() / 2);
    comidaJuego.generarNuevaPosicion(tableroJuego, serpienteJuego);

    escenaJuego->clear();
    QGraphicsPixmapItem *fondo = escenaJuego->addPixmap(QPixmap(config.getRutaFondo()));
    fondo->setPos(0, 0);
    dibujarGridPermanente(config.getColorGrid());
    if (!config.getRutaMuro().isEmpty()) {
        dibujarMuros(config.getRutaMuro());
    }
    fondo->setZValue(-1);

    QPixmap pixmapComidaInicial(config.getRutaComida());
    pixmapComidaInicial = pixmapComidaInicial.scaled(tamanoCeldaActual, tamanoCeldaActual, Qt::KeepAspectRatio, Qt::SmoothTransformation);
    itemComida = escenaJuego->addPixmap(pixmapComidaInicial);
    itemComida->setZValue(1);
    itemComida->setPos(origenXCuadricula + comidaJuego.getX() * tamanoCeldaActual,
                       origenYCuadricula + comidaJuego.getY() * tamanoCeldaActual);

    segmentosVisuales = nullptr;
    cantidadSegmentosVisuales = 0;
    redibujarSerpiente();

    lblValorPuntos->setGeometry(config.getRectPuntos());
    lblValorVidas->setGeometry(config.getRectVidas());
    lblValorTiempo->setGeometry(config.getRectTiempo());

    QString colorHex = config.getColorTextoHUD().name();
    lblValorPuntos->setStyleSheet(QString("color: %1; font-family: '%2'; font-size: %3px;")
                                      .arg(colorHex, familiaFuente).arg(config.getFuentePuntos()));
    lblValorVidas->setStyleSheet(QString("color: %1; font-family: '%2'; font-size: %3px;")
                                     .arg(colorHex, familiaFuente).arg(config.getFuenteVidas()));
    lblValorTiempo->setStyleSheet(QString("color: %1; font-family: '%2'; font-size: %3px;")
                                      .arg(colorHex, familiaFuente).arg(config.getFuenteTiempo()));

    lblValorPuntos->setText("0/" + QString::number(metaFrutasNivel));

    actualizarIconosPausa(config.getIconoPausa(), config.getVolumenPlay(), config.getVolumenPause());
    btnPausaJuego->setGeometry(740, 45, 40, 40);

    timerReloj->start();
    timerJuego->start(config.getIntervaloInicial());
    vistaJuego->setFocus();

    if (config.getTieneBloquesMoviles()) {
        inicializarBloquesMoviles();
    }

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
        serpienteJuego.crecer();
        if (nivelJugadoActual == 3) serpienteJuego.crecer();
        frutasComidas++;
        comidaJuego.generarNuevaPosicion(tableroJuego, serpienteJuego);
        int intentosComida = 0;
        while (powerUpVisible && comidaJuego.getX() == powerUpJuego.getX() && comidaJuego.getY() == powerUpJuego.getY() && intentosComida < 10) {
            comidaJuego.generarNuevaPosicion(tableroJuego, serpienteJuego);
            intentosComida++;
        }
        QPixmap pixmapComida(":/Recursos/Manzana.png");
        pixmapComida = pixmapComida.scaled(tamanoCeldaActual, tamanoCeldaActual, Qt::KeepAspectRatio, Qt::SmoothTransformation);
        itemComida->setPixmap(pixmapComida);
        itemComida->setPos(origenXCuadricula + comidaJuego.getX() * tamanoCeldaActual, origenYCuadricula + comidaJuego.getY() * tamanoCeldaActual);

        if (modoSafariActivo) {
            lblValorPuntos->setText("Frutas: " + QString::number(frutasComidas));
            if (frutasComidas % 15 == 0) {
                jugadorActual.registrarSafariCompletado();
                gestorArchivos->actualizarJugador(jugadorActual);
                mostrarToastSafari("¡Safari completado!\nTotal: " + QString::number(jugadorActual.getCantidadSafarisCompletados()));
            }
        } else {
            if (nivelJugadoActual == 2 && frutasComidas % 2 == 0) {
                intervaloBaseNivel2 = std::max(INTERVALO_MINIMO_NIVEL2, intervaloBaseNivel2 - 10);
                if (!ralentizadoActivo) timerJuego->setInterval(intervaloBaseNivel2);
            }
            lblValorPuntos->setText(QString::number(frutasComidas) + "/" + QString::number(metaFrutasNivel));

            if (frutasComidas >= metaFrutasNivel) {
                timerJuego->stop();
                timerReloj->stop();
                bool esRecordNuevo = (frutasComidas > jugadorActual.getPuntajeMaximo());
                jugadorActual.actualizarRecord(frutasComidas, segundosTranscurridos);
                jugadorActual.actualizarRecordNivel(nivelJugadoActual, segundosTranscurridos);
                if (nivelJugadoActual == 1) jugadorActual.desbloquearNivel(2);
                if (nivelJugadoActual == 2) jugadorActual.desbloquearNivel(3);
                jugadorActual.registrarNivelCompletado(nivelJugadoActual);
                gestorArchivos->actualizarJugador(jugadorActual);
                mostrarVictoria(nivelJugadoActual, frutasComidas, vidasRestantes, segundosTranscurridos, esRecordNuevo);
                return;
            }
        }
    }

    contadorPowerUp++;

    if (cantidadVariantesPowerUp > 0 && !powerUpVisible && contadorPowerUp >= TICKS_ESPERA_RANA) {
        int variante = rand() % cantidadVariantesPowerUp;
        rutaSpritePowerUp = rutasPowerUpVariantes[variante];
        tipoEfectoPowerUp = tiposPowerUpVariantes[variante];
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
        } else if (tipoEfectoPowerUp == 3) { // PEZ GLOBO: crece 2 segmentos
            serpienteJuego.crecer();
            serpienteJuego.crecer();
            qDebug() << "¡Pez globo! Creciste 2 segmentos";
        } else if (tipoEfectoPowerUp == 2) { // PEZ PAYASO: quita 2 segmentos, o mata si ya no puede
            if (serpienteJuego.getLongitud() <= LONGITUD_MINIMA_SEGURA) {
                finalizarPartidaPorDerrota("Te quedaste sin cuerpo suficiente");
                return;
            } else {
                serpienteJuego.encoger(2);
                qDebug() << "¡Pez payaso! Perdiste 2 segmentos";
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
        if (serpienteJuego.consumirEscudo()) {
            qDebug() << "¡Escudo absorbió el golpe!";
            serpienteJuego.inicializar(tableroJuego.getColumnas() / 2, tableroJuego.getFilas() / 2);
        } else {
            vidasRestantes--;
            lblValorVidas->setText(QString::number(vidasRestantes));
            if (vidasRestantes <= 0) {
                if (modoSafariActivo) {
                    finalizarSafariPorDerrota();
                } else {
                    QString razon = chocoConMuro ? "Chocaste contra un muro" : "Chocaste contigo mismo";
                    finalizarPartidaPorDerrota(razon);
                }
                return;
            } else {
                serpienteJuego.inicializar(tableroJuego.getColumnas() / 2, tableroJuego.getFilas() / 2);
            }
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
    btnNivel2 = new QPushButton(paginaNiveles);
    btnNivel2->setIcon(QIcon(":/Recursos/nivel2boton.png"));
    btnNivel2->setIconSize(QSize(96, 96));
    btnNivel2->setGeometry(558, 172, 96, 96);
    btnNivel2->setFlat(true);
    btnNivel2->setStyleSheet("border: none; background: transparent;");
    connect(btnNivel2, &QPushButton::clicked, this, &MainWindow::iniciarNivel2);

    // --- Botón Nivel 3 (requiere haber alcanzado nivel 3) ---
    btnNivel3 = new QPushButton(paginaNiveles);
    btnNivel3->setIcon(QIcon(":/Recursos/nivel3boton.png"));
    btnNivel3->setIconSize(QSize(96, 96));
    btnNivel3->setGeometry(157, 510, 96, 96);
    btnNivel3->setFlat(true);
    btnNivel3->setStyleSheet("border: none; background: transparent;");

    bool nivel3Desbloqueado = jugadorActual.tieneNivelDesbloqueado(3);
    btnNivel3->setEnabled(nivel3Desbloqueado);
    connect(btnNivel3, &QPushButton::clicked, this, &MainWindow::iniciarNivel3);
    // --- Botón Safari (requiere al menos 1 serpiente capturada) ---
    btnSafari = new QPushButton(paginaNiveles);
    btnSafari->setIcon(QIcon(":/Recursos/safariboton.png"));
    btnSafari->setIconSize(QSize(96, 96));
    btnSafari->setGeometry(557, 510, 96, 96);
    btnSafari->setFlat(true);
    btnSafari->setStyleSheet("border: none; background: transparent;");
    connect(btnSafari, &QPushButton::clicked, this, [this](){
        qDebug() << "Botón Safari presionado";
        for (int i = 1; i <= 3; i++) {
            QString estado = jugadorActual.haCompletadoNivel(i) ? "SinCandado" : "ConCandado";
            btnSkinSafari[i]->setIcon(QIcon(QString(":/Recursos/AlbumNivel%1%2.png").arg(i).arg(estado)));
            btnSkinSafari[i]->setEnabled(jugadorActual.haCompletadoNivel(i));
        }
        stack->setCurrentWidget(paginaSafariSetup);
    });
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
    actualizarBotonesNiveles();
    stack->addWidget(paginaNiveles);
}
void MainWindow::actualizarBotonesNiveles()
{
    btnNivel2->setEnabled(jugadorActual.tieneNivelDesbloqueado(2));
    btnNivel3->setEnabled(jugadorActual.tieneNivelDesbloqueado(3));
    btnSafari->setEnabled(jugadorActual.tieneNivelDesbloqueado(2));
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
bool MainWindow::celdaOcupadaPorSerpiente(int x, int y)
{
    Nodo* actual = serpienteJuego.getCabeza();
    while (actual != nullptr) {
        if (actual->x == x && actual->y == y) return true;
        actual = actual->siguiente;
    }
    return false;
}

void MainWindow::iniciarNivel1()
{
    modoSafariActivo = false;
    ConfiguracionNivel config;
    config.establecer(1, 14, 10, 50, 10, true, 150, 50, 115,
                      ":/Recursos/nivel1background.png", ":/Recursos/Manzana.png",
                      ":/Recursos/BoaHead.png", ":/Recursos/BoaHeadLeft.png",
                      ":/Recursos/BoaColaLeft.png", ":/Recursos/BoaCola.png");

    QString colores[3] = { ":/Recursos/NodoNegroLvl1.png", ":/Recursos/NodoAmarilloLvl1.png", ":/Recursos/NodoNaranjaLvl1.png" };
    config.establecerColoresCuerpo(colores, 3);

    QString rutasPU[1] = { ":/Recursos/RanaLvl1.png" };
    int tiposPU[1] = { 0 };
    config.establecerPowerUps(rutasPU, tiposPU, 1);

    config.establecerVidas(true, 3);
    config.establecerColorGrid(QColor(143, 208, 53, 100));

    iniciarNivelConConfiguracion(config);
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
    modoSafariActivo = false;
    intervaloBaseNivel2 = 180;

    ConfiguracionNivel config;
    config.establecer(2, 17, 12, 45, 10, false, intervaloBaseNivel2, 11, 101,
                      ":/Recursos/nivel2background.png", ":/Recursos/ManzanaLvl2.png",
                      ":/Recursos/CascabelHead.png", ":/Recursos/CascabelHeadLeft.png",
                      ":/Recursos/CascabelColaLeft.png", ":/Recursos/CascabelCola.png");

    QString colores[1] = { ":/Recursos/NodoLvl2.png" };
    config.establecerColoresCuerpo(colores, 1);

    QString rutasPU[1] = { ":/Recursos/RatonLvl2.png" };
    int tiposPU[1] = { 1 };
    config.establecerPowerUps(rutasPU, tiposPU, 1);

   config.establecerVidas(true, 3);
    config.establecerMuro(":/Recursos/RocaLvl2.png", true);
    config.establecerColorGrid(QColor(255, 255, 255, 100));
    config.establecerHUD(QColor(52, 16, 3),
                         QRect(122, 39, 100, 40), QRect(302, 39, 60, 40), QRect(640, 40, 100, 40),
                         25, 25, 25);
    config.establecerIconosPausa(":/Recursos/PausaVolumenLvl2.png", ":/Recursos/PlayVolumenLvl2.png", ":/Recursos/PauseVolumenLvl2.png");

    iniciarNivelConConfiguracion(config);

    btnPausaJuego->setGeometry(740, 25, 40, 40); // única diferencia visual propia de este nivel
}
void MainWindow::inicializarBloquesMoviles()
{
    contadorMovimientoBloques = 0;

    for (int i = 0; i < CANTIDAD_BLOQUES_MOVILES; i++) {
        int x, y;
        do {
            x = 2 + rand() % (tableroJuego.getColumnas() - 4);
            y = 2 + rand() % (tableroJuego.getFilas() - 4);
        } while (tableroJuego.obtenerValor(y, x) == 1 || (abs(x - 9) < 3 && abs(y - 7) < 3));

        int dx = (rand() % 2 == 0) ? 1 : -1;
        int dy = (rand() % 2 == 0) ? 1 : -1;

        tableroJuego.asignarValor(y, x, 1);

        QPixmap pixmapBloque(":/Recursos/RocaLvl3.png");
        pixmapBloque = pixmapBloque.scaled(tamanoCeldaActual, tamanoCeldaActual, Qt::KeepAspectRatio, Qt::SmoothTransformation);
        QGraphicsPixmapItem *sprite = escenaJuego->addPixmap(pixmapBloque);
        sprite->setZValue(1);
        sprite->setPos(origenXCuadricula + x * tamanoCeldaActual, origenYCuadricula + y * tamanoCeldaActual);

        bloquesMoviles[i].inicializar(x, y, dx, dy, sprite);
    }
}

void MainWindow::moverBloquesMoviles()
{
    for (int i = 0; i < CANTIDAD_BLOQUES_MOVILES; i++) {
        tableroJuego.asignarValor(bloquesMoviles[i].getY(), bloquesMoviles[i].getX(), 0);
        bloquesMoviles[i].mover(tableroJuego.getColumnas(), tableroJuego.getFilas());

        int nx = bloquesMoviles[i].getX();
        int ny = bloquesMoviles[i].getY();
        if (celdaOcupadaPorSerpiente(nx, ny)) {
            bloquesMoviles[i].mover(tableroJuego.getColumnas(), tableroJuego.getFilas());
        }

        tableroJuego.asignarValor(bloquesMoviles[i].getY(), bloquesMoviles[i].getX(), 1);
        bloquesMoviles[i].getSprite()->setPos(
            origenXCuadricula + bloquesMoviles[i].getX() * tamanoCeldaActual,
            origenYCuadricula + bloquesMoviles[i].getY() * tamanoCeldaActual
            );
    }
}

void MainWindow::iniciarNivel3()
{
    modoSafariActivo = false;
    ConfiguracionNivel config;
    config.establecer(3, 19, 14, 40, 10, false, 100, 12, 104,
                      ":/Recursos/nivel3background.png", ":/Recursos/ManzanaLvl3.png",
                      ":/Recursos/MarinaHead.png", ":/Recursos/MarinaHeadLeft.png",
                      ":/Recursos/MarinaTailLeft.png", ":/Recursos/MarinaTail.png");

    QString colores[2] = { ":/Recursos/NodoAzulLvl3.png", ":/Recursos/NodoNegroLvl3.png" };
    config.establecerColoresCuerpo(colores, 2);

    QString rutasPU[2] = { ":/Recursos/PezLvl3.png", ":/Recursos/PezGloboLvl3.png" };
    int tiposPU[2] = { 2, 3 };
    config.establecerPowerUps(rutasPU, tiposPU, 2);

    config.establecerVidas(true, 3);
    config.establecerMuro(":/Recursos/RocaLvl3.png", false);
    config.establecerBloquesMoviles(true);
    config.establecerColorGrid(QColor(255, 255, 255, 100));
    config.establecerHUD(QColor(255, 255, 255),
                         QRect(114, 40, 100, 40), QRect(275, 40, 60, 40), QRect(664, 41, 100, 40),
                         29, 31, 25);
    config.establecerIconosPausa(":/Recursos/PausaVolumenLvl3.png", ":/Recursos/PlayVolumenLvl3.png", ":/Recursos/PauseVolumenLvl3.png");

    iniciarNivelConConfiguracion(config);
}
void MainWindow::crearPaginaSafariSetup()
{
    paginaSafariSetup = new QWidget();

    QLabel *fondo = new QLabel(paginaSafariSetup);
    fondo->setPixmap(QPixmap(":/Recursos/SafariSetupBackground.png"));
    fondo->setGeometry(0, 0, 800, 700);
    fondo->lower();

    // --- Selector de skin (4 opciones, exclusivas) ---
    grupoSkinSafari = new QButtonGroup(this);
    grupoSkinSafari->setExclusive(true);
    const int posXSkin[4] = {40, 220, 400, 580};
    for (int i = 0; i < 4; i++) {
        btnSkinSafari[i] = new QPushButton(paginaSafariSetup);
        btnSkinSafari[i]->setCheckable(true);
        btnSkinSafari[i]->setIconSize(QSize(150, 150));
        btnSkinSafari[i]->setGeometry(posXSkin[i], 90, 150, 150);
        btnSkinSafari[i]->setStyleSheet(
            "QPushButton { border: 4px solid transparent; background: transparent; }"
            "QPushButton:checked { border: 4px solid rgb(143,208,53); }"
            );
        grupoSkinSafari->addButton(btnSkinSafari[i], i); // id 0=Esmeralda,1=Boa,2=Cascabel,3=Marina
    }
    btnSkinSafari[0]->setIcon(QIcon(":/Recursos/EsmeraldaHeadSafari.png"));
    btnSkinSafari[0]->setEnabled(true); // siempre disponible
    btnSkinSafari[0]->setChecked(true); // seleccionado por defecto

    // --- Selector de muros (2 opciones, exclusivas) ---
    grupoMurosSafari = new QButtonGroup(this);
    grupoMurosSafari->setExclusive(true);
    QString textosMuros[2] = {"Atravesable", "Mortal"};
    for (int i = 0; i < 2; i++) {
        btnMurosSafari[i] = new QPushButton(textosMuros[i], paginaSafariSetup);
        btnMurosSafari[i]->setCheckable(true);
        btnMurosSafari[i]->setGeometry(100 + i * 300, 300, 250, 60);
        btnMurosSafari[i]->setStyleSheet(
            "QPushButton { background: rgb(15,58,13); color: white; border: 3px solid rgb(143,208,53); font-size: 18px; }"
            "QPushButton:checked { background: rgb(143,208,53); color: black; }"
            );
        grupoMurosSafari->addButton(btnMurosSafari[i], i); // 0=Atravesable, 1=Mortal
    }
    btnMurosSafari[0]->setChecked(true);

    // --- Selector de vidas (2 opciones, exclusivas) ---
    grupoVidasSafari = new QButtonGroup(this);
    grupoVidasSafari->setExclusive(true);
    QString textosVidas[2] = {"Normal (3 vidas)", "Hardcore (1 vida)"};
    for (int i = 0; i < 2; i++) {
        btnVidasSafari[i] = new QPushButton(textosVidas[i], paginaSafariSetup);
        btnVidasSafari[i]->setCheckable(true);
        btnVidasSafari[i]->setGeometry(100 + i * 300, 390, 250, 60);
        btnVidasSafari[i]->setStyleSheet(
            "QPushButton { background: rgb(15,58,13); color: white; border: 3px solid rgb(143,208,53); font-size: 16px; }"
            "QPushButton:checked { background: rgb(143,208,53); color: black; }"
            );
        grupoVidasSafari->addButton(btnVidasSafari[i], i); // 0=Normal(3), 1=Hardcore(1)
    }
    btnVidasSafari[0]->setChecked(true);

    // --- Checkboxes de power-ups (selección múltiple) ---
    QString nombresPowerUp[4] = {"Rana", "Ratón", "Pez payaso", "Pez globo"};
    for (int i = 0; i < 4; i++) {
        chkPowerUpSafari[i] = new QCheckBox(nombresPowerUp[i], paginaSafariSetup);
        chkPowerUpSafari[i]->setGeometry(100 + (i % 2) * 300, 470 + (i / 2) * 40, 250, 30);
        chkPowerUpSafari[i]->setStyleSheet(
            QString("color: white; font-family: '%1'; font-size: 16px;").arg(familiaFuente)
            );
        chkPowerUpSafari[i]->setChecked(true); // todos activos por defecto
    }

    // --- Botón Jugar ---
    QPushButton *btnJugarSafari = new QPushButton("JUGAR", paginaSafariSetup);
    btnJugarSafari->setGeometry(250, 570, 300, 70);
    btnJugarSafari->setStyleSheet(
        QString("background: rgb(143,208,53); color: black; font-size: 24px; font-family: '%1'; border-radius: 10px;").arg(familiaFuente)
        );
    connect(btnJugarSafari, &QPushButton::clicked, this, [this](){
        int skin = grupoSkinSafari->checkedId();
        if (skin != 0 && !jugadorActual.haCompletadoNivel(skin)) return; // seguridad extra
        iniciarSafari();
    });

    // --- Botón Volver ---
    QPushButton *btnVolverSafari = new QPushButton(paginaSafariSetup);
    btnVolverSafari->setIcon(QIcon(":/Recursos/UsernameSalir.png"));
    btnVolverSafari->setIconSize(QSize(278, 70));
    btnVolverSafari->setGeometry(261, 655, 278, 70);
    btnVolverSafari->setFlat(true);
    btnVolverSafari->setStyleSheet("border: none; background: transparent;");
    connect(btnVolverSafari, &QPushButton::clicked, this, [this](){
        stack->setCurrentWidget(paginaNiveles);
    });

    stack->addWidget(paginaSafariSetup);
}
void MainWindow::iniciarSafari()
{
    modoSafariActivo = true;
    nivelJugadoActual = 0;

    int skin = grupoSkinSafari->checkedId();
    bool murosMortales = (grupoMurosSafari->checkedId() == 1);
    int vidasIniciales = (grupoVidasSafari->checkedId() == 0) ? 3 : 1;

    modoInfinitoActual = !murosMortales;

    if (cicloColoresNivel != nullptr) delete[] cicloColoresNivel;

    if (skin == 0) { // Esmeralda, default
        rutaCabezaD = ":/Recursos/EsmeraldaHeadSafari.png"; rutaCabezaI = ":/Recursos/EsmeraldaHeadLeftSafari.png";
        rutaColaD = ":/Recursos/EsmeraldaColaLeftSafari.png"; rutaColaI = ":/Recursos/EsmeraldaColaSafari.png";
        cantidadColoresCiclo = 1;
        cicloColoresNivel = new QString[1]{":/Recursos/EsmeraldaNodoSafari.png"};
    } else if (skin == 1) {
        rutaCabezaD = ":/Recursos/BoaHead.png"; rutaCabezaI = ":/Recursos/BoaHeadLeft.png";
        rutaColaD = ":/Recursos/BoaColaLeft.png"; rutaColaI = ":/Recursos/BoaCola.png";
        cantidadColoresCiclo = 3;
        cicloColoresNivel = new QString[3]{":/Recursos/NodoNegroLvl1.png", ":/Recursos/NodoAmarilloLvl1.png", ":/Recursos/NodoNaranjaLvl1.png"};
    } else if (skin == 2) {
        rutaCabezaD = ":/Recursos/CascabelHeadSafari.png"; rutaCabezaI = ":/Recursos/CascabelHeadLeftSafari.png";
        rutaColaD = ":/Recursos/CascabelColaLeftSafari.png"; rutaColaI = ":/Recursos/CascabelColaSafari.png";
        cantidadColoresCiclo = 1;
        cicloColoresNivel = new QString[1]{":/Recursos/NodoLvl2Safari.png"};
    } else {
        rutaCabezaD = ":/Recursos/MarinaHeadSafari.png"; rutaCabezaI = ":/Recursos/MarinaHeadLeftSafari.png";
        rutaColaD = ":/Recursos/MarinaColaSafari.png"; rutaColaI = ":/Recursos/MarinaColaSafari.png";
        cantidadColoresCiclo = 2;
        cicloColoresNivel = new QString[2]{":/Recursos/NodoAzulLvl3Safari.png", ":/Recursos/NodoNegroLvl3Safari.png"};
    }

    // --- Power-ups seleccionados ---
    QString rutasDisponibles[4] = {":/Recursos/RanaLvl1.png", ":/Recursos/RatonSafari.png", ":/Recursos/PezPayasoSafari.png", ":/Recursos/PezGloboSafari.png"};
    int tiposDisponibles[4] = {0, 1, 2, 3};

    if (rutasPowerUpVariantes != nullptr) { delete[] rutasPowerUpVariantes; delete[] tiposPowerUpVariantes; }
    int cantidadSeleccionados = 0;
    for (int i = 0; i < 4; i++) if (chkPowerUpSafari[i]->isChecked()) cantidadSeleccionados++;
    if (cantidadSeleccionados == 0) cantidadSeleccionados = 4; // seguridad: si no marcó ninguno, usa los 4

    cantidadVariantesPowerUp = cantidadSeleccionados;
    rutasPowerUpVariantes = new QString[cantidadSeleccionados];
    tiposPowerUpVariantes = new int[cantidadSeleccionados];
    int idx = 0;
    for (int i = 0; i < 4; i++) {
        if (chkPowerUpSafari[i]->isChecked() || cantidadSeleccionados == 4) {
            rutasPowerUpVariantes[idx] = rutasDisponibles[i];
            tiposPowerUpVariantes[idx] = tiposDisponibles[i];
            idx++;
        }
    }

    powerUpVisible = false; contadorPowerUp = 0; itemPowerUp = nullptr;
    ralentizadoActivo = false; contadorRalentizado = 0;

    origenXCuadricula = 50; origenYCuadricula = 115; tamanoCeldaActual = 50;
    frutasComidas = 0;
    vidasRestantes = vidasIniciales;
    lblValorVidas->setText(QString::number(vidasRestantes));
    segundosTranscurridos = 0;
    lblValorTiempo->setText("00:00");

    tableroJuego.configurarNivel(14, 10, 50);
    if (murosMortales) tableroJuego.generarMurosPerimetro();

    serpienteJuego.inicializar(7, 5);
    comidaJuego.generarNuevaPosicion(tableroJuego, serpienteJuego);

    escenaJuego->clear();
    QGraphicsPixmapItem *fondo = escenaJuego->addPixmap(QPixmap(":/Recursos/SafariBackground.png"));
    fondo->setPos(0, 0);
    dibujarGridPermanente(QColor(255, 255, 255, 80));
    if (murosMortales) {
        dibujarMuros(":/Recursos/RocaSafari.png");
    }
    fondo->setZValue(-1);

    QPixmap pixmapComidaInicial(":/Recursos/Manzana.png");
    pixmapComidaInicial = pixmapComidaInicial.scaled(tamanoCeldaActual, tamanoCeldaActual, Qt::KeepAspectRatio, Qt::SmoothTransformation);
    itemComida = escenaJuego->addPixmap(pixmapComidaInicial);
    itemComida->setZValue(1);
    itemComida->setPos(origenXCuadricula + comidaJuego.getX() * tamanoCeldaActual, origenYCuadricula + comidaJuego.getY() * tamanoCeldaActual);

    segmentosVisuales = nullptr;
    cantidadSegmentosVisuales = 0;
    redibujarSerpiente();

    lblValorPuntos->setGeometry(131, 59, 200, 40);
    lblValorPuntos->setStyleSheet(QString("color: white; font-family: '%1'; font-size: 27px;").arg(familiaFuente));
    lblValorPuntos->setText("Frutas: 0");

    actualizarIconosPausa(":/Recursos/PauseVolumen.png", ":/Recursos/PlayVolumen.png", ":/Recursos/PauseVolumen.png");
    btnPausaJuego->setGeometry(740, 45, 40, 40);

    timerReloj->start();
    timerJuego->start(150);
    vistaJuego->setFocus();
    stack->setCurrentWidget(paginaJuego);
}
void MainWindow::mostrarToastSafari(QString texto)
{
    lblToastSafari->setText(texto);
    lblToastSafari->setVisible(true);
    lblToastSafari->raise();
    QTimer::singleShot(2200, this, [this](){ lblToastSafari->setVisible(false); });
}

void MainWindow::finalizarSafariPorDerrota()
{
    timerJuego->stop();
    timerReloj->stop();
     modoSafariActivo = false;
    mostrarToastSafari("Te quedaste sin vidas.\nSafaris completados: " + QString::number(jugadorActual.getCantidadSafarisCompletados()));
    QTimer::singleShot(2500, this, [this](){ stack->setCurrentWidget(paginaMenuPrincipal); });
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
        actualizarBotonesAlbums();
        abrirAlbum(nivelJugadoActual);
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
        else if (nivelJugadoActual == 3) iniciarNivel3();
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
                        : (nivel == 3) ? ":/Recursos/PerdisteLvl3.png"
                                       : ":/Recursos/PerdisteLvl1.png"; // fallback seguro, nunca debería llegar aquí en el flujo actual

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

    jugadorActual.actualizarRecord(frutasComidas, segundosTranscurridos);
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

void MainWindow::crearPaginaRanking()
{
    paginaRanking = new QWidget();

    QLabel *fondo = new QLabel(paginaRanking);
    fondo->setStyleSheet("background-color: rgb(15,58,13);"); // reemplaza por tu imagen cuando la tengas
    fondo->setGeometry(0, 0, 800, 700);
    fondo->lower();

    grupoTabsRanking = new QButtonGroup(this);
    grupoTabsRanking->setExclusive(true);
    QString nombresTabs[4] = {"Nivel 1", "Nivel 2", "Nivel 3", "Safari"};
    for (int i = 0; i < 4; i++) {
        btnTabRanking[i] = new QPushButton(nombresTabs[i], paginaRanking);
        btnTabRanking[i]->setCheckable(true);
        btnTabRanking[i]->setGeometry(20 + i * 195, 20, 180, 50);
        btnTabRanking[i]->setStyleSheet(
            "QPushButton { background: rgb(15,58,13); color: white; border: 3px solid rgb(143,208,53); font-size: 16px; }"
            "QPushButton:checked { background: rgb(143,208,53); color: black; }"
            );
        grupoTabsRanking->addButton(btnTabRanking[i], i);
        connect(btnTabRanking[i], &QPushButton::clicked, this, [this, i](){ mostrarRanking(i); });
    }
    btnTabRanking[0]->setChecked(true);

    contenedorFilasRanking = new QWidget(paginaRanking);
    contenedorFilasRanking->setGeometry(50, 100, 700, 500);
    layoutFilasRanking = new QVBoxLayout(contenedorFilasRanking);
    layoutFilasRanking->setAlignment(Qt::AlignTop);

    QPushButton *btnVolverRanking = new QPushButton(paginaRanking);
    btnVolverRanking->setIcon(QIcon(":/Recursos/UsernameSalir.png"));
    btnVolverRanking->setIconSize(QSize(278, 70));
    btnVolverRanking->setGeometry(261, 615, 278, 70);
    btnVolverRanking->setFlat(true);
    btnVolverRanking->setStyleSheet("border: none; background: transparent;");
    connect(btnVolverRanking, &QPushButton::clicked, this, [this](){
        stack->setCurrentWidget(paginaMenuPrincipal);
    });

    stack->addWidget(paginaRanking);
}

void MainWindow::mostrarRanking(int tab)
{
    QLayoutItem *item;
    while ((item = layoutFilasRanking->takeAt(0)) != nullptr) {
        delete item->widget();
        delete item;
    }

    if (tab < 3) {
        int nivel = tab + 1;
        std::vector<Jugador> ranking = gestorArchivos->obtenerRankingNivel(nivel, 10);
        for (int i = 0; i < (int)ranking.size(); i++) {
            if (!ranking[i].haCompletadoNivel(nivel)) continue;
            int tiempo = ranking[i].getTiempoNivel(nivel);
            if (tiempo <= 0) continue;
            int minutos = tiempo / 60;
            int segundos = tiempo % 60;
            QString texto = QString("%1. %2 — %3:%4")
                                .arg(i + 1)
                                .arg(QString::fromStdString(ranking[i].getNombre()))
                                .arg(minutos, 2, 10, QChar('0'))
                                .arg(segundos, 2, 10, QChar('0'));
            QLabel *fila = new QLabel(texto, contenedorFilasRanking);
            fila->setStyleSheet(QString("color: white; font-family: '%1'; font-size: 20px;").arg(familiaFuente));
            layoutFilasRanking->addWidget(fila);
        }
        if (ranking.empty()) {
            QLabel *vacio = new QLabel("Nadie ha completado este nivel todavía.", contenedorFilasRanking);
            vacio->setStyleSheet(QString("color: white; font-family: '%1'; font-size: 18px;").arg(familiaFuente));
            layoutFilasRanking->addWidget(vacio);
        }
    } else {
        std::vector<Jugador> ranking = gestorArchivos->obtenerRankingSafari(10);
        for (int i = 0; i < (int)ranking.size(); i++) {
            QString texto = QString("%1. %2 — %3 Modos Safari completados (%4)")
                                .arg(i + 1)
                                .arg(QString::fromStdString(ranking[i].getNombre()))
                                .arg(ranking[i].getCantidadSafarisCompletados())
                                .arg(ranking[i].getMedallaSafari());
            QLabel *fila = new QLabel(texto, contenedorFilasRanking);
            fila->setStyleSheet(QString("color: white; font-family: '%1'; font-size: 20px;").arg(familiaFuente));
            layoutFilasRanking->addWidget(fila);
        }
        if (ranking.empty()) {
            QLabel *vacio = new QLabel("Nadie ha completado un Safari todavía.", contenedorFilasRanking);
            vacio->setStyleSheet(QString("color: white; font-family: '%1'; font-size: 18px;").arg(familiaFuente));
            layoutFilasRanking->addWidget(vacio);
        }
    }
}

MainWindow::~MainWindow()
{
    delete gestorArchivos;
}


// ======================= ÁLBUMS =======================

bool MainWindow::nivelCompletado(int nivel) const
{
    return jugadorActual.haCompletadoNivel(nivel);
}

void MainWindow::crearPaginaAlbums()
{
    paginaAlbums = new QWidget();

    QLabel *fondo = new QLabel(paginaAlbums);
    fondo->setPixmap(QPixmap(":/Recursos/PantallaAlbums.png"));
    fondo->setScaledContents(true); // la imagen es 1097x960, se ajusta a 800x700
    fondo->setGeometry(0, 0, 800, 700);
    fondo->lower();

    const int posX[3] = {39, 290, 540}; // 39.3, 289.6 y 540.4 redondeados
    const int posY = 280;

    for (int i = 0; i < 3; i++) {
        btnAlbumNivel[i] = new QPushButton(paginaAlbums);
        btnAlbumNivel[i]->setIconSize(QSize(221, 221));
        btnAlbumNivel[i]->setGeometry(posX[i], posY, 221, 221);
        btnAlbumNivel[i]->setFlat(true);
        btnAlbumNivel[i]->setStyleSheet("border: none; background: transparent;");

        connect(btnAlbumNivel[i], &QPushButton::clicked, this, [this, i](){
            if (nivelCompletado(i + 1)) abrirAlbum(i + 1); // con candado no hace nada
        });
    }

    QPushButton *btnVolver = new QPushButton(paginaAlbums);
    btnVolver->setIcon(QIcon(":/Recursos/UsernameSalir.png"));
    btnVolver->setIconSize(QSize(278, 70));
    btnVolver->setGeometry(261, 585, 278, 70);
    btnVolver->setFlat(true);
    btnVolver->setStyleSheet("border: none; background: transparent;");
    connect(btnVolver, &QPushButton::clicked, this, [this](){
        stack->setCurrentWidget(paginaMenuPrincipal);
    });

    actualizarBotonesAlbums();
    stack->addWidget(paginaAlbums);
}

void MainWindow::actualizarBotonesAlbums()
{
    for (int i = 0; i < 3; i++) {
        int nivel = i + 1;
        QString estado = nivelCompletado(nivel) ? "SinCandado" : "ConCandado";
        btnAlbumNivel[i]->setIcon(QIcon(QString(":/Recursos/AlbumNivel%1%2.png").arg(nivel).arg(estado)));
    }
}

void MainWindow::crearPaginaAlbumNivel(int nivel)
{
    QWidget *pagina = new QWidget();

    QLabel *fondo = new QLabel(pagina);
    fondo->setPixmap(QPixmap(QString(":/Recursos/PantallaAlbum%1.png").arg(nivel)));
    fondo->setGeometry(0, 0, 800, 700);
    fondo->lower();

    QPushButton *btnVolver = new QPushButton("< VOLVER", pagina);
    btnVolver->setGeometry(12, 10, 130, 40);
    btnVolver->setCursor(Qt::PointingHandCursor);
    btnVolver->setStyleSheet(QString("color: white; background: rgba(0, 0, 0, 150); border: none; "
                                     "font-family: '%1'; font-size: 22px;").arg(familiaFuente));
    connect(btnVolver, &QPushButton::clicked, this, [this](){
        stack->setCurrentWidget(paginaAlbums);
    });

    paginaAlbumNivel[nivel - 1] = pagina;
    stack->addWidget(pagina);
}

void MainWindow::abrirAlbum(int nivel)
{
    if (nivel < 1 || nivel > 3) return;
    stack->setCurrentWidget(paginaAlbumNivel[nivel - 1]);
}
