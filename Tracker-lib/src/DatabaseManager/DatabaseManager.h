#pragma once
#include "pch.h"
#include "Work.h"
#include "Creator.h"
#include "AttachedCreator.h"

//==================================================================================================================================

namespace DatabaseManager {
	bool init(const QString& name);
	void deinit();

	//Work.
	bool add_work(const Work& work);
	bool remove_work(const int id);
	bool update_work(const QString& column, const int id, const QString& value);
	bool get_work(Work& work, const int id);
	bool get_work_creators(QList<AttachedCreator>& creators, const int id);
	bool search_works(QList<Work>& works, const QString& search, const QString& by, const QString& status, const QString& type);

	//Creator.
	bool add_creator(const Creator& creator);
	bool remove_creator(const int id);
	bool update_creator(const QString& column, const int id, const QString& value);
	bool get_creator(Creator& creator, const int id);
	bool get_creator_works(QList<Work>& works, const int id);
	bool search_creators(QList<Creator>& creators, const QString& search);
	bool attach_creator(const int work_id, const int creator_id, const QString& type);
	bool detach_creator(const int work_id, const int creator_id, const QString& type);
};

//==================================================================================================================================
