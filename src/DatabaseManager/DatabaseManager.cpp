#include "pch.h"
#include "DatabaseManager.h"
#include "Work.h"
#include "Creator.h"

//==================================================================================================================================
//==================================================================================================================================
//==================================================================================================================================

bool DatabaseManager::init(const QString& database_name) {
	QSqlDatabase::addDatabase("QSQLITE");

	QSqlDatabase db = QSqlDatabase::database();
	db.setDatabaseName(database_name);
	if (!db.open()) {
		QSqlError error = db.lastError();
		qCritical() << QString("Failed to open database [%1], with error [%2]: [%3]").arg(database_name).arg(error.type()).arg(error.text());
		return false;
	}

	//Activate Foreign Keys. It seems Qt compiles SQLite with the Foreign Keys flag disabled.
	QSqlQuery query;
	if (!query.exec("PRAGMA foreign_keys = ON")) {
		QSqlError error = query.lastError();
		qCritical() << QString("Failed to turn on Foreign Keys, with error [%1]: [%2]").arg(error.type()).arg(error.text());
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
		"	added		TEXT, "
		"	md_id		TEXT UNIQUE, "
		"	mu_id		TEXT UNIQUE"
		")"
	);
	if (!query.exec()) {
		QSqlError error = query.lastError();
		qCritical() << QString("Failed to create Works table, with error [%1]: [%2]").arg(error.type()).arg(error.text());
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
		QSqlError error = query.lastError();
		qCritical() << QString("Failed to create Creators table, with error [%1]: [%2]").arg(error.type()).arg(error.text());
		return false;
	}

	//Create the Work-Creator table.
	query.prepare(
		"CREATE TABLE IF NOT EXISTS work_creator ("
		"	work_id		INTEGER NOT NULL, "
		"	creator_id	INTEGER NOT NULL, "
		"	type		TEXT NOT NULL, "
		"	PRIMARY KEY (work_id, creator_id), "
		"	FOREIGN KEY (work_id) REFERENCES works (id) ON DELETE CASCADE, "
		"	FOREIGN KEY (creator_id) REFERENCES creators (id) ON DELETE CASCADE"
		")"
	);
	if (!query.exec()) {
		QSqlError error = query.lastError();
		qCritical() << QString("Failed to create Work-Creator table, with error [%1]: [%2]").arg(error.type()).arg(error.text());
		return false;
	}


	return true;
}

//==================================================================================================================================

void DatabaseManager::deinit() {
	QSqlDatabase db = QSqlDatabase::database();
	db.close();
	QSqlDatabase::removeDatabase("QSQLITE");
}

//==================================================================================================================================
//==================================================================================================================================
//==================================================================================================================================

bool DatabaseManager::add_work(const Work& work) {
	QSqlQuery query;
	query.prepare(
		"INSERT INTO works (name, status, type, chapter, updated, added, md_id, mu_id) "
		"VALUES (:name, :status, :type, :chapter, :updated, :added, :md_id, :mu_id)"
	);
	query.bindValue(":name", work.name);
	query.bindValue(":status", work.status);
	query.bindValue(":type", work.type);
	query.bindValue(":chapter", work.chapter);
	query.bindValue(":updated", work.updated);
	query.bindValue(":added", work.added);
	query.bindValue(":md_id", work.md_id);
	query.bindValue(":mu_id", work.mu_id);

	if (query.exec()) {
		return true;
	}
	else {
		QSqlError error = query.lastError();
		qWarning() << QString("Failed to create Work [%1], with error [%2]: [%3]").arg(work.name).arg(error.type()).arg(error.text());
		return false;
	}
}

//==================================================================================================================================

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
		QSqlError error = query.lastError();
		qWarning() << QString("Failed to remove Work [%1], with error [%2]: [%3]").arg(id).arg(error.type()).arg(error.text());
		return false;
	}
}

//==================================================================================================================================

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
		QSqlError error = query.lastError();
		qWarning() << QString("Failed to update Column [%1] for Work [%2] to value [%3], with error [%4]: [%5]")
							  .arg(id).arg(column).arg(value).arg(error.type()).arg(error.text());
		return false;
	}
}

//==================================================================================================================================

bool DatabaseManager::get_work(Work& work, const int id) {
	//Select the Work.
	QSqlQuery query;
	query.prepare(
		"SELECT name, status, type, chapter, updated, added, md_id, mu_id "
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
			work.md_id = query.value(6).toString();
			work.mu_id = query.value(7).toString();
		}
	}
	else {
		QSqlError error = query.lastError();
		qWarning() << QString("Failed to get Work [%1], with error [%2]: [%3]").arg(id).arg(error.type()).arg(error.text());
		return false;
	}

	//Find all the associated Creators.
	query.prepare(
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
	query.bindValue(":work_id", work.id);

	if (query.exec()) {
		while (query.next()) {
			work.creators.emplace_back(Creator{
				.id = query.value(0).toInt(),
				.name = query.value(1).toString(),
				.type = query.value(2).toString()
			});
		}
	}
	else {
		QSqlError error = query.lastError();
		qWarning() << QString("Failed to get Creators for Work [%1], with error [%2]: [%3]")
							  .arg(work.name).arg(error.type()).arg(error.text());
		return false;
	}


	return true;
}

//==================================================================================================================================

