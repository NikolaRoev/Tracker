#include "DatabaseManager.h"
#include "Work.h"
#include "Creator.h"

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

void DatabaseManager::add_work(const QString& name, const QString& status, const QString& type, const QString& grouping, const QString& chapter) {
	QSqlQuery query;
	query.prepare(
		"INSERT INTO works (name, status, type, grouping, chapter, updated, added) "
		"VALUES (:name, :status, :type, :grouping, :chapter, NULL, datetime('now', 'localtime'))"
	);
	query.bindValue(":name", name);
	query.bindValue(":status", status);
	query.bindValue(":type", type);
	query.bindValue(":grouping", grouping);
	query.bindValue(":chapter", chapter);

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

void DatabaseManager::update_work(const QString& column, const int id, const QString& value) {
	//If we are updating the chapter, set a new update datetime.
	QString datetime_arg;
	if (column == "chapter") {
		datetime_arg = ", updated = datetime('now', 'localtime') ";
	}

	//Query construction.
	QString query_text = QString(
		"UPDATE works "
		"SET %1 = (:value) %2"
		"WHERE id = (:id)"
	).arg(column, datetime_arg);

	//Query execution.
	QSqlQuery query;
	query.prepare(query_text);
	query.bindValue(":value", value);
	query.bindValue(":id", id);

	if (!query.exec()) {
		qDebug() << query.lastError();
	}
}

//==================================================================================================================================

Work DatabaseManager::get_work(const int id) {
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

QVector<Work> DatabaseManager::search_works(const QString& maybe_partial_name, const QString& by, const QString& status, const QString& type) {
	//Query text construction.
	QString query_text =
			"SELECT id, name, grouping, chapter, updated "
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
	QVector<Work> out;

	if (query.exec()) {
		while (query.next()) {
			out.emplace_back(Work{ .id = query.value(0).toInt(),
								   .name = query.value(1).toString(),
								   .grouping = query.value(2).toString(),
								   .chapter = query.value(3).toString(),
								   .updated = query.value(4).toString()});
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
	QSqlQuery query;
	query.prepare(
		"INSERT INTO creators (name) "
		"VALUES (:name)"
	);
	query.bindValue(":name", name);

	if (!query.exec()) {
		qDebug() << query.lastError();
	}
}

//==================================================================================================================================

void DatabaseManager::remove_creator(const int id) {
	QSqlQuery query;
	query.prepare(
		"DELETE FROM creators "
		"WHERE id = (:id)"
	);
	query.bindValue(":id", id);

	if (!query.exec()) {
		qDebug() << query.lastError();
	}
}

//==================================================================================================================================

void DatabaseManager::update_creator(const QString& column, const int id, const QString& value) {
	QString query_text = QString(
		"UPDATE creators "
		"SET %1 = (:value) "
		"WHERE id = (:id)"
	).arg(column);

	QSqlQuery query;
	query.prepare(query_text);
	query.bindValue(":value", value);
	query.bindValue(":id", id);

	if (!query.exec()) {
		qDebug() << query.lastError();
	}
}

//==================================================================================================================================

Creator DatabaseManager::get_creator(const int id) {
	QSqlQuery query;
	query.prepare(
		"SELECT * "
		"FROM creators "
		"WHERE id = (:id)"
	);
	query.bindValue(":id", id);

	Creator out;

	if (query.exec()) {
		if (query.next()) {
			out.id = query.value(0).toInt();
			out.name = query.value(1).toString();

			QSqlQuery creator_query;
			creator_query.prepare(
				"WITH current_works AS ("
				"	SELECT DISTINCT work_id AS id "
				"	FROM work_creator "
				"	WHERE creator_id = (:creator_id)"
				") "
				"SELECT current_works.id, works.name "
				"FROM works "
				"INNER JOIN current_works "
				"ON works.id = current_works.id"
			);
			creator_query.bindValue(":creator_id", out.id);

			if (creator_query.exec()) {
				while (creator_query.next()) {
					out.works.emplace_back(creator_query.value(0).toInt(),
										   creator_query.value(1).toString());
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

QVector<Creator> DatabaseManager::search_creators(const QString& maybe_partial_name) {
	QSqlQuery query;
	query.prepare("SELECT id, name "
				  "FROM creators "
				  "WHERE name LIKE (:name)"
	);
	query.bindValue(":name", '%' + maybe_partial_name + '%');

	QVector<Creator> out;

	if (query.exec()) {
		while (query.next()) {
			out.emplace_back(Creator{ .id = query.value(0).toInt(), .name = query.value(1).toString()});
		}
	}
	else {
		qDebug() << query.lastError();
	}

	return out;
}

//==================================================================================================================================

void DatabaseManager::attach_creator(const int work_id, const int creator_id, const QString& type) {
	QSqlQuery query;
	query.prepare(
		"INSERT INTO work_creator (work_id, creator_id, type) "
		"VALUES (:work_id, :creator_id, :type)"
	);
	query.bindValue(":work_id", work_id);
	query.bindValue(":creator_id", creator_id);
	query.bindValue(":type", type);

	if (!query.exec()) {
		qDebug() << query.lastError();
	}
}

//==================================================================================================================================

void DatabaseManager::detach_creator(const int work_id, const int creator_id, const QString& type) {
	QSqlQuery query;
	query.prepare(
		"DELETE FROM work_creator "
		"WHERE work_id = (:work_id) AND creator_id = (:creator_id) AND type = (:type)"
	);
	query.bindValue(":work_id", work_id);
	query.bindValue(":creator_id", creator_id);
	query.bindValue(":type", type);

	if (!query.exec()) {
		qDebug() << query.lastError();
	}
}

//==================================================================================================================================
//==================================================================================================================================
//==================================================================================================================================
