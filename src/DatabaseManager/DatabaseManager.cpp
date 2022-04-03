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

	//Works table.
	QSqlQuery query;
	query.prepare(QStringLiteral("CREATE TABLE IF NOT EXISTS works ("
								 "    name			TEXT PRIMARY KEY NOT NULL,"
								 "    status		TEXT CHECK (status IN ('Reading', 'Completed')) NOT NULL,"
								 "    type			TEXT CHECK (type IN ('Series', 'One Shot')) NOT NULL,"
								 "    grouping		TEXT,"
								 "    chapter		TEXT,"
								 "    updated		TEXT"
								 ");"));
	if (!query.exec()) {
		qDebug() << query.lastError();
	}

	//Creators table.
	query.prepare(QStringLiteral("CREATE TABLE IF NOT EXISTS creators ("
								 "    name	TEXT NOT NULL,"
								 "    work	TEXT NOT NULL,"
								 "    FOREIGN KEY (work) REFERENCES works (name)"
								 ");"));
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

void DatabaseManager::add_work(const QString name, const QString status, const QString type, const QString grouping, const QString chapter, const QString updated) {
	QSqlQuery query;
	query.prepare(QStringLiteral("INSERT INTO works (name, status, type, grouping, chapter, updated)"
								 "VALUES (:name, :status, :type, :grouping, :chapter, :updated);"));
	query.bindValue(":name", name);
	query.bindValue(":status", status);
	query.bindValue(":type", type);
	query.bindValue(":grouping", grouping);
	query.bindValue(":chapter", chapter);
	query.bindValue(":updated", updated);

	if (!query.exec()) {
		qDebug() << query.lastError();
	}
}

//==================================================================================================================================

void DatabaseManager::add_creator(const QString name, const QString work) {
	QSqlQuery query;
	query.prepare(QStringLiteral("INSERT INTO creators (name, work)"
								 "VALUES (:name, :work);"));
	query.bindValue(":name", name);
	query.bindValue(":work", work);

	if (!query.exec()) {
		qDebug() << query.lastError();
	}
}

//==================================================================================================================================
//==================================================================================================================================
