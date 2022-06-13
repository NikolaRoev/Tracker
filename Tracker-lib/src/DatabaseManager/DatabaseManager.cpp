#include "pch.h"
#include "DatabaseManager.h"
#include "Work.h"
#include "Creator.h"

//==================================================================================================================================
//==================================================================================================================================
//==================================================================================================================================

QString DatabaseManager::init(const QString& database_name) {
	QSqlDatabase::addDatabase("QSQLITE");

	QSqlDatabase db = QSqlDatabase::database();
	db.setDatabaseName(database_name);
	if (!db.open()) {
		QSqlError error = db.lastError();
		return QString("Failed to open database [%1], with error [%2]: [%3]").arg(database_name).arg(error.type()).arg(error.text());
	}

	//Activate Foreign Keys. It seems Qt compiles SQLite with the Foreign Keys flag disabled.
	QSqlQuery query;
	if (!query.exec("PRAGMA foreign_keys = ON")) {
		QSqlError error = query.lastError();
		return QString("Failed to turn on Foreign Keys, with error [%1]: [%2]").arg(error.type()).arg(error.text());
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
		return QString("Failed to create Works table, with error [%1]: [%2]").arg(error.type()).arg(error.text());
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
		return QString("Failed to create Creators table, with error [%1]: [%2]").arg(error.type()).arg(error.text());
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
		return QString("Failed to create Work-Creator table, with error [%1]: [%2]").arg(error.type()).arg(error.text());
	}


	return {};
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

QString DatabaseManager::add_work(const Work& work) {
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
		return {};
	}
	else {
		QSqlError error = query.lastError();
		return QString("Failed to add Work [%1], with error [%2]: [%3]").arg(work.name).arg(error.type()).arg(error.text());
	}
}

//==================================================================================================================================

QString DatabaseManager::remove_work(const int id) {
	QSqlQuery query;
	query.prepare(
		"DELETE FROM works "
		"WHERE id = (:id)"
	);
	query.bindValue(":id", id);

	if (query.exec()) {
		return {};
	}
	else {
		QSqlError error = query.lastError();
		return QString("Failed to remove Work [%1], with error [%2]: [%3]").arg(id).arg(error.type()).arg(error.text());
	}
}

//==================================================================================================================================

QString DatabaseManager::update_work(const QString& column, const int id, const QString& value) {
	QSqlQuery query;
	query.prepare(QString(
		"UPDATE works "
		"SET %1 = (:value) "
		"WHERE id = (:id)"
	).arg(column));
	query.bindValue(":value", value);
	query.bindValue(":id", id);

	if (query.exec()) {
		return {};
	}
	else {
		QSqlError error = query.lastError();
		return QString("Failed to update Column [%1] for Work [%2] to value [%3], with error [%4]: [%5]")
					  .arg(column).arg(id).arg(value).arg(error.type()).arg(error.text());
	}
}

//==================================================================================================================================

QString DatabaseManager::get_work(Work& work, const int id) {
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
		return QString("Failed to get Work [%1], with error [%2]: [%3]").arg(id).arg(error.type()).arg(error.text());
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
		return QString("Failed to get Creators for Work [%1], with error [%2]: [%3]").arg(work.name).arg(error.type()).arg(error.text());
	}


	return {};
}

//==================================================================================================================================

