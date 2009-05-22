#ifndef SCRIPT_H
#define SCRIPT_H

#include "../Data/projectfile.h"

#include <QTextStream>
#include <QString>

namespace Scripting {

  class ComsolScript {
  public:
	ComsolScript(QString name);
	virtual ~ComsolScript();

	QString getName() const { return name; }

	virtual void write(QTextStream& script, Data::ProjectFile& data) const = 0;
  private:
	QString name;
  };

}

#endif
