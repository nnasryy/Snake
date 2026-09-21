#ifndef CONFIGURACIONNIVEL_H
#define CONFIGURACIONNIVEL_H

#include <QString>
#include <QColor>
#include <QRect>

class ConfiguracionNivel
{
public:
    ConfiguracionNivel();
    ~ConfiguracionNivel();

    ConfiguracionNivel(const ConfiguracionNivel&) = delete;
    ConfiguracionNivel& operator=(const ConfiguracionNivel&) = delete;

    void establecer(int nivel, int columnas, int filas, int tamanoCelda,
                    int metaFrutas, bool modoInfinito, int intervaloInicial,
                    int origenX, int origenY,
                    QString rutaFondo, QString rutaComida,
                    QString cabezaD, QString cabezaI, QString colaD, QString colaI);

    void establecerColoresCuerpo(QString* colores, int cantidad);
    void establecerMuro(QString rutaMuro, bool generarBloquesInternos);
    void establecerPowerUps(QString* rutas, int* tipos, int cantidad);
    void establecerVidas(bool tieneVidas, int vidasIniciales);
    void establecerBloquesMoviles(bool tieneBloques);
    void establecerColorGrid(QColor color);
    void establecerHUD(QColor colorTexto, QRect rectPuntos, QRect rectVidas, QRect rectTiempo,
                       int fuentePuntos, int fuenteVidas, int fuenteTiempo);
    void establecerIconosPausa(QString iconoPausa, QString volumenPlay, QString volumenPause);

    int getNivel() const;
    int getColumnas() const;
    int getFilas() const;
    int getTamanoCelda() const;
    int getMetaFrutas() const;
    bool getModoInfinito() const;
    int getIntervaloInicial() const;
    int getOrigenX() const;
    int getOrigenY() const;
    QString getRutaFondo() const;
    QString getRutaComida() const;
    QString getCabezaD() const;
    QString getCabezaI() const;
    QString getColaD() const;
    QString getColaI() const;

    QString* getColoresCuerpo() const;
    int getCantidadColores() const;
    QString getRutaMuro() const;
    bool getGeneraBloquesInternos() const;
    QString* getRutasPowerUp() const;
    int* getTiposPowerUp() const;
    int getCantidadPowerUps() const;
    bool getTieneVidas() const;
    int getVidasIniciales() const;
    bool getTieneBloquesMoviles() const;
    QColor getColorGrid() const;
    QColor getColorTextoHUD() const;
    QRect getRectPuntos() const;
    QRect getRectVidas() const;
    QRect getRectTiempo() const;
    int getFuentePuntos() const;
    int getFuenteVidas() const;
    int getFuenteTiempo() const;
    QString getIconoPausa() const;
    QString getVolumenPlay() const;
    QString getVolumenPause() const;

private:
    int nivel, columnas, filas, tamanoCelda, metaFrutas, intervaloInicial, origenX, origenY;
    bool modoInfinito;
    QString rutaFondo, rutaComida, cabezaD, cabezaI, colaD, colaI;

    QString* coloresCuerpo;
    int cantidadColores;

    QString rutaMuro;
    bool generaBloquesInternos;

    QString* rutasPowerUp;
    int* tiposPowerUp;
    int cantidadPowerUps;

    bool tieneVidas;
    int vidasIniciales;
    bool tieneBloquesMoviles;

    QColor colorGrid;
    QColor colorTextoHUD;
    QRect rectPuntos, rectVidas, rectTiempo;
    int fuentePuntos, fuenteVidas, fuenteTiempo;

    QString iconoPausa, volumenPlay, volumenPause;
};

#endif