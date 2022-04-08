#include "DatabaseManager.h"
#include "Creator.h"
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
	db.setDatabaseName(name + ".db");
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
								 "    type	TEXT CHECK (type IN ('Author', 'Artist')) NOT NULL,"
								 "    PRIMARY KEY (name, work, type)"
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

void DatabaseManager::close() {
	QSqlDatabase db = QSqlDatabase::database();
	db.close();
}

//==================================================================================================================================
//==================================================================================================================================

void DatabaseManager::add_work(const QString& name, const QString& status, const QString& type, const QString& grouping, const QString& chapter, const QVector<Creator>& creators) {
	QSqlQuery query;

	//Insert the work.
	query.prepare(QStringLiteral("INSERT INTO works (name, status, type, grouping, chapter, updated)"
								 "VALUES (:name, :status, :type, :grouping, :chapter, date('now'))"));
	query.bindValue(":name", name);
	query.bindValue(":status", status);
	query.bindValue(":type", type);
	query.bindValue(":grouping", grouping);
	query.bindValue(":chapter", chapter);

	if (query.exec()) {
		//Insert the creators.
		for (const auto& creator : creators) {
			QSqlQuery query;
			query.prepare(QStringLiteral("INSERT INTO creators (name, work, type)"
										 "VALUES (:name, :work, :type)"));
			query.bindValue(":name", creator.name);
			query.bindValue(":work", name);
			query.bindValue(":type", creator.type);

			if (!query.exec()) {
				qDebug() << query.lastError();
			}
		}
	}
	else {
		qDebug() << query.lastError();
	}
}

//==================================================================================================================================

void DatabaseManager::remove_work(const QString& name) {
	QSqlQuery query;
	query.prepare(QStringLiteral("DELETE FROM works WHERE name = (:name)"));
	query.bindValue(":name", name);

	if (!query.exec()) {
		qDebug() << query.lastError();
	}
}

//==================================================================================================================================
//==================================================================================================================================

QVector<Work> DatabaseManager::search_works(const QString& maybe_partial_name) {
	QSqlQuery query;
	//Works select.
	query.prepare(QStringLiteral("SELECT * FROM works WHERE name LIKE (:name)"));
	query.bindValue(":name", '%' + maybe_partial_name + '%');

	QVector<Work> out;

	if (query.exec()) {
		while (query.next()) {
			auto& temp = out.emplace_back(query.value(0).toString(), query.value(1).toString(),
										  query.value(2).toString(), query.value(3).toString(),
										  query.value(4).toString(), query.value(5).toString());
			//Creators select.
			QSqlQuery creators_query;
			creators_query.prepare(QStringLiteral("SELECT name, type FROM creators WHERE work = (:work)"));
			creators_query.bindValue(":work", temp.name);

			if (creators_query.exec()) {
				while (creators_query.next()) {
					temp.creators.emplace_back(creators_query.value(0).toString(), creators_query.value(1).toString());
				}
			}
			else {
				qDebug() << creators_query.lastError();
			}
		}
	}
	else {
		qDebug() << query.lastError();
	}

	return out;
}

//==================================================================================================================================

QVector<QString> DatabaseManager::search_authors(const QString& maybe_partial_name) {
	QSqlQuery query;
	query.prepare(QStringLiteral("SELECT DISTINCT name FROM creators WHERE name LIKE (:name)"));
	query.bindValue(":name", '%' + maybe_partial_name + '%');

	QVector<QString> out;

	if (query.exec()) {
		while (query.next()) {
			out.emplace_back(query.value(0).toString());
		}
	}
	else {
		qDebug() << query.lastError();
	}

	return out;
}

//==================================================================================================================================
//==================================================================================================================================
//==================================================================================================================================
