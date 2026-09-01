#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QStackedWidget>
#include <QPushButton>
#include <QLineEdit>

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

    QString familiaFuente;   // nombre real de la fuente Pixellari ya cargada
    QLineEdit *campoNombre;  // lo guardamos como miembro para validarlo desde el botón

    void crearPaginaInicio();
    void crearPaginaUsername();
    void validarNombre();
    void crearPaginaMenuPrincipal();
};

#endif // MAINWINDOW_H