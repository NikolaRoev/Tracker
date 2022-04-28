#pragma once
#include "Work.h"
#include "Creator.h"

#include <QString>
#include <QVector>

//==================================================================================================================================

class DatabaseManager {
public:
	static void init();
	static void deinit();
	static void open(const QString& name);
	static void close();
	static const QString get();


	//Work.
	static void add_work(const QString& name, const QString& status, const QString& type, const QString& grouping);
	static void remove_work(const int id);
	static void update_work(const QString& column, const int id, const QString& value);

	//TO DO: simplify me?? Why have both of the same? just use Reding for update works???
	static QVector<Work> search_update_works(const QString& maybe_partial_name);
	static QVector<Work> search_works_by_name(const QString& maybe_partial_name, const QString& status, const QString& type);
	static Work search_work(const int id);


	//Creator.
	static void add_creator(const QString& name);
	static void remove_creator(const int id);
};

//==================================================================================================================================
