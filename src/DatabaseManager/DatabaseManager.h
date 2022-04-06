#pragma once
#include "Work.h"

#include <QString>
#include <QVector>

//==================================================================================================================================

class DatabaseManager{
public:
	static void init();
	static void deinit();

	static void add_work(const QString name, const QString status, const QString type,
						 const QString grouping, const QString chapter);

	static void add_creator(const QString name, const QString work);

	static void remove_work(const QString name);

	static QVector<Work> get_works(const QString name);
	static QVector<QString> get_creators(const QString work);
};

//==================================================================================================================================
