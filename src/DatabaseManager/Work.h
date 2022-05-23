#pragma once
#include <QString>
#include <QVector>

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
	QString md_id;
	QString mu_id;
	QVector<Creator> creators;
};

//==================================================================================================================================
