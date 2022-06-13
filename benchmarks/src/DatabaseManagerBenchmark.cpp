#pragma once
#include <QTest>

#include "DatabaseManager/DatabaseManager.h"
#include "DatabaseManager/Work.h"
#include "DatabaseManager/Creator.h"

//==================================================================================================================================

class DatabaseManagerBenchmark: public QObject {
	Q_OBJECT
private slots:
	void initTestCase() {
		//QVERIFY(DatabaseManager::init("test.db"));
	}

	void cleanup() {
		QSqlQuery query;
		QVERIFY(query.exec("DELETE FROM works"));
		QVERIFY(query.exec("DELETE FROM creators"));
		QVERIFY(query.exec("DELETE FROM work_creator"));
	}


	//Work.
	void add_workTest() {

	}

	void remove_workTest() {

	}

	void update_workTest() {

	}

	void get_workTest() {

	}

	void search_worksTest() {

	}


	//Creator.

	void cleanupTestCase() {
		DatabaseManager::deinit();
		QVERIFY(QFile::remove("test.db"));
	}
};

//==================================================================================================================================

QTEST_APPLESS_MAIN(DatabaseManagerBenchmark)
#include "./DatabaseManagerBenchmark.moc"

//==================================================================================================================================
