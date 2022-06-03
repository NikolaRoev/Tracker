#pragma once
#include <QTest>
#include <iostream>

#include "DatabaseManager/DatabaseManager.h"
#include "DatabaseManager/Work.h"
#include "DatabaseManager/Creator.h"

//==================================================================================================================================

class DatabaseManagerTest: public QObject {
	Q_OBJECT
private slots:
	void initTestCase() {
		QVERIFY(DatabaseManager::init("test.db"));

		std::cout << "44444444444444444";
	}

	void cleanup() {
		QSqlQuery query;
		QVERIFY(query.exec("DELETE FROM works"));
		QVERIFY(query.exec("DELETE FROM creators"));
		QVERIFY(query.exec("DELETE FROM work_creator"));
	}


	//Work.
	void add_workTest() {
		Work work;
		work.name = "name";
		work.status = "Reading";
		work.type = "Series";
		work.chapter = "chapter";
		work.updated = "updated";
		work.added = "added";
		work.md_id = "md_id";
		work.mu_id = "mu_id";
		QVERIFY(DatabaseManager::add_work(work));

		QSqlQuery query;
		QVERIFY(query.exec("SELECT * FROM works"));
		QVERIFY(query.next());
		QVERIFY(query.value(0).toInt() == 1);
		QVERIFY(query.value(1).toString() == "name");
		QVERIFY(query.value(2).toString() == "Reading");
		QVERIFY(query.value(3).toString() == "Series");
		QVERIFY(query.value(4).toString() == "chapter");
		QVERIFY(query.value(5).toString() == "updated");
		QVERIFY(query.value(6).toString() == "added");
		QVERIFY(query.value(7).toString() == "md_id");
		QVERIFY(query.value(8).toString() == "mu_id");
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

QTEST_APPLESS_MAIN(DatabaseManagerTest)
#include "./DatabaseManagerTest.moc"

//==================================================================================================================================
