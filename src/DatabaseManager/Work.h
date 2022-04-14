#pragma once
#include "Creator.h"

#include <QString>
#include <QVector>

//==================================================================================================================================

struct Work {
	int id{};
	QString name;
	QString status;
	QString type;
	QString grouping;
	QString chapter;
	QString updated;
	QString added;
	QVector<Creator> creators;
};

//==================================================================================================================================
