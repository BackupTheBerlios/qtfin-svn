#ifndef GEMEOTRY3DVIEWER_H
#define GEMEOTRY3DVIEWER_H

#include "../Data/projectfile.h"
#include "../Scripting/viewerscriptmanager.h"

#include <QFrame>
#include <QLabel>

class Geometry3DViewer : public QFrame
{
	Q_OBJECT
public:
	Geometry3DViewer(Data::ProjectFile& data, QWidget *parent = 0);
	~Geometry3DViewer();
	virtual void closeEvent(QCloseEvent *event);

public slots:
	void show();

private:
	QLabel* lState;
	Scripting::ViewerScriptManager* vsm;
	const static QString OUTPUT_PATH;

private slots:
	void scriptExecutionEnded(bool successed);

};

#endif
