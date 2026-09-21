QT += widgets multimedia

CONFIG += c++17

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    bloquemovil.cpp \
    comida.cpp \
    configuracionnivel.cpp \
    gestorarchivos.cpp \
    gestormusica.cpp \
    jugador.cpp \
    main.cpp \
    mainwindow.cpp \
    nodo.cpp \
    serpiente.cpp \
    tablero.cpp \
    usuario.cpp

HEADERS += \
    bloquemovil.h \
    comida.h \
    configuracionnivel.h \
    gestorarchivos.h \
    gestormusica.h \
    jugador.h \
    mainwindow.h \
    nodo.h \
    serpiente.h \
    tablero.h \
    usuario.h
# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

RESOURCES += \
    Recursos.qrc

DISTFILES +=
