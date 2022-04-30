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
	static void add_work(const QString& name, const QString& status, const QString& type, const QString& grouping, const QString& chapter);
	static void remove_work(const int id);
	static void update_work(const QString& column, const int id, const QString& value);
	static Work get_work(const int id);

	static QVector<Work> search_works_by_name(const QString& maybe_partial_name,
											  const QString& status = QString(),
											  const QString& type = QString());


	//Creator.
	static void add_creator(const QString& name);
	static void remove_creator(const int id);
	static void update_creator(const QString& column, const int id, const QString& value);
	static Creator get_creator(const int id);

	static QVector<Creator> search_creators(const QString& maybe_partial_name);

	static void attach_creator(const int work_id, const int creator_id, const QString& creator_type);
	static void detach_creator(const int work_id, const int creator_id, const QString& creator_type);
};

//==================================================================================================================================
