#pragma once
#include <QString>

//==================================================================================================================================

class DatabaseManager{
public:
	static void init();
	static void deinit();

	static void add_work(const QString name, const QString status, const QString type,
						 const QString grouping, const QString chapter);

	static void add_creator(const QString name, const QString work);

	static void remove_work(const QString name);
};

//==================================================================================================================================
