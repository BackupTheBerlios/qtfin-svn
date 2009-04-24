# -------------------------------------------------
# Project created by QtCreator 2009-03-09T21:00:28
# -------------------------------------------------
TARGET = Monofin
TEMPLATE = app
CONFIG += debug_and_release
CONFIG(debug, debug|release):CONFIG += console
else:DEFINES += QT_NO_DEBUG_OUTPUT \
    QT_NO_WARNING_OUTPUT
SOURCES += main.cpp \
    monofin.cpp \
    lib/qtwindowlistmenu.cpp \
    Data/configfile.cpp \
    Data/layerconfig.cpp \
    Data/layer.cpp \
    Data/monofinfile.cpp \
    Data/profil.cpp \
    Data/projectfile.cpp \
    Data/segment.cpp \
    Data/surface.cpp \
    Drawing/boundingpoint.cpp \
    Drawing/brline.cpp \
    Drawing/controlpoint.cpp \
    Drawing/extremitypoint.cpp \
    Drawing/ghostline.cpp \
    Drawing/ghostpoint.cpp \
    Drawing/paintingscene.cpp \
    Drawing/paintingview.cpp \
    Drawing/selectionrect.cpp \
    Drawing/symmetryaxis.cpp \
    Drawing/tangent.cpp \
    Scripting/script.cpp \
    Scripting/Comsol/comsolscript.cpp \
    Scripting/Comsol/comsolscriptwriter.cpp \
    Scripting/Comsol/dummywriter.cpp \
    Ui/layerparameters.cpp \
    Ui/mainwindow.cpp \
    Ui/parametersdialog.cpp \
    Ui/startupdialog.cpp
HEADERS += monofin.h \
    lib/qtwindowlistmenu.h \
    Data/DataConstants.h \
    Data/configfile.h \
    Data/historyholder.h \
    Data/layerconfig.h \
    Data/layer.h \
    Data/monofinfile.h \
    Data/profil.h \
    Data/projectfile.h \
    Data/segment.h \
    Data/surface.h \
    Drawing/boundingpoint.h \
    Drawing/brline.h \
    Drawing/controlpoint.h \
    Drawing/extremitypoint.h \
    Drawing/ghostline.h \
    Drawing/ghostpoint.h \
    Drawing/paintingscene.h \
    Drawing/paintingview.h \
    Drawing/selectionrect.h \
    Drawing/symmetryaxis.h \
    Drawing/tangent.h \
    Scripting/script.h \
    Scripting/scriptwriter.h \
    Scripting/Comsol/comsolscript.h \
    Scripting/Comsol/comsolscriptwriter.h \
    Scripting/Comsol/dummywriter.h \
    Ui/layerparameters.h \
    Ui/mainwindow.h \
    Ui/parametersdialog.h \
    Ui/startupdialog.h
FORMS += Ui/layerParameters.ui \
    Ui/parametersDialog.ui \
    Ui/startupDialog.ui
TRANSLATIONS = translations/monofin_de.ts \
    translations/monofin_en.ts \
    translations/monofin_fr.ts
target.path = .
INSTALLS += target
RESOURCES += resources/images.qrc
