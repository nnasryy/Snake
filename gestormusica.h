#ifndef GESTORMUSICA_H
#define GESTORMUSICA_H

#include <QObject>
#include <QMediaPlayer>
#include <QAudioOutput>

// Única clase que sabe de audio. El resto del programa solo llama
// reproducir() / pausar() / alternar() y escucha estadoCambio().
class GestorMusica : public QObject
{
    Q_OBJECT

public:
    explicit GestorMusica(QObject *parent = nullptr);

    bool estaSonando() const;
    void reproducir();
    void pausar();

public slots:
    void alternar();

signals:
    void estadoCambio(bool sonando);

private:
    QMediaPlayer *reproductor;
    QAudioOutput *salida;
    bool sonando;
};

#endif // GESTORMUSICA_H
