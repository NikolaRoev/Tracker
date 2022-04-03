#include "DatabaseManager.h"

#include <QDebug>
#include <QString>
#include <QSqlDatabase>
#include <QSqlQuery>
#include <QSqlError>


void DatabaseManager::init() {
	QSqlDatabase db = QSqlDatabase::addDatabase("QSQLITE");
	db.setDatabaseName("tracker.db");

	if (!db.open()) {
		qDebug() << db.lastError();
	}

	//create a table of creators that each has a work associated
	QSqlQuery query(QStringLiteral("CREATE TABLE IF NOT EXISTS works ("
								   "	name		TEXT PRIMARY KEY NOT NULL,"
								   "	status		TEXT CHECK (status IN ('Reading', 'Completed')) NOT NULL,"
								   "	type		TEXT CHECK (status IN ('Series', 'One Shot')) NOT NULL,"
								   "	grouping	TEXT,"
								   "	chapter		TEXT,"
								   "	updated		TEXT"
								   ");"));
	if (!query.exec()) {
		qDebug() << query.lastError();
	}


	query.prepare(QStringLiteral("CREATE TABLE IF NOT EXISTS creators ("
								 "	name TEXT NOT NULL,"
								 "	work TEXT NOT NULL,"
								 "	FOREIGN KEY (work) REFERENCES works (name)"
								 ");"));

	if (!query.exec()) {
		qDebug() << query.lastError();
	}
}

void DatabaseManager::deinit() {
	QSqlDatabase::removeDatabase("QSQLITE");
}

void DatabaseManager::add_reading(const QString name) {
	QSqlQuery query;
	query.prepare(QStringLiteral("INSERT INTO reading (name)"
								 "VALUES (:name);"));
	query.bindValue(":name", name);

	if (!query.exec()) {
		qDebug() << query.lastError();
	}
}

