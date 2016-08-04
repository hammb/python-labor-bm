#ifndef TEST_CASE_H
#define TEST_CASE_H

#include <QtTest/QtTest>

class TestCase : public QObject
{
	Q_OBJECT

public slots:
	void testFunction() {
		QCOMPARE( "Test", "Test");
	}
	

	
};

#endif
