#pragma once
#include "Work.h"
#include "Creator.h"

#include <QString>
#include <QVector>

//==================================================================================================================================

namespace DatabaseManager {
	void init();
	void deinit();
	void open(const QString& name);
	void close();
	const QString get_name();

	//Work.
	void add_work(const Work& work);
	void remove_work(const int id);
	void update_work(const QString& column, const int id, const QString& value);
	Work get_work(const int id);
	QVector<Work> search_works(const QString& search, const QString& by, const QString& status, const QString& type);

	//Creator.
	void add_creator(const QString& name);
	void remove_creator(const int id);
	void update_creator(const QString& column, const int id, const QString& value);
	Creator get_creator(const int id);
	QVector<Creator> search_creators(const QString& search);
	void attach_creator(const int work_id, const int creator_id, const QString& type);
	void detach_creator(const int work_id, const int creator_id);
}

//==================================================================================================================================
