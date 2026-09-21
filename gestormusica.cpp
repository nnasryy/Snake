#include "gestormusica.h"
#include <QUrl>
#include <QDebug>

GestorMusica::GestorMusica(QObject *parent)
    : QObject(parent), sonando(false)
{
    salida = new QAudioOutput(this);
    salida->setVolume(0.5f);                       // 0.0 a 1.0

    reproductor = new QMediaPlayer(this);
    reproductor->setAudioOutput(salida);
    reproductor->setSource(QUrl("qrc:/Recursos/musica.mp3"));

    // Loop manual: setLoops(Infinite) falla con algunos mp3, así que al llegar
    // al final volvemos al inicio nosotros mismos (solo si no está en pausa).
    connect(reproductor, &QMediaPlayer::mediaStatusChanged, this,
            [this](QMediaPlayer::MediaStatus estado){
                if (estado == QMediaPlayer::EndOfMedia && sonando) {
                    reproductor->setPosition(0);
                    reproductor->play();
                }
            });

    connect(reproductor, &QMediaPlayer::errorOccurred, this,
            [](QMediaPlayer::Error, const QString &mensaje){
                qDebug() << "Error de audio:" << mensaje;
            });
}

bool GestorMusica::estaSonando() const
{
    return sonando;
}

void GestorMusica::reproducir()
{
    if (sonando) return;
    reproductor->play();
    sonando = true;
    emit estadoCambio(sonando);
}

void GestorMusica::pausar()
{
    if (!sonando) return;
    reproductor->pause(); // pause() (no stop()) para continuar donde se quedó
    sonando = false;
    emit estadoCambio(sonando);
}

void GestorMusica::alternar()
{
    if (sonando) pausar();
    else reproducir();
}
