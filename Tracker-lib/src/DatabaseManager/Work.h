#pragma once
#include "pch.h"

//==================================================================================================================================

struct Creator;

//==================================================================================================================================

struct Work {
	int id{};
	QString name;
	QString status;
	QString type;
	QString chapter;
	QString updated;
	QString added;
	QVector<Creator> creators;
};

//==================================================================================================================================
