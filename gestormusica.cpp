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
    reproductor->setLoops(QMediaPlayer::Infinite); // repetir la canción sin fin
    reproductor->setSource(QUrl("qrc:/Recursos/musica.mp3"));

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
