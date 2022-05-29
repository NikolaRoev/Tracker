#pragma once
#include <QTest>

#include "DatabaseManager/DatabaseManager.h"
#include "DatabaseManager/Work.h"
#include "DatabaseManager/Creator.h"

//==================================================================================================================================

class DatabaseManagerTest: public QObject {
	Q_OBJECT
private slots:
	void initTestCase() {
		QVERIFY(DatabaseManager::init("test.db"));
	}

	void firstTest() {

	}

	void cleanupTestCase() {
		DatabaseManager::deinit();
		QVERIFY(QFile::remove("test.db"));
	}
};

//==================================================================================================================================

QTEST_APPLESS_MAIN(DatabaseManagerTest)
#include "./DatabaseManagerTest.moc"

//==================================================================================================================================
