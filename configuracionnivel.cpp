#include "configuracionnivel.h"

ConfiguracionNivel::ConfiguracionNivel()
{
    nivel = 0; columnas = 0; filas = 0; tamanoCelda = 0;
    metaFrutas = 0; intervaloInicial = 0; origenX = 0; origenY = 0;
    modoInfinito = false;

    coloresCuerpo = nullptr;
    cantidadColores = 0;

    generaBloquesInternos = false;

    rutasPowerUp = nullptr;
    tiposPowerUp = nullptr;
    cantidadPowerUps = 0;

    tieneVidas = false;
    vidasIniciales = 0;
    tieneBloquesMoviles = false;

    colorGrid = QColor(255, 255, 255, 100);
    colorTextoHUD = QColor(143, 208, 53);
    rectPuntos = QRect(131, 59, 100, 40);
    rectVidas = QRect(306, 59, 60, 40);
    rectTiempo = QRect(665, 59, 100, 40);
    fuentePuntos = 27; fuenteVidas = 31; fuenteTiempo = 26;

    iconoPausa = ":/Recursos/PauseVolumen.png";
    volumenPlay = ":/Recursos/PlayVolumen.png";
    volumenPause = ":/Recursos/PauseVolumen.png";
}

ConfiguracionNivel::~ConfiguracionNivel()
{
    if (coloresCuerpo != nullptr) delete[] coloresCuerpo;
    if (rutasPowerUp != nullptr) delete[] rutasPowerUp;
    if (tiposPowerUp != nullptr) delete[] tiposPowerUp;
}

void ConfiguracionNivel::establecer(int p_nivel, int p_columnas, int p_filas, int p_tamanoCelda,
                                    int p_metaFrutas, bool p_modoInfinito, int p_intervaloInicial,
                                    int p_origenX, int p_origenY,
                                    QString p_rutaFondo, QString p_rutaComida,
                                    QString p_cabezaD, QString p_cabezaI, QString p_colaD, QString p_colaI)
{
    nivel = p_nivel;
    columnas = p_columnas;
    filas = p_filas;
    tamanoCelda = p_tamanoCelda;
    metaFrutas = p_metaFrutas;
    modoInfinito = p_modoInfinito;
    intervaloInicial = p_intervaloInicial;
    origenX = p_origenX;
    origenY = p_origenY;
    rutaFondo = p_rutaFondo;
    rutaComida = p_rutaComida;
    cabezaD = p_cabezaD;
    cabezaI = p_cabezaI;
    colaD = p_colaD;
    colaI = p_colaI;
}

void ConfiguracionNivel::establecerColoresCuerpo(QString* colores, int cantidad)
{
    if (coloresCuerpo != nullptr) delete[] coloresCuerpo;
    cantidadColores = cantidad;
    coloresCuerpo = new QString[cantidad];
    for (int i = 0; i < cantidad; i++) coloresCuerpo[i] = colores[i];
}

void ConfiguracionNivel::establecerMuro(QString p_rutaMuro, bool p_generarBloquesInternos)
{
    rutaMuro = p_rutaMuro;
    generaBloquesInternos = p_generarBloquesInternos;
}

void ConfiguracionNivel::establecerPowerUps(QString* rutas, int* tipos, int cantidad)
{
    if (rutasPowerUp != nullptr) delete[] rutasPowerUp;
    if (tiposPowerUp != nullptr) delete[] tiposPowerUp;
    cantidadPowerUps = cantidad;
    rutasPowerUp = new QString[cantidad];
    tiposPowerUp = new int[cantidad];
    for (int i = 0; i < cantidad; i++) {
        rutasPowerUp[i] = rutas[i];
        tiposPowerUp[i] = tipos[i];
    }
}

void ConfiguracionNivel::establecerVidas(bool p_tieneVidas, int p_vidasIniciales)
{
    tieneVidas = p_tieneVidas;
    vidasIniciales = p_vidasIniciales;
}

