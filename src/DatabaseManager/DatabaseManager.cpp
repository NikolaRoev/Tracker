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

	QSqlQuery query(QStringLiteral("CREATE TABLE IF NOT EXISTS hfy_reading ("
								   "	id INTEGER AUTO INCREMENT,"
								   "	name TEXT PRIMARY KEY NOT NULL,"
								   "	chapter TEXT,"
								   "	author TEXT NOT NULL,"
								   "	updated TEXT"
								   ");"));
	if (!query.exec()) {
		qDebug() << query.lastError();
	}
}

void DatabaseManager::deinit() {
	QSqlDatabase::removeDatabase("QSQLITE");
}
