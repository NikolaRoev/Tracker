#pragma once
#include "Creator.h"
#include "Work.h"

#include <QString>
#include <QVector>

//==================================================================================================================================

class DatabaseManager {
public:
	static void init();
	static void deinit();
	static void open(const QString& name);
	static void close();

	static void add_work(const QString& name, const QString& status, const QString& type, const QString& grouping, const QString& chapter, const QVector<Creator>& creators);
	static void remove_work(const QString& name);

	static QVector<Work> search_works(const QString& maybe_partial_name);
	static QVector<QString> search_authors(const QString& maybe_partial_name);
};

//==================================================================================================================================
