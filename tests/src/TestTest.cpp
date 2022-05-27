#pragma once
#include <QTest>

class TestTest: public QObject {
	Q_OBJECT
private slots:
	void initTestCase()
	{
		qDebug("Called before everything else.");
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

QTEST_APPLESS_MAIN(TestTest)
#include "./TestTest.moc"
