#include "monofin.h"

#include "Drawing/paintingscene.h"
#include "Drawing/paintingview.h"

#include <QtCore/QFileInfo>
#include <QtGui/QFileDialog>
#include <QtGui/QMessageBox>

Monofin::Monofin(QWidget *parent)
    : QWidget(parent)
{
    setAttribute(Qt::WA_DeleteOnClose);
    _scene = new PaintingScene(1024, 768, this);
    
    _layout = new QVBoxLayout;
    _layout->addWidget(new PaintingView(_scene));
    setLayout(_layout);

    setConnections();

    _curFile = QString();
}

// PUBLIC

void Monofin::newFile()
{
    static int documentNumber = 1;
    setCurrentFile(QString());
    ++documentNumber;
}

bool Monofin::open()
{
    if (okToContinue()) {
        QString fileName = QFileDialog::getOpenFileName(this,
                                                        tr("Open Monofin"), ".",
                                                        _fileFilters);
        if (!fileName.isEmpty())
            return openFile(fileName);
    }
    return false;
}

bool Monofin::openFile(const QString &fileName)
{
    if (QFile::exists(fileName))
        return readFile(fileName);
    else
        return false;
}

bool Monofin::save()
{
    if (_isUntitled)
        return saveAs();
    else
        return saveFile(_curFile);
}

bool Monofin::saveAs()
{
    QString fileName = QFileDialog::getSaveFileName(this,
                                                    tr("Save Monofin"), ".",
                                                    tr("Monofin files (*.fin)"));
    if(fileName.isEmpty())
        return false;
    else
        return saveFile(fileName);
}

QSize Monofin::sizeHint() const
{
    return _layout->sizeHint();
}

// PUBLIC SLOTS

void Monofin::activateAddControl(bool a)
{
    _scene->activateAddControl(a);
}

void Monofin::activateAddPoint(bool a)
{
    _scene->activateAddPoint(a);
}

void Monofin::activateCreateLine(bool a)
{
    _scene->activateCreateLine(a);
}

void Monofin::activateRemoveControlPoint(bool a)
{
    _scene->activateRemoveControlPoint(a);
}

void Monofin::alignTangents()
{
    _scene->alignTangents();
}

void Monofin::cleanPoints()
{
    _scene->cleanPoints();
}

void Monofin::removeSelectedPoints()
{
    _scene->removeSelectedPoints();
}

void Monofin::showCoords()
{
    _scene->showCoords();
}

// PROTECTED

void Monofin::closeEvent(QCloseEvent *closeEvent)
{
    if (okToContinue())
        closeEvent->accept();
    else
        closeEvent->ignore();
}

// PRIVATE SLOTS

void Monofin::monofinWasModified()
{
    setWindowModified(true);
}

// PRIVATE

bool Monofin::okToContinue()
{
    if (isWindowModified()) {
        int r = QMessageBox::warning(this, tr("Monofin"),
                                     tr("The document has been modified.\n"
                                        "Do you want to save your changes?"),
                                     QMessageBox::Yes | QMessageBox::Default,
                                     QMessageBox::No,
                                     QMessageBox::Cancel | QMessageBox::Escape);
        if (r == QMessageBox::Yes)
            return save();
        else if (r == QMessageBox::Cancel)
            return false;
    }
    return true;
}

bool Monofin::readFile(const QString &fileName)
{
    // TODO
    setCurrentFile(fileName);
    return true;
}

bool Monofin::saveFile(const QString &fileName)
{
    // TODO
    setCurrentFile(fileName);
    return true;
}

void Monofin::setConnections()
{
    connect(_scene, SIGNAL(lineFinished(bool)), this, SIGNAL(lineFinished(bool)));
}


void Monofin::setCurrentFile(const QString &fileName)
{
    _curFile = fileName;
    setWindowModified(false);

    QString shownName = "Untitled";
    _isUntitled = true;

    if (!_curFile.isEmpty()) {
        shownName = strippedName(_curFile);
        _isUntitled = false;
    }

    setWindowTitle(tr("%1[*] - %2").arg(shownName).arg(tr("Monofin")));
    emit currentFileChanged();
}

QString Monofin::strippedName(const QString &fullFileName)
{
    return QFileInfo(fullFileName).fileName();
}
