#pragma once
#include "Creator.h"
#include "UpdateWork.h"

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

	static void add_work(const QString& name, const QString& status, const QString& type, const QString& grouping);
	static void remove_work(const QString& name);

	static void update_work_name(const int id, const QString& new_name);
	static void update_work_chapter(const int id, const QString& new_chapter);

	static QVector<UpdateWork> search_update_works(const QString& maybe_partial_name);
	static QVector<QString> search_authors(const QString& maybe_partial_name);
};

//==================================================================================================================================
