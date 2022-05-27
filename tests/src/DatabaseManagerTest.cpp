#pragma once
#include <QTest>

#include "DatabaseManager/DatabaseManager.h"
#include "DatabaseManager/Work.h"
#include "DatabaseManager/Creator.h"

class DatabaseManagerTest: public QObject {
	Q_OBJECT
private slots:
	void initTestCase()
	{
		DatabaseManager::init("test.db");
		qDebug("Called before everything.");
	}

	void firstTest()
	{
		QVERIFY(true); // check that a condition is satisfied
		QCOMPARE(1, 1); // compare two values
	}

	void cleanupTestCase()
	{
		qDebug("Called after myFirstTest and mySecondTest.");
	}
};

QTEST_APPLESS_MAIN(DatabaseManagerTest)
#include "./DatabaseManagerTest.moc"
