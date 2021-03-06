QT       += core gui widgets

TARGET = TP3
TEMPLATE = app

SOURCES += main.cpp

SOURCES += \
    mainwidget.cpp \
    geometryengine.cpp \
    calendar.cpp \
    quadtree.cpp

HEADERS += \
    mainwidget.h \
    geometryengine.h \
    calendar.h \
    quadtree.h

RESOURCES += \
    shaders.qrc \
    textures.qrc

# install
target.path = $$[QT_INSTALL_EXAMPLES]/opengl/cube
INSTALLS += target
