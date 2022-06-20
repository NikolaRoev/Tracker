#include "pch.h"
#include "DatabaseManager.h"
#include "Work.h"
#include "Creator.h"
#include "AttachedCreator.h"

bool DatabaseManager::init(const QString& name) {
	QSqlDatabase::addDatabase("QSQLITE");
	QSqlDatabase db = QSqlDatabase::database();
	db.setDatabaseName(name);
	if (!db.open()) {
		qCritical() << db.lastError();
		return false;
	}

	//Activate Foreign Keys. It seems Qt compiles SQLite with the Foreign Keys flag disabled.
	QSqlQuery query;
	if (!query.exec("PRAGMA foreign_keys = ON")) {
		qCritical() << query.lastError();
		return false;
	}

	//Create the Works table.
	query.prepare(
		"CREATE TABLE IF NOT EXISTS works ("
		"	id			INTEGER PRIMARY KEY NOT NULL, "
		"	name		TEXT NOT NULL, "
		"   status		TEXT CHECK (status IN ('Reading', 'Completed')) NOT NULL, "
		"   type		TEXT CHECK (type IN ('Series', 'One Shot', 'Anthology')) NOT NULL, "
		"   chapter		TEXT, "
		"   updated		TEXT, "
		"	added		TEXT"
		")"
	);
	if (!query.exec()) {
		qCritical() << query.lastError();
		return false;
	}

	//Create the Creators table.
	query.prepare(
		"CREATE TABLE IF NOT EXISTS creators ("
		"	id		INTEGER PRIMARY KEY NOT NULL, "
		"	name	TEXT NOT NULL"
		")"
	);
	if (!query.exec()) {
		qCritical() << query.lastError();
		return false;
	}

	//Create the Work-Creator table.
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
		qCritical() << query.lastError();
		return false;
	}


	return true;
}

void DatabaseManager::deinit() {
	QSqlDatabase db = QSqlDatabase::database();
	db.close();
	QSqlDatabase::removeDatabase("QSQLITE");
}

bool DatabaseManager::add_work(const Work& work) {
	QSqlQuery query;
	query.prepare(
		"INSERT INTO works (name, status, type, chapter, updated, added) "
		"VALUES (:name, :status, :type, :chapter, :updated, :added)"
	);
	query.bindValue(":name", work.name);
	query.bindValue(":status", work.status);
	query.bindValue(":type", work.type);
	query.bindValue(":chapter", work.chapter);
	query.bindValue(":updated", work.updated);
	query.bindValue(":added", work.added);

	if (query.exec()) {
		return true;
	}
	else {
		qWarning() << query.lastError();
		return false;
	}
}

bool DatabaseManager::remove_work(const int id) {
	QSqlQuery query;
	query.prepare(
		"DELETE FROM works "
		"WHERE id = (:id)"
	);
	query.bindValue(":id", id);

	if (query.exec()) {
		return true;
	}
	else {
		qWarning() << query.lastError();
		return false;
	}
}

bool DatabaseManager::update_work(const QString& column, const int id, const QString& value) {
	QSqlQuery query;
	query.prepare(QString(
		"UPDATE works "
		"SET %1 = (:value) "
		"WHERE id = (:id)"
	).arg(column));
	query.bindValue(":value", value);
	query.bindValue(":id", id);

	if (query.exec()) {
		return true;
	}
	else {
		qWarning() << query.lastError();
		return false;
	}
}

bool DatabaseManager::get_work(Work& work, const int id) {
	QSqlQuery query;
	query.prepare(
		"SELECT name, status, type, chapter, updated, added "
		"FROM works "
		"WHERE id = (:id)"
	);
	query.bindValue(":id", id);

	if (query.exec()) {
		if (query.next()) {
			work.id = id;
			work.name = query.value(0).toString();
			work.status = query.value(1).toString();
			work.type = query.value(2).toString();
			work.chapter = query.value(3).toString();
			work.updated = query.value(4).toString();
			work.added = query.value(5).toString();

			return true;
		}
	}
	else {
		qWarning() << query.lastError();
	}

	return false;
}

bool DatabaseManager::get_work_creators(QList<AttachedCreator>& creators, const int id) {
	QSqlQuery query;
	query.prepare(
		"WITH attached_creators AS ("
		"	SELECT creator_id AS id, type "
		"	FROM work_creator "
		"	WHERE work_id = (:work_id)"
		") "
		"SELECT attached_creators.id, creators.name, attached_creators.type "
		"FROM creators "
		"INNER JOIN attached_creators "
		"ON creators.id = attached_creators.id"
	);
	query.bindValue(":work_id", id);

	if (query.exec()) {
		while (query.next()) {
			creators.emplace_back(query.value(0).toInt(), query.value(1).toString(), query.value(2).toString());
		}

		return true;
	}
	else {
		qWarning() << query.lastError();
		return false;
	}
}

