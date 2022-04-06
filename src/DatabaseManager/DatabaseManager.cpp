#include "DatabaseManager.h"

#include <QDebug>
#include <QString>
#include <QSqlDatabase>
#include <QSqlQuery>
#include <QSqlError>

//==================================================================================================================================
//==================================================================================================================================

void DatabaseManager::init() {
	QSqlDatabase db = QSqlDatabase::addDatabase("QSQLITE");
	db.setDatabaseName("tracker.db");
	if (!db.open()) {
		qDebug() << db.lastError();
	}
	QSqlQuery query;
	query.exec("PRAGMA foreign_keys = ON;");

	//Works table.
	query.prepare(QStringLiteral("CREATE TABLE IF NOT EXISTS works ("
								 "    name			TEXT PRIMARY KEY NOT NULL,"
								 "    status		TEXT CHECK (status IN ('Reading', 'Completed')) NOT NULL,"
								 "    type			TEXT CHECK (type IN ('Series', 'One Shot')) NOT NULL,"
								 "    grouping		TEXT,"
								 "    chapter		TEXT,"
								 "    updated		TEXT"
								 ")"));
	if (!query.exec()) {
		qDebug() << query.lastError();
	}

	//Creators table.
	query.prepare(QStringLiteral("CREATE TABLE IF NOT EXISTS creators ("
								 "    name	TEXT NOT NULL,"
								 "    work	TEXT NOT NULL,"
								 "    PRIMARY KEY (name, work)"
								 "    CONSTRAINT fk_works"
								 "        FOREIGN KEY (work)"
								 "        REFERENCES works (name)"
								 "        ON DELETE CASCADE"
								 ")"));
	if (!query.exec()) {
		qDebug() << query.lastError();
	}
}

//==================================================================================================================================

void DatabaseManager::deinit() {
	QSqlDatabase::removeDatabase("QSQLITE");
}

//==================================================================================================================================
//==================================================================================================================================

void DatabaseManager::add_work(const QString name, const QString status, const QString type, const QString grouping, const QString chapter) {
	QSqlQuery query;
	query.prepare(QStringLiteral("INSERT INTO works (name, status, type, grouping, chapter, updated)"
								 "VALUES (:name, :status, :type, :grouping, :chapter, date('now'))"));
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

void DatabaseManager::add_creator(const QString name, const QString work) {
	QSqlQuery query;
	query.prepare(QStringLiteral("INSERT INTO creators (name, work)"
								 "VALUES (:name, :work)"));
	query.bindValue(":name", name);
	query.bindValue(":work", work);

	if (!query.exec()) {
		qDebug() << query.lastError();
	}
}

//==================================================================================================================================

void DatabaseManager::remove_work(const QString name) {
	QSqlQuery query;
	query.prepare(QStringLiteral("DELETE FROM works WHERE name = (:name)"));
	query.bindValue(":name", name);

	if (!query.exec()) {
		qDebug() << query.lastError();
	}
}

//==================================================================================================================================

QVector<Work> DatabaseManager::get_works(const QString name) {
	QSqlQuery query;
	query.prepare(QStringLiteral("SELECT * FROM works WHERE name LIKE (:name)"));
	query.bindValue(":name", '%' + name + '%');

	if (!query.exec()) {
		qDebug() << query.lastError();
	}

	//Get works.
	QVector<Work> out;
	while (query.next()) {
		auto& temp = out.emplace_back(query.value(0).toString(),
									  query.value(1).toString(),
									  query.value(2).toString(),
									  query.value(3).toString(),
									  query.value(4).toString(),
									  query.value(5).toString());

		temp.creators = DatabaseManager::get_creators(temp.name);
	}

	return out;
}

//==================================================================================================================================

QVector<QString> DatabaseManager::get_creators(const QString work) {
	QSqlQuery query;
	query.prepare(QStringLiteral("SELECT name FROM creators WHERE work = (:work)"));
	query.bindValue(":work", work);

	if (!query.exec()) {
		qDebug() << query.lastError();
	}

	QVector<QString> out;
	while (query.next()) {
		out.emplace_back(query.value(0).toString());
	}

	return out;
}

//==================================================================================================================================
//==================================================================================================================================