QVector<Work> DatabaseManager::search_works(const QString& search, const QString& by, const QString& status, const QString& type) {
	//Query text construction.
	QString query_text;

	if (by == "creator") {
		query_text = QString(
			"WITH current_creators AS ("
			"	SELECT id, name "
			"	FROM creators "
			"	WHERE name LIKE (:search)"
			"), "
			"current_works AS ("
			"	SELECT DISTINCT work_id "
			"	FROM work_creator "
			"	INNER JOIN current_creators "
			"	ON current_creators.id = work_creator.creator_id"
			") "
			"SELECT works.id, works.name, works.status, works.type, works.chapter, works.updated, works.added, works.md_id, works.mu_id "
			"FROM works "
			"INNER JOIN current_works "
			"ON current_works.work_id = works.id "
			"WHERE 1 = 1"
		);
	}
	else {
		query_text = QString(
			"SELECT id, name, status, type, chapter, updated, added, md_id, mu_id "
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


	//Query preparation.
	QSqlQuery query;
	query.prepare(query_text);
	query.bindValue(":search", '%' + search + '%');

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
			out.emplace_back(Work{
				.id = query.value(0).toInt(),
				.name = query.value(1).toString(),
				.status = query.value(2).toString(),
				.type = query.value(3).toString(),
				.chapter = query.value(4).toString(),
				.updated = query.value(5).toString(),
				.added = query.value(6).toString(),
				.md_id = query.value(7).toString(),
				.mu_id = query.value(8).toString()
			});
		}
	}
	else {
		qWarning() << query.lastError();
		QMessageBox::warning(m_parent,
			QString("Failed search for Work [%1] by [%2] with status [%3] and type [%4].").arg(search, by, status, type),
			query.lastError().text());
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
		qWarning() << query.lastError();
		QMessageBox::warning(m_parent, QString("Failed to add Creator [%1].").arg(name), query.lastError().text());
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
		qWarning() << query.lastError();
		QMessageBox::warning(m_parent, QString("Failed to remove Creator [%1].").arg(id), query.lastError().text());
	}
}

//==================================================================================================================================

void DatabaseManager::update_creator(const QString& column, const int id, const QString& value) {
	QSqlQuery query;
	query.prepare(QString(
		"UPDATE creators "
		"SET %1 = (:value) "
		"WHERE id = (:id)"
	).arg(column));
	query.bindValue(":value", value);
	query.bindValue(":id", id);

	if (!query.exec()) {
		qWarning() << query.lastError();
		QMessageBox::warning(m_parent,
			QString("Failed to update Creator [%1] property [%2] to [%3].").arg(id).arg(column, value),
			query.lastError().text());
	}
}

//==================================================================================================================================

Creator DatabaseManager::get_creator(const int id) {
	//Select the Creator.
	QSqlQuery query;
	query.prepare(
		"SELECT name "
		"FROM creators "
		"WHERE id = (:id)"
	);
	query.bindValue(":id", id);

	Creator out;
	if (query.exec()) {
		if (query.next()) {
			out.id = id;
			out.name = query.value(0).toString();
		}
	}
	else {
		qWarning() << query.lastError();
		QMessageBox::warning(m_parent, QString("Failed to get Creator [%1].").arg(id), query.lastError().text());
	}

	//Find all the associated Works.
	query.prepare(
		"WITH current_works AS ("
		"	SELECT DISTINCT work_id AS id "
		"	FROM work_creator "
		"	WHERE creator_id = (:creator_id)"
		") "
		"SELECT current_works.id, works.name, works.status, works.type, works.chapter, works.updated, works.added "
		"FROM works "
		"INNER JOIN current_works "
		"ON works.id = current_works.id"
	);
	query.bindValue(":creator_id", out.id);

	if (query.exec()) {
		while (query.next()) {
			out.works.emplace_back(Work{
				.id = query.value(0).toInt(),
				.name = query.value(1).toString(),
				.status = query.value(2).toString(),
				.type = query.value(3).toString(),
				.chapter = query.value(4).toString(),
				.updated = query.value(5).toString(),
				.added = query.value(6).toString()
			});
		}
	}
	else {
		qWarning() << query.lastError();
		QMessageBox::warning(m_parent, QString("Failed to get Works for Creator [%1].").arg(id), query.lastError().text());
	}


	return out;
}

//==================================================================================================================================

QVector<Creator> DatabaseManager::search_creators(const QString& search) {
	QSqlQuery query;
	query.prepare("SELECT id, name "
				  "FROM creators "
				  "WHERE name LIKE (:name)"
	);
	query.bindValue(":name", '%' + search + '%');

	QVector<Creator> out;
	if (query.exec()) {
		while (query.next()) {
			out.emplace_back(Creator{
				.id = query.value(0).toInt(),
				.name = query.value(1).toString()
			});
		}
	}
	else {
		qWarning() << query.lastError();
		QMessageBox::warning(m_parent, QString("Failed search for Creator [%1].").arg(search), query.lastError().text());
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
		qWarning() << query.lastError();
		QMessageBox::warning(m_parent,
			QString("Failed to attach Creator [%1] to Work [%2] as type [%3].").arg(creator_id, work_id).arg(type),
			query.lastError().text());
	}
}

//==================================================================================================================================

void DatabaseManager::detach_creator(const int work_id, const int creator_id) {
	QSqlQuery query;
	query.prepare(
		"DELETE FROM work_creator "
		"WHERE work_id = (:work_id) AND creator_id = (:creator_id)"
	);
	query.bindValue(":work_id", work_id);
	query.bindValue(":creator_id", creator_id);

	if (!query.exec()) {
		qWarning() << query.lastError();
		QMessageBox::warning(m_parent,
			QString("Failed to detach Creator [%1] to Work [%2]").arg(creator_id, work_id),
			query.lastError().text());
	}
}

//==================================================================================================================================
//==================================================================================================================================
//==================================================================================================================================
