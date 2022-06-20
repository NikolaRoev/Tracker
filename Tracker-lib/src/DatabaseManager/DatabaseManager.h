#pragma once
#include "pch.h"
#include "Work.h"
#include "Creator.h"
#include "AttachedCreator.h"

//==================================================================================================================================
//
//	Uses SQLite and consists of 3 tables.
//	Functions return `false` if query fails to execute.
//
//==================================================================================================================================
//
//	Works:
//	+------+----------+------------+----------+-------------+-------------+-----------+
//	|  id  |   name   |   status   |   type   |   chapter   |   updated   |   added   |
//	+------+----------+------------+----------+-------------+-------------+-----------+
//	|   1  |  Bleach  |  Reading   |  Series  |     120     |  DATETIME*  | DATETIME* |
//	+------+----------+------------+----------+-------------+-------------+-----------+
//	*Check Utility/Utility.h - QString util::current_datetime() for the specific DATETIME format.
//
//	id: INTEGER PRIMARY KEY NOT NULL
//	name: TEXT NOT NULL
//	status: TEXT CHECK (status IN ('Reading', 'Completed')) NOT NULL
//	type: TEXT CHECK (type IN ('Series', 'One Shot', 'Anthology')) NOT NULL
//	chapter: TEXT
//	updated: TEXT
//	added: TEXT
//
//==================================================================================================================================
//
//	Creators:
//	+------+------------+
//	|  id  |    name    |
//	+------+------------+
//	|   1  |  Tite Kubo |
//	+------+------------+
//
//	id: INTEGER PRIMARY KEY NOT NULL
//	name: TEXT NOT NULL
//
//==================================================================================================================================
//
//	Work-Creator:
//	+-----------+--------------+----------+
//	|  work_id  |  creator_id  |   type   |
//	+--------------------------+----------+
//	|      1    |       1      |  Author  |
//	+-----------+--------------+----------+
//	|      1    |       1      |  Artist  |
//	+-----------+--------------+----------+
//
//	work_id: INTEGER NOT NULL
//	creator_id: INTEGER NOT NULL
//	type: TEXT CHECK (type IN ('Author', 'Artist')) NOT NULL
//	PRIMARY KEY (work_id, creator_id, type)
//	FOREIGN KEY (work_id) REFERENCES works (id) ON DELETE CASCADE
//	FOREIGN KEY (creator_id) REFERENCES creators (id) ON DELETE CASCADE
//
//==================================================================================================================================

namespace DatabaseManager {
	bool init(const QString& name);
	void deinit();

	bool add_work(const Work& work);
	bool remove_work(const int id);
	bool update_work(const QString& column, const int id, const QString& value);
	//Also returns `false` if `id` was not found when quering the database.
	bool get_work(Work& work, const int id);
	bool get_work_creators(QList<AttachedCreator>& creators, const int id);
	bool search_works(QList<Work>& works, const QString& search, const QString& by, const QString& status, const QString& type);

	bool add_creator(const Creator& creator);
	bool remove_creator(const int id);
	bool update_creator(const QString& column, const int id, const QString& value);
	//Also returns `false` if `id` was not found when quering the database.
	bool get_creator(Creator& creator, const int id);
	bool get_creator_works(QList<Work>& works, const int id);
	bool search_creators(QList<Creator>& creators, const QString& search);
	//Also returns `false` if the Work and Creator are already attached as `type`.
	bool attach_creator(const int work_id, const int creator_id, const QString& type);
	bool detach_creator(const int work_id, const int creator_id, const QString& type);
};
