#pragma once
#include <QString>

class DatabaseManager{
private:


public:
	static void init();
	static void deinit();

	static void add_reading(const QString name);
};
