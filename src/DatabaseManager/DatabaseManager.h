#pragma once
#include <QString>

//==================================================================================================================================

class DatabaseManager{
public:
	static void init();
	static void deinit();

	static void add_work(const QString name, const QString status, const QString type,
						 const QString grouping, const QString chapter, const QString updated);

	static void add_creator(const QString name, const QString work);
};

//==================================================================================================================================
