#include "DatabaseManager.h"
#include "UpdateWork.h"
#include "FilterWork.h"
#include "Work.h"

#include <QDebug>
#include <QString>
#include <QVector>
#include <QSqlDatabase>
#include <QSqlError>
#include <QSqlQuery>

//==================================================================================================================================
//==================================================================================================================================
//==================================================================================================================================

void DatabaseManager::init() {
	QSqlDatabase::addDatabase("QSQLITE");
}

//==================================================================================================================================

void DatabaseManager::deinit() {
	QSqlDatabase::removeDatabase("QSQLITE");
}

//==================================================================================================================================

void DatabaseManager::open(const QString& name) {
	QSqlDatabase db = QSqlDatabase::database();
	db.setDatabaseName(name);
	if (!db.open()) {
		qDebug() << db.lastError();
	}
	QSqlQuery query;
	query.exec("PRAGMA foreign_keys = ON");

	//Works table.
	query.prepare(
		"CREATE TABLE IF NOT EXISTS works ("
		"	id			INTEGER PRIMARY KEY NOT NULL, "
		"	name		TEXT NOT NULL, "
		"   status		TEXT CHECK (status IN ('Reading', 'Completed')) NOT NULL, "
		"   type		TEXT CHECK (type IN ('Series', 'One Shot', 'Anthology')) NOT NULL, "
		"   grouping	TEXT, "
		"   chapter		TEXT, "
		"   updated		TEXT, "
		"	added		TEXT"
		")"
	);
	if (!query.exec()) {
		qDebug() << query.lastError();
	}

	//Creators table.
	query.prepare(
		"CREATE TABLE IF NOT EXISTS creators ("
		"	id		INTEGER PRIMARY KEY NOT NULL, "
		"	name	TEXT NOT NULL"
		")"
	);
	if (!query.exec()) {
		qDebug() << query.lastError();
	}

	//Work-creator table.
	query.prepare(
		"CREATE TABLE IF NOT EXISTS work_creator ("
		"	work_id		INTEGER NOT NULL, "
		"	creator_id	INTEGER NOT NULL, "
		"	type		TEXT CHECK (type IN ('Author', 'Artist')) NOT NULL, "
		"	PRIMARY KEY (work_id, creator_id, type), "
		"	FOREIGN KEY (work_id) REFERENCES works (id) ON DELETE CASCADE, "
		"	FOREIGN KEY (creator_id) REFERENCES creators (id) ON DELETE CASCADE"
		")"
	);
	if (!query.exec()) {
		qDebug() << query.lastError();
	}
}

//==================================================================================================================================

void DatabaseManager::close() {
	QSqlDatabase db = QSqlDatabase::database();
	db.close();
}

//==================================================================================================================================

const QString DatabaseManager::get() {
	QSqlDatabase db = QSqlDatabase::database();
	return db.databaseName();
}

//==================================================================================================================================
//==================================================================================================================================

void DatabaseManager::add_work(const QString& name, const QString& status, const QString& type, const QString& grouping) {
	QSqlQuery query;

	//Insert the work.
	query.prepare(
		"INSERT INTO works (name, status, type, grouping, chapter, updated, added) "
		"VALUES (:name, :status, :type, :grouping, NULL, NULL, datetime('now'))"
	);

	query.bindValue(":name", name);
	query.bindValue(":status", status);
	query.bindValue(":type", type);
	query.bindValue(":grouping", grouping);

	if (!query.exec()) {
		qDebug() << query.lastError();
	}
}

//==================================================================================================================================

void DatabaseManager::remove_work(const int id) {
	QSqlQuery query;
	query.prepare(
		"DELETE FROM works "
		"WHERE id = (:id)"
	);
	query.bindValue(":id", id);

	if (!query.exec()) {
		qDebug() << query.lastError();
	}
}

//==================================================================================================================================
//==================================================================================================================================

void DatabaseManager::update_work_name(const int id, const QString& new_name) {
	QSqlQuery query;
	query.prepare(
		"UPDATE works "
		"SET name = (:new_name) "
		"WHERE id = (:id)"
	);
	query.bindValue(":new_name", new_name);
	query.bindValue(":id", id);

	if (!query.exec()) {
		qDebug() << query.lastError();
	}
}

//==================================================================================================================================

void DatabaseManager::update_work_status(const int id, const QString& new_status) {
	QSqlQuery query;
	query.prepare(
		"UPDATE works "
		"SET status = (:new_status) "
		"WHERE id = (:id)"
	);
	query.bindValue(":new_status", new_status);
	query.bindValue(":id", id);

	if (!query.exec()) {
		qDebug() << query.lastError();
	}
}

//==================================================================================================================================

