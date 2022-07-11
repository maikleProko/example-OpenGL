QT       += core gui opengl
LIBS += -lopengl32 -lglu32
greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = KG5Lab
TEMPLATE = app


SOURCES += main.cpp\
    Circle.cpp \
        Dialog.cpp \
    Ring.cpp \
    Scene.cpp \
    Surface.cpp

HEADERS  += Dialog.h \
    Circle.h \
    Ring.h \
    Scene.h \
    Surface.h

FORMS    += Dialog.ui

RESOURCES += \
    Shaders.qrc
