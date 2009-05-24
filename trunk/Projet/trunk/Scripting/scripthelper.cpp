#include "scripthelper.h"

#include "scriptmanager.h"

#include <QDir>
#include <QString>

using namespace Scripting;

void ScriptHelper::writePath(QTextStream& script, Data::ProjectFile& data)
{
	QString path = QString("path('%1', path);");
	path = path
		   .arg(QDir::toNativeSeparators(ScriptManager::ScriptDirectory));

	script << path << endl;
	script << endl;
}

void ScriptHelper::writeMonofinDeclaration(QTextStream& script, Data::ProjectFile& data)
{
	script << "clear monofin;" << endl;
	script << endl;
}

void ScriptHelper::writeMonofinLength(QTextStream& script, Data::ProjectFile& data)
{
	QString length = QString("monofin.length = %1;");
	length = length
                   .arg(data.getLayerMaxLength());

	script << length << endl;
	script << endl;
}

void ScriptHelper::writeMonofinSegments(QTextStream& script, Data::ProjectFile& data)
{
	script << "monofin.segments = [ ..." << endl;
	for (int i = 0; i < data.getAllSegmentKeys().count(); i++) {
		// keys
		int segKey = data.getAllSegmentKeys().at(i);
		int aKey, bKey, cKey;

		// coordinates
		float aX, aY, bX, bY, cX, cY;

		data.getSegment(segKey, aKey, cKey, bKey);
		data.getIntersectionPoint(aKey, aX, aY);
		data.getControlPoint(bKey, bX, bY);
		data.getIntersectionPoint(cKey, cX, cY);

		if (bKey == Data::MONOFIN_SURFACE_NO_CONTROL_POINT) {
			// there is no control point on the segment
			// so we need to compute a "virtual" control point in the middle of the segment
			bX = (aX + cX) / 2;
			bY = (aY + cY) / 2;
		}

		script <<
				QString("struct('x', [%1 %2 %3], 'y', [%4 %5 %6])")
					.arg(aX)
					.arg(bX)
					.arg(cX)
					.arg(aY)
					.arg(bY)
					.arg(cY);

		if (i < data.getAllSegmentKeys().count() - 1) {
			script << ", ..." << endl;
		}
	}

	script << " ];" << endl << endl;
}

void ScriptHelper::writeMonofinLayers(QTextStream& script, Data::ProjectFile& data)
{
	script << "monofin.layers = [ ..." << endl;

        float monofinLength = data.getLayerMaxLength();

	// if no layer, we add a default one
	if (data.getHowManyLayers() == 0) {
		script << QString("struct('thickness', %1, 'length', %2, 'E', %3, 'nu', %4, 'rho', %5)")
			.arg(0.001) // 1 cm
			.arg(1)
			.arg(2.0e11)
			.arg(0.33)
			.arg(7850);
	}

	for (int i = 0; i < data.getHowManyLayers(); i++) {
		script <<
				QString("struct('thickness', %1, 'length', %2, 'E', %3, 'nu', %4, 'rho', %5)")
					.arg(data.getLayerHeight(i))
					.arg(data.getLayerLength(i) / monofinLength) // the script needs a ratio
					.arg(data.getLayerConfigYoung(i))
					.arg(data.getLayerConfigPoisson(i))
					.arg(data.getLayerConfigRho(i));

		if (i < data.getHowManyLayers() - 1) {
			script << ", ..." << endl;
		}
	}

	script << " ];" << endl << endl;
}