void ConfiguracionNivel::establecerBloquesMoviles(bool p_tieneBloques) { tieneBloquesMoviles = p_tieneBloques; }
void ConfiguracionNivel::establecerColorGrid(QColor color) { colorGrid = color; }

void ConfiguracionNivel::establecerHUD(QColor colorTexto, QRect p_rectPuntos, QRect p_rectVidas, QRect p_rectTiempo,
                                       int p_fuentePuntos, int p_fuenteVidas, int p_fuenteTiempo)
{
    colorTextoHUD = colorTexto;
    rectPuntos = p_rectPuntos;
    rectVidas = p_rectVidas;
    rectTiempo = p_rectTiempo;
    fuentePuntos = p_fuentePuntos;
    fuenteVidas = p_fuenteVidas;
    fuenteTiempo = p_fuenteTiempo;
}

void ConfiguracionNivel::establecerIconosPausa(QString p_iconoPausa, QString p_volumenPlay, QString p_volumenPause)
{
    iconoPausa = p_iconoPausa;
    volumenPlay = p_volumenPlay;
    volumenPause = p_volumenPause;
}

int ConfiguracionNivel::getNivel() const { return nivel; }
int ConfiguracionNivel::getColumnas() const { return columnas; }
int ConfiguracionNivel::getFilas() const { return filas; }
int ConfiguracionNivel::getTamanoCelda() const { return tamanoCelda; }
int ConfiguracionNivel::getMetaFrutas() const { return metaFrutas; }
bool ConfiguracionNivel::getModoInfinito() const { return modoInfinito; }
int ConfiguracionNivel::getIntervaloInicial() const { return intervaloInicial; }
int ConfiguracionNivel::getOrigenX() const { return origenX; }
int ConfiguracionNivel::getOrigenY() const { return origenY; }
QString ConfiguracionNivel::getRutaFondo() const { return rutaFondo; }
QString ConfiguracionNivel::getRutaComida() const { return rutaComida; }
QString ConfiguracionNivel::getCabezaD() const { return cabezaD; }
QString ConfiguracionNivel::getCabezaI() const { return cabezaI; }
QString ConfiguracionNivel::getColaD() const { return colaD; }
QString ConfiguracionNivel::getColaI() const { return colaI; }

QString* ConfiguracionNivel::getColoresCuerpo() const { return coloresCuerpo; }
int ConfiguracionNivel::getCantidadColores() const { return cantidadColores; }
QString ConfiguracionNivel::getRutaMuro() const { return rutaMuro; }
bool ConfiguracionNivel::getGeneraBloquesInternos() const { return generaBloquesInternos; }
QString* ConfiguracionNivel::getRutasPowerUp() const { return rutasPowerUp; }
int* ConfiguracionNivel::getTiposPowerUp() const { return tiposPowerUp; }
int ConfiguracionNivel::getCantidadPowerUps() const { return cantidadPowerUps; }
bool ConfiguracionNivel::getTieneVidas() const { return tieneVidas; }
int ConfiguracionNivel::getVidasIniciales() const { return vidasIniciales; }
bool ConfiguracionNivel::getTieneBloquesMoviles() const { return tieneBloquesMoviles; }
QColor ConfiguracionNivel::getColorGrid() const { return colorGrid; }
QColor ConfiguracionNivel::getColorTextoHUD() const { return colorTextoHUD; }
QRect ConfiguracionNivel::getRectPuntos() const { return rectPuntos; }
QRect ConfiguracionNivel::getRectVidas() const { return rectVidas; }
QRect ConfiguracionNivel::getRectTiempo() const { return rectTiempo; }
int ConfiguracionNivel::getFuentePuntos() const { return fuentePuntos; }
int ConfiguracionNivel::getFuenteVidas() const { return fuenteVidas; }
int ConfiguracionNivel::getFuenteTiempo() const { return fuenteTiempo; }
QString ConfiguracionNivel::getIconoPausa() const { return iconoPausa; }
QString ConfiguracionNivel::getVolumenPlay() const { return volumenPlay; }
QString ConfiguracionNivel::getVolumenPause() const { return volumenPause; }