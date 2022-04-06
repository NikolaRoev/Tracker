#pragma once
#include <QString>
#include <QVector>

//==================================================================================================================================

struct Work {
	QString name;
	QString status;
	QString type;
	QString grouping;
	QString chapter;
	QString updated;
	QVector<QString> creators;
};

//==================================================================================================================================
