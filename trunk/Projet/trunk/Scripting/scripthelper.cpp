#include "scripthelper.h"

#include "scriptmanager.h"

#include <QDir>
#include <QString>
#include <QList>

using namespace Scripting;

const /*static*/ float ScriptHelper::DEFAULT_MONOFIN_LENGTH = 25; // cm
const /*static*/ float ScriptHelper::DEFAULT_LAYER_THICKNESS = 1; // cm
const /*static*/ float ScriptHelper::DEFAULT_LAYER_E = 200000;	// MPa
const /*static*/ float ScriptHelper::DEFAULT_LAYER_NU = 0.33;		// ratio
const /*static*/  float ScriptHelper::DEFAULT_LAYER_RHO = 7850;	// Kg/m^3

void ScriptHelper::writePath(QTextStream& script, Data::ProjectFile& data)
{
	script
			<< QString("path('%1', path);").arg(ScriptManager::getScriptDirectory())
			<< endl;
}

void ScriptHelper::writeMonofinDeclaration(QTextStream& script, Data::ProjectFile& data)
{
	script
			<< "clear monofin;"
			<< endl;
}

void ScriptHelper::writeMonofinLength(QTextStream& script, Data::ProjectFile& data)
{
	float length = data.getHowManyLayers() > 0
					? data.getLayerMaxLength()
					: DEFAULT_MONOFIN_LENGTH;

	script
			<< QString("monofin.length = %1 * 1e-2;").arg(length) // cm -> m
			<< endl;
}

void ScriptHelper::writeMonofinSegments(QTextStream& script, Data::ProjectFile& data)
{
	script << "monofin.segments = [ ";

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

		script << QString("struct('x', [%1 %2 %3], 'y', [%4 %5 %6])")
				  .arg(aX)
				  .arg(bX)
				  .arg(cX)
				  .arg(aY)
				  .arg(bY)
				  .arg(cY);

		if (i < data.getAllSegmentKeys().count() - 1) {
			script << ", ";
		}
	}

	script
			<< " ];"
			<< endl;

	// Création de méta données
	QList<int> iKeys = data.getExtremityPoint();

	if (iKeys.length() != 2) {
		script
				<< "error('Cannot generate the COMSOL script whith a corrupted Monofin.');"
				<< endl;
	} else {
		float aX, aY, bX, bY;
		data.getIntersectionPoint(iKeys.at(0), aX, aY);
		data.getIntersectionPoint(iKeys.at(1), bX, bY);

		float absoluteLength = qMax(aX, bX) - qMin(aX, bX);
		script
				<< QString("monofin.metaSegments = struct('absoluteLength', %1, 'dX', %2, 'dY', %3);")
				   .arg(absoluteLength)
				   .arg(qMin(aX, bX) == aX ? aX : bX)
				   .arg(qMin(aX, bX) == aX ? aY : bY)
				<< endl;
	}
}

void ScriptHelper::writeMonofinLayers(QTextStream& script, Data::ProjectFile& data)
{
	script << "monofin.layers = [ ";

	// if no layer, we add a default one
	if (data.getHowManyLayers() == 0) {
		script << QString("struct('thickness', %1 * 1e-2, 'length', 1, 'E', %2 * 1e6, 'nu', %3, 'rho', %4)")
				  .arg(DEFAULT_LAYER_THICKNESS) // cm -> m
				  .arg(DEFAULT_LAYER_E)
				  .arg(DEFAULT_LAYER_NU)
				  .arg(DEFAULT_LAYER_RHO);
	} else {

		float monofinLength = data.getLayerMaxLength();

		for (int i = 0; i < data.getHowManyLayers(); i++) {
			script << QString("struct('thickness', %1 * 1e-2, 'length', %2, 'E', %3 * 1e6, 'nu', %4, 'rho', %5)")
					  .arg(data.getLayerHeight(i))
					  .arg(data.getLayerLength(i) / monofinLength) // the script needs a ratio
					  .arg(data.getLayerConfigYoung(i))
					  .arg(data.getLayerConfigPoisson(i))
					  .arg(data.getLayerConfigRho(i));

			if (i < data.getHowManyLayers() - 1) {
				script << ", ";
			}
		}
	}

	script
			<< " ];"
			<< endl;
}

void ScriptHelper::writeEOS(QTextStream& script) {
	script
			<< endl
			<< "@@@"
			<< endl;

	// ensure everything is written to the stream
	script.flush();
}
