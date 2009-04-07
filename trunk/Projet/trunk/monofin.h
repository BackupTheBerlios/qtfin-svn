#ifndef MONOFIN_H
#define MONOFIN_H

#include <QVBoxLayout>
#include <QWidget>

class MonofinFile;
class PaintingScene;

class Monofin : public QWidget
{
    Q_OBJECT
    Q_PROPERTY(QString currentFile READ currentFile)
public:
    Monofin(QWidget *parent = 0);

    QString currentFile() const { return _curFile; }
    void newFile();
    bool okToContinue();
    bool open();
    bool openFile(const QString &fileName);
    bool save();
    bool saveAs();
    QSize sizeHint() const;

public slots:
    // for PaintingScene
    void activateAddControl(bool a);
    void activateAddPoint(bool a);
    void activateCreateLine(bool a);
    void activateRemoveControlPoint(bool a);
    void alignTangents();
    void cleanPoints();
    void removeSelectedPoints();
    void showCoords();

signals:
    void currentFileChanged();
    void lineFinished(bool);

    protected:
    void closeEvent(QCloseEvent *closeEvent);

private slots:
    void monofinWasModified();

private:
    bool readFile(const QString &fileName);
    bool saveFile(const QString &fileName);
    void setConnections();
    void setCurrentFile(const QString &fileName);
    bool writeFile(const QString &fileName);
    QString strippedName(const QString &fullFileName);

private:
    QString _curFile;
    bool _isUntitled;
    QString _fileFilters;
    MonofinFile *_monofinFile;
    QVBoxLayout *_layout;
    PaintingScene *_scene;
};

#endif // MONOFIN_H
