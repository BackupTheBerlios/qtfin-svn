#ifndef GEMEOTRY3DVIEWER_H
#define GEMEOTRY3DVIEWER_H

#include "../Data/projectfile.h"
#include "../Scripting/scriptmanager.h"
#include "../Scripting/viewerscript.h"

#include <QFrame>
#include <QLabel>

class Geometry3DViewer : public QFrame
{
	Q_OBJECT
public:
	Geometry3DViewer(Data::ProjectFile& data, QWidget *parent = 0);
	virtual void closeEvent(QCloseEvent *event);

public slots:
	virtual void show();

private:
	QLabel* lState;
	Scripting::ScriptManager* smanager;
	Scripting::ViewerScript script;

private slots:
	void on_smanager_ended(bool successed);

};

#endif
