#include "mainwindow.h"
#include <QVBoxLayout>
#include <QLabel>
#include <QApplication>
#include <QDebug>
#include <QDirIterator>
#include <QFontDatabase>

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


    // --- Fondo ---
    QLabel *fondo = new QLabel(paginaInicio);
    fondo->setPixmap(QPixmap(":/Recursos/SnakeInicio.png"));
    fondo->setGeometry(0, 0, 800, 700);
    fondo->lower();

    // --- Botón Play ---
    QPushButton *btnJugar = new QPushButton(paginaInicio);
    btnJugar->setIcon(QIcon(":/Recursos/PlayButton.png"));
    btnJugar->setIconSize(QSize(294, 124));
    btnJugar->setGeometry(96, 445, 294, 124);
    btnJugar->setFlat(true);
    btnJugar->setStyleSheet("border: none; background: transparent;");

    // --- Botón Exit ---
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

    // --- Fondo ---
    QLabel *fondo = new QLabel(paginaUsername);
    fondo->setPixmap(QPixmap(":/Recursos/UsernamePantalla.png"));
    fondo->setGeometry(0, 0, 800, 700);
    fondo->lower();

    // --- Campo de texto ---
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

    // --- Botón Salir (vuelve al menú) ---
    QPushButton *btnSalir = new QPushButton(paginaUsername);
    btnSalir->setIcon(QIcon(":/Recursos/UsernameSalir.png"));
    btnSalir->setIconSize(QSize(278, 70));
    btnSalir->setGeometry(66, 462, 278, 70);
    btnSalir->setFlat(true);
    btnSalir->setStyleSheet("border: none; background: transparent;");

    connect(btnSalir, &QPushButton::clicked, this, [this](){
        stack->setCurrentWidget(paginaInicio);
    });

    // --- Botón Confirmar ---
    QPushButton *btnConfirmar = new QPushButton(paginaUsername);
    btnConfirmar->setIcon(QIcon(":/Recursos/UsernameConfirmar.png"));
    btnConfirmar->setIconSize(QSize(278, 70));
    btnConfirmar->setGeometry(457, 462, 278, 70);
    btnConfirmar->setFlat(true);
    btnConfirmar->setStyleSheet("border: none; background: transparent;");

    connect(btnConfirmar, &QPushButton::clicked, this, &MainWindow::validarNombre);
    connect(campoNombre, &QLineEdit::returnPressed, this, &MainWindow::validarNombre); // Enter también confirma

    // --- Botón Volumen (igual que en el menú) ---
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
    QString nombre = campoNombre->text().trimmed();

    if (nombre.isEmpty()) {
        // Feedback visual: borde y texto en rojo
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

    // Nombre válido: aquí más adelante llamamos a GestorArchivos
    // para revisar si ya existe y cargar sus récords/skins.
    qDebug() << "Nombre confirmado:" << nombre;

   stack->setCurrentWidget(paginaMenuPrincipal);
}

void MainWindow::crearPaginaMenuPrincipal()
{
    paginaMenuPrincipal = new QWidget();

    // --- Fondo ---
    QLabel *fondo = new QLabel(paginaMenuPrincipal);
    fondo->setPixmap(QPixmap(":/Recursos/MenuPantalla.png"));
    fondo->setGeometry(0, 0, 800, 700);
    fondo->lower();

    // --- Botón Jugar (lleva a ingreso de nombre) ---
    QPushButton *btnJugar = new QPushButton(paginaMenuPrincipal);
    btnJugar->setIcon(QIcon(":/Recursos/MenuJugar.png"));
    btnJugar->setIconSize(QSize(278, 70));
    btnJugar->setGeometry(94, 302, 278, 70);
    btnJugar->setFlat(true);
    btnJugar->setStyleSheet("border: none; background: transparent;");

    connect(btnJugar, &QPushButton::clicked, this, [this](){
        stack->setCurrentWidget(paginaUsername);
    });

    // --- Botón Instrucciones ---
    QPushButton *btnInstrucciones = new QPushButton(paginaMenuPrincipal);
    btnInstrucciones->setIcon(QIcon(":/Recursos/MenuInstrucciones.png"));
    btnInstrucciones->setIconSize(QSize(278, 70));
    btnInstrucciones->setGeometry(425, 302, 278, 70);
    btnInstrucciones->setFlat(true);
    btnInstrucciones->setStyleSheet("border: none; background: transparent;");

    connect(btnInstrucciones, &QPushButton::clicked, this, [](){
        qDebug() << "Botón Instrucciones presionado (pantalla pendiente)";
    });

    // --- Botón Récords ---
    QPushButton *btnRecords = new QPushButton(paginaMenuPrincipal);
    btnRecords->setIcon(QIcon(":/Recursos/MenuRecords.png"));
    btnRecords->setIconSize(QSize(278, 70));
    btnRecords->setGeometry(92, 390, 278, 70);
    btnRecords->setFlat(true);
    btnRecords->setStyleSheet("border: none; background: transparent;");

    connect(btnRecords, &QPushButton::clicked, this, [](){
        qDebug() << "Botón Récords presionado (pantalla pendiente)";
    });

    // --- Botón Opciones ---
    QPushButton *btnOpciones = new QPushButton(paginaMenuPrincipal);
    btnOpciones->setIcon(QIcon(":/Recursos/MenuOpciones.png"));
    btnOpciones->setIconSize(QSize(278, 70));
    btnOpciones->setGeometry(428, 390, 278, 70);
    btnOpciones->setFlat(true);
    btnOpciones->setStyleSheet("border: none; background: transparent;");

    connect(btnOpciones, &QPushButton::clicked, this, [](){
        qDebug() << "Botón Opciones presionado (pantalla pendiente)";
    });

    // --- Botón Salir ---
    QPushButton *btnSalir = new QPushButton(paginaMenuPrincipal);
    btnSalir->setIcon(QIcon(":/Recursos/UsernameSalir.png"));
    btnSalir->setIconSize(QSize(278, 70));
    btnSalir->setGeometry(268, 480, 278, 70);
    btnSalir->setFlat(true);
    btnSalir->setStyleSheet("border: none; background: transparent;");

    connect(btnSalir, &QPushButton::clicked, this, [this](){
        stack->setCurrentWidget(paginaUsername);
    });

    // --- Botón Volumen ---
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




MainWindow::~MainWindow()
{
    // Qt libera automáticamente los widgets hijos del padre (this)
}