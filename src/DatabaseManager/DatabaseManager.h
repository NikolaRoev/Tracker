#pragma once
#include "UpdateWork.h"
#include "Work.h"

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
	static void remove_work(const int id);

	static void update_work_name(const int id, const QString& new_name);
	static void update_work_status(const int id, const QString& new_status);
	static void update_work_type(const int id, const QString& new_type);
	static void update_work_grouping(const int id, const QString& new_grouping);
	static void update_work_chapter(const int id, const QString& new_chapter);

	static QVector<UpdateWork> search_update_works(const QString& maybe_partial_name);
	static QVector<Work> search_works(const QString& maybe_partial_name, const QString& status);


	static void add_creator(const QString& name);
	static void remove_creator(const int id);
};

//==================================================================================================================================
