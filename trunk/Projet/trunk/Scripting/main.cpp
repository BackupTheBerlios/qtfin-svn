#include <QApplication>
#include <QFont>
#include <QPushButton>
#include <QWidget>

#include <QProcess>
#include <QStringList>

#include "../Data/projectfile.h"
#include "defaultscriptmanager.h"
#include "viewerscriptmanager.h"

using namespace Scripting;

class ScriptingUI : public QWidget {
public:
	ScriptingUI(QWidget *parent);
};

ScriptingUI::ScriptingUI(QWidget *parent)
	: QWidget(parent) {
	setFixedSize(200, 150);

	QPushButton *start = new QPushButton(tr("Quit"), this);
	start->setGeometry(0, 0, 50, 25);

	//QObject::connect(start, SIGNAL(clicked()), sm, SLOT(execute()));
}

int main(int argc, char *argv[]) {
	Data::ProjectFile *data = new Data::ProjectFile();
	//DefaultScriptManager* sm = new DefaultScriptManager(false, 9, "c:\\toto", *data);
	ViewerScriptManager* sm = new ViewerScriptManager("c:\\toto", *data);

	QApplication app(argc, argv);
	//ScriptingUI widget(0, sm);
	//widget.show();
	if (sm->write()) {
		sm->execute();
	}

	return app.exec();
}
