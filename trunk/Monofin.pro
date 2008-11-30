TEMPLATE = app
TARGET = Monofin
QT += core \
    gui \
	xml \
    svg
HEADERS += Ui/mainWindow.h \
    Ui/physicalParameters.h \
    Ui/startupDialog.h \
    Ui/steps.h
SOURCES += main.cpp \
    Ui/mainWindow.cpp \
    Ui/physicalParameters.cpp \
    Ui/startupDialog.cpp \
    Ui/steps.cpp
FORMS += Ui/mainWindow.ui \
    Ui/physicalParameters.ui \
    Ui/startupDialog.ui \
    Ui/steps.ui
RESOURCES += resources/pixmaps.qrc \
    resources/icons.qrc
