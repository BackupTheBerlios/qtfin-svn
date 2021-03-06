# -------------------------------------------------
# Project created by QtCreator 2009-03-09T21:00:28
# -------------------------------------------------
QT += xml
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
    Data/xmlloadvisitor.cpp \
    Data/xmlsavevisitor.cpp \
    Drawing/boundingpoint.cpp \
    Drawing/brline.cpp \
    Drawing/controlpoint.cpp \
    Drawing/extremitypoint.cpp \
    Drawing/ghostline.cpp \
    Drawing/ghostpoint.cpp \
    Drawing/layeritem.cpp \
    Drawing/layerrowitem.cpp \
    Drawing/layerview.cpp \
    Drawing/paintingscene.cpp \
    Drawing/paintingview.cpp \
    Drawing/selectionrect.cpp \
    Drawing/symmetryaxis.cpp \
    Drawing/tangent.cpp \
    EdgeDetection/abstractalgoedgesextraction.cpp \
    EdgeDetection/algosnake.cpp \
    EdgeDetection/drawpreview.cpp \
    EdgeDetection/drawpreviewview.cpp \
    EdgeDetection/edgesextractionscene.cpp \
    EdgeDetection/edgesextractionview.cpp \
    EdgeDetection/pixmapitem.cpp \
    EdgeDetection/rotatecircle.cpp \
    EdgeDetection/scircle.cpp \
    EdgeDetection/spoint.cpp \
    Scripting/comsolscript.cpp \
    Scripting/defaultscript.cpp \
    Scripting/scripthelper.cpp \
    Scripting/scriptmanager.cpp \
    Scripting/viewerscript.cpp \
    Ui/layerparameters.cpp \
    Ui/mainwindow.cpp \
    Ui/parametersdialog.cpp \
    Ui/startupdialog.cpp \
    Ui/geometry3dviewer.cpp \
    Ui/generatecomsolfiledialog.cpp \
    Ui/insertlayerdialog.cpp \
    Ui/layerindexdialog.cpp \
    Ui/formitem.cpp \
    Ui/graphic.cpp
HEADERS += monofin.h \
    lib/qtwindowlistmenu.h \
    Data/DataConstants.h \
    Data/configfile.h \
    Data/historyholder.h \
    Data/historyholder.imp \
    Data/layerconfig.h \
    Data/layer.h \
    Data/LoadVisitor.h \
    Data/monofinfile.h \
    Data/profil.h \
    Data/projectfile.h \
    Data/SaveVisitor.h \
    Data/segment.h \
    Data/surface.h \
    Data/LoadVisitor.h \
    Data/SaveVisitor.h \
    Data/xmlloadvisitor.h \
    Data/xmlsavevisitor.h \
    Drawing/boundingpoint.h \
    Drawing/brline.h \
    Drawing/controlpoint.h \
    Drawing/extremitypoint.h \
    Drawing/ghostline.h \
    Drawing/ghostpoint.h \
    Drawing/layeritem.h \
    Drawing/layerrowitem.h \
    Drawing/layerview.h \
    Drawing/paintingscene.h \
    Drawing/paintingview.h \
    Drawing/selectionrect.h \
    Drawing/symmetryaxis.h \
    Drawing/tangent.h \
    EdgeDetection/abstractalgoedgesextraction.h \
    EdgeDetection/algosnake.h \
    EdgeDetection/drawpreview.h \
    EdgeDetection/drawpreviewview.h \
    EdgeDetection/edgesextractionscene.h \
    EdgeDetection/edgesextractionview.h \
    EdgeDetection/pixmapitem.h \
    EdgeDetection/rotatecircle.h \
    EdgeDetection/scircle.h \
    EdgeDetection/spoint.h \
    Scripting/comsolscript.h \
    Scripting/defaultscript.h \
    Scripting/scripthelper.h \
    Scripting/scriptmanager.h \
    Scripting/viewerscript.h \
    Ui/layerparameters.h \
    Ui/mainwindow.h \
    Ui/parametersdialog.h \
    Ui/startupdialog.h \
    Ui/geometry3dviewer.h \
    Ui/generatecomsolfiledialog.h \
    Ui/insertlayerdialog.h \
    Ui/layerindexdialog.h \
    Ui/formitem.h \
    Ui/graphic.h
FORMS += Ui/layerParameters.ui \
    Ui/parametersDialog.ui \
    Ui/startupDialog.ui \
    Ui/insertlayerdialog.ui \
    Ui/layerindexdialog.ui \
    Ui/generatecomsolfiledialog.ui \
    Ui/graphic.ui \
    Ui/parametersModifier.ui
TRANSLATIONS = translations/monofin_de.ts \
    translations/monofin_en.ts \
    translations/monofin_fr.ts
RESOURCES += resources/images.qrc \
    resources/icons.qrc
mac {
RESOURCES += \
    translations.qrc \
    library.qrc
}
target.path = .
INSTALLS += target
RC_FILE = resources/monofin.rc

# compatibility issues
CONFIG += x86
QMAKE_CXXFLAGS += -m32