QString DatabaseManager::search_works(QList<Work>& works, const QString& search, const QString& by, const QString& status, const QString& type) {
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
	if (query.exec()) {
		while (query.next()) {
			works.emplace_back(Work{
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

		return {};
	}
	else {
		QSqlError error = query.lastError();
		return QString("Failed to search for Works [%1], with error [%2]: [%3]").arg(search).arg(error.type()).arg(error.text());
	}
}

//==================================================================================================================================
//==================================================================================================================================

QString DatabaseManager::add_creator(const Creator& creator) {
	QSqlQuery query;
	query.prepare(
		"INSERT INTO creators (name) "
		"VALUES (:name)"
	);
	query.bindValue(":name", creator.name);

	if (query.exec()) {
		return {};
	}
	else {
		QSqlError error = query.lastError();
		return QString("Failed to add Creator [%1], with error [%2]: [%3]").arg(creator.name).arg(error.type()).arg(error.text());
	}
}

//==================================================================================================================================

QString DatabaseManager::remove_creator(const int id) {
	QSqlQuery query;
	query.prepare(
		"DELETE FROM creators "
		"WHERE id = (:id)"
	);
	query.bindValue(":id", id);

	if (query.exec()) {
		return {};
	}
	else {
		QSqlError error = query.lastError();
		return QString("Failed to remove Creator [%1], with error [%2]: [%3]").arg(id).arg(error.type()).arg(error.text());
	}
}

//==================================================================================================================================

QString DatabaseManager::update_creator(const QString& column, const int id, const QString& value) {
	QSqlQuery query;
	query.prepare(QString(
		"UPDATE creators "
		"SET %1 = (:value) "
		"WHERE id = (:id)"
	).arg(column));
	query.bindValue(":value", value);
	query.bindValue(":id", id);

	if (query.exec()) {
		return {};
	}
	else {
		QSqlError error = query.lastError();
		return QString("Failed to update Column [%1] for Creator [%2] to value [%3], with error [%4]: [%5]")
					  .arg(column).arg(id).arg(value).arg(error.type()).arg(error.text());
	}
}

//==================================================================================================================================

QString DatabaseManager::get_creator(Creator& creator, const int id) {
	//Select the Creator.
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
		}
	}
	else {
		QSqlError error = query.lastError();
		return QString("Failed to get Creator [%1], with error [%2]: [%3]").arg(id).arg(error.type()).arg(error.text());
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
	query.bindValue(":creator_id", creator.id);

	if (query.exec()) {
		while (query.next()) {
			creator.works.emplace_back(Work{
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
		QSqlError error = query.lastError();
		return QString("Failed to get Works for Creator [%1], with error [%2]: [%3]")
					  .arg(creator.name).arg(error.type()).arg(error.text());
	}


	return {};
}

//==================================================================================================================================

QString DatabaseManager::search_creators(QList<Creator>& creators, const QString& search) {
	QSqlQuery query;
	query.prepare("SELECT id, name "
				  "FROM creators "
				  "WHERE name LIKE (:name)"
	);
	query.bindValue(":name", '%' + search + '%');

	if (query.exec()) {
		while (query.next()) {
			creators.emplace_back(Creator{
				.id = query.value(0).toInt(),
				.name = query.value(1).toString()
			});
		}

		return {};
	}
	else {
		QSqlError error = query.lastError();
		return QString("Failed to search for Creators [%1], with error [%2]: [%3]").arg(search).arg(error.type()).arg(error.text());
	}
}

//==================================================================================================================================

QString DatabaseManager::attach_creator(const int work_id, const int creator_id, const QString& type) {
	QSqlQuery query;
	query.prepare(
		"INSERT INTO work_creator (work_id, creator_id, type) "
		"VALUES (:work_id, :creator_id, :type)"
	);
	query.bindValue(":work_id", work_id);
	query.bindValue(":creator_id", creator_id);
	query.bindValue(":type", type);

	if (query.exec()) {
		return {};
	}
	else {
		QSqlError error = query.lastError();
		return QString("Failed to attach Creator [%1] to Work [%2] as [%3], with error [%4]: [%5]")
					  .arg(creator_id).arg(work_id).arg(type).arg(error.type()).arg(error.text());
	}
}

//==================================================================================================================================

QString DatabaseManager::detach_creator(const int work_id, const int creator_id) {
	QSqlQuery query;
	query.prepare(
		"DELETE FROM work_creator "
		"WHERE work_id = (:work_id) AND creator_id = (:creator_id)"
	);
	query.bindValue(":work_id", work_id);
	query.bindValue(":creator_id", creator_id);

	if (query.exec()) {
		return {};
	}
	else {
		QSqlError error = query.lastError();
		return QString("Failed to detach Creator [%1] from Work [%2], with error [%3]: [%4]")
					  .arg(creator_id).arg(work_id).arg(error.type()).arg(error.text());;
	}
}

//==================================================================================================================================
//==================================================================================================================================
//==================================================================================================================================