void DatabaseManager::update_work_type(const int id, const QString& new_type) {
	QSqlQuery query;
	query.prepare(
		"UPDATE works "
		"SET type = (:new_type) "
		"WHERE id = (:id)"
	);
	query.bindValue(":new_type", new_type);
	query.bindValue(":id", id);

	if (!query.exec()) {
		qDebug() << query.lastError();
	}
}

//==================================================================================================================================

void DatabaseManager::update_work_grouping(const int id, const QString& new_grouping) {
	QSqlQuery query;
	query.prepare(
		"UPDATE works "
		"SET grouping = (:new_grouping) "
		"WHERE id = (:id)"
	);
	query.bindValue(":new_grouping", new_grouping);
	query.bindValue(":id", id);

	if (!query.exec()) {
		qDebug() << query.lastError();
	}
}

//==================================================================================================================================

void DatabaseManager::update_work_chapter(const int id, const QString& new_chapter) {
	QSqlQuery query;
	query.prepare(
		"UPDATE works "
		"SET chapter = (:new_chapter), updated = datetime('now') "
		"WHERE id = (:id)"
	);
	query.bindValue(":new_chapter", new_chapter);
	query.bindValue(":id", id);

	if (!query.exec()) {
		qDebug() << query.lastError();
	}
}

//==================================================================================================================================
//==================================================================================================================================

QVector<UpdateWork> DatabaseManager::search_update_works(const QString& maybe_partial_name) {
	QSqlQuery query;
	query.prepare(
		"SELECT id, name, chapter "
		"FROM works "
		"WHERE name LIKE (:name) AND status = 'Reading'"
	);
	query.bindValue(":name", '%' + maybe_partial_name + '%');


	QVector<UpdateWork> out;

	if (query.exec()) {
		while (query.next()) {
			out.emplace_back(query.value(0).toInt(), query.value(1).toString(), query.value(2).toString());
		}
	}
	else {
		qDebug() << query.lastError();
	}

	return out;
}

//==================================================================================================================================

QVector<FilterWork> DatabaseManager::search_works_by_name(const QString& maybe_partial_name, const QString& status, const QString& type) {
	//Query text construction.
	QString query_text =
			"SELECT id, name "
			"FROM works "
			"WHERE name LIKE (:name)";

	if (!status.isEmpty()) {
		query_text.append(" AND status = (:status)");
	}

	if (!type.isEmpty()) {
		query_text.append(" AND type = (:type)");
	}


	//Query preparation.
	QSqlQuery query;
	query.prepare(query_text);
	query.bindValue(":name", '%' + maybe_partial_name + '%');

	if (!status.isNull()) {
		query.bindValue(":status", status);
	}

	if (!type.isNull()) {
		query.bindValue(":type", type);
	}


	//Query execution.
	QVector<FilterWork> out;

	if (query.exec()) {
		while (query.next()) {
			out.emplace_back(query.value(0).toInt(), query.value(1).toString());
		}
	}
	else {
		qDebug() << query.lastError();
	}

	return out;
}

//==================================================================================================================================

Work DatabaseManager::search_work(const int id) {
	QSqlQuery query;
	query.prepare(
		"SELECT * "
		"FROM works "
		"WHERE id = (:id)"
	);
	query.bindValue(":id", id);


	Work out;

	if (query.exec()) {
		if (query.next()) {
			out.id = query.value(0).toInt();
			out.name = query.value(1).toString();
			out.status = query.value(2).toString();
			out.type = query.value(3).toString();
			out.grouping = query.value(4).toString();
			out.chapter = query.value(5).toString();
			out.updated = query.value(6).toString();
			out.added = query.value(7).toString();

			QSqlQuery creator_query;
			creator_query.prepare(
				"WITH current_creators AS ("
				"	SELECT creator_id AS id, type "
				"	FROM work_creator "
				"	WHERE work_id = (:work_id)"
				") "
				"SELECT current_creators.id, creators.name, current_creators.type "
				"FROM creators "
				"INNER JOIN current_creators "
				"ON creators.id = current_creators.id"
			);
			creator_query.bindValue(":work_id", out.id);

			if (creator_query.exec()) {
				while (creator_query.next()) {
					out.creators.emplace_back(creator_query.value(0).toInt(),
											   creator_query.value(1).toString(),
											   creator_query.value(2).toString());
				}
			}
			else {
				qDebug() << creator_query.lastError();
			}
		}
	}
	else {
		qDebug() << query.lastError();
	}

	return out;
}

//==================================================================================================================================
//==================================================================================================================================

void DatabaseManager::add_creator(const QString& name) {

}

//==================================================================================================================================

void DatabaseManager::remove_creator(const int id) {

}

//==================================================================================================================================
//==================================================================================================================================
//==================================================================================================================================
