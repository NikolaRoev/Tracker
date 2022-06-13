#pragma once
#include "pch.h"
#include "Work.h"
#include "Creator.h"

//==================================================================================================================================

namespace DatabaseManager {
	QString init(const QString& database_name);
	void deinit();

	//Work.
	QString add_work(const Work& work);
	QString remove_work(const int id);
	QString update_work(const QString& column, const int id, const QString& value);
	QString get_work(Work& work, const int id);
	QString search_works(QList<Work>& works, const QString& search, const QString& by, const QString& status, const QString& type);

	//Creator.
	QString add_creator(const Creator& creator);
	QString remove_creator(const int id);
	QString update_creator(const QString& column, const int id, const QString& value);
	QString get_creator(Creator& creator, const int id);
	QString search_creators(QList<Creator>& creators, const QString& search);
	QString attach_creator(const int work_id, const int creator_id, const QString& type);
	QString detach_creator(const int work_id, const int creator_id);
};

//==================================================================================================================================