bool DatabaseManager::search_works(QList<Work>& works, const QString& search, const QString& by, const QString& status, const QString& type) {
	QString query_text;
	if (by == "creator") {
		query_text = QString(
			"WITH found_creators AS ("
			"	SELECT id "
			"	FROM creators "
			"	WHERE name LIKE (:search)"
			"), "
			"matched_works AS ("
			"	SELECT DISTINCT work_id "
			"	FROM work_creator "
			"	INNER JOIN found_creators "
			"	ON work_creator.creator_id = found_creators.id"
			") "
			"SELECT works.id, works.name, works.status, works.type, works.chapter, works.updated, works.added "
			"FROM works "
			"INNER JOIN matched_works "
			"ON works.id = matched_works.work_id "
			"WHERE 1 = 1"
		);
	}
	else {
		query_text = QString(
			"SELECT id, name, status, type, chapter, updated, added "
			"FROM works "
			"WHERE %1 LIKE (:search)"
		).arg(by);
	}

	if (!status.isEmpty()) {
		query_text.append(" AND status = (:status)");
	}

	if (!type.isEmpty()) {
		query_text.append(" AND type = (:type)");
	}


	QSqlQuery query;
	query.prepare(query_text);
	query.bindValue(":search", QString("%%1%").arg(search));

	if (!status.isNull()) {
		query.bindValue(":status", status);
	}

	if (!type.isNull()) {
		query.bindValue(":type", type);
	}


	if (query.exec()) {
		while (query.next()) {
			works.emplace_back(
				query.value(0).toInt(),
				query.value(1).toString(),
				query.value(2).toString(),
				query.value(3).toString(),
				query.value(4).toString(),
				query.value(5).toString(),
				query.value(6).toString()
			);
		}

		return true;
	}
	else {
		qWarning() << query.lastError();
		return false;
	}
}

bool DatabaseManager::add_creator(const Creator& creator) {
	QSqlQuery query;
	query.prepare(
		"INSERT INTO creators (name) "
		"VALUES (:name)"
	);
	query.bindValue(":name", creator.name);

	if (query.exec()) {
		return true;
	}
	else {
		qWarning() << query.lastError();
		return false;
	}
}

bool DatabaseManager::remove_creator(const int id) {
	QSqlQuery query;
	query.prepare(
		"DELETE FROM creators "
		"WHERE id = (:id)"
	);
	query.bindValue(":id", id);

	if (query.exec()) {
		return true;
	}
	else {
		qWarning() << query.lastError();
		return false;
	}
}

bool DatabaseManager::update_creator(const QString& column, const int id, const QString& value) {
	QSqlQuery query;
	query.prepare(QString(
		"UPDATE creators "
		"SET %1 = (:value) "
		"WHERE id = (:id)"
	).arg(column));
	query.bindValue(":value", value);
	query.bindValue(":id", id);

	if (query.exec()) {
		return true;
	}
	else {
		qWarning() << query.lastError();
		return false;
	}
}

bool DatabaseManager::get_creator(Creator& creator, const int id) {
	QSqlQuery query;
	query.prepare(
		"SELECT name "
		"FROM creators "
		"WHERE id = (:id)"
	);
	query.bindValue(":id", id);

	if (query.exec()) {
		if (query.next()) {
			creator.id = id;
			creator.name = query.value(0).toString();

			return true;
		}
	}
	else {
		qWarning() << query.lastError();
	}

	return false;
}

bool DatabaseManager::get_creator_works(QList<Work>& works, const int id) {
	QSqlQuery query;
	query.prepare(
		"WITH attached_works AS ("
		"	SELECT DISTINCT work_id AS id "
		"	FROM work_creator "
		"	WHERE creator_id = (:creator_id)"
		") "
		"SELECT works.id, works.name, works.status, works.type, works.chapter, works.updated, works.added "
		"FROM works "
		"INNER JOIN attached_works "
		"ON works.id = attached_works.id"
	);
	query.bindValue(":creator_id", id);

	if (query.exec()) {
		while (query.next()) {
			works.emplace_back(
				query.value(0).toInt(),
				query.value(1).toString(),
				query.value(2).toString(),
				query.value(3).toString(),
				query.value(4).toString(),
				query.value(5).toString(),
				query.value(6).toString()
			);
		}

		return true;
	}
	else {
		qWarning() << query.lastError();
		return false;
	}
}

bool DatabaseManager::search_creators(QList<Creator>& creators, const QString& search) {
	QSqlQuery query;
	query.prepare("SELECT id, name "
				  "FROM creators "
				  "WHERE name LIKE (:name)"
	);
	query.bindValue(":name", QString("%%1%").arg(search));

	if (query.exec()) {
		while (query.next()) {
			creators.emplace_back(query.value(0).toInt(), query.value(1).toString());
		}

		return true;
	}
	else {
		qWarning() << query.lastError();
		return false;
	}
}

bool DatabaseManager::attach_creator(const int work_id, const int creator_id, const QString& type) {
	QSqlQuery query;
	query.prepare(
		"INSERT INTO work_creator (work_id, creator_id, type) "
		"VALUES (:work_id, :creator_id, :type)"
	);
	query.bindValue(":work_id", work_id);
	query.bindValue(":creator_id", creator_id);
	query.bindValue(":type", type);

	if (query.exec()) {
		return true;
	}
	else {
		qWarning() << query.lastError();
		return false;
	}
}

bool DatabaseManager::detach_creator(const int work_id, const int creator_id, const QString& type) {
	QSqlQuery query;
	query.prepare(
		"DELETE FROM work_creator "
		"WHERE work_id = (:work_id) AND creator_id = (:creator_id) AND type = (:type)"
	);
	query.bindValue(":work_id", work_id);
	query.bindValue(":creator_id", creator_id);
	query.bindValue(":type", type);

	if (query.exec()) {
		return true;
	}
	else {
		qWarning() << query.lastError();
		return false;
	}
}
