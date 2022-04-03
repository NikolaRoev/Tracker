#include "MainWindow/MainWindow.h"
#include "DatabaseManager/DatabaseManager.h"

#include <QtGlobal>
#include <QApplication>

int main(int argc, char *argv[]) {
	qSetMessagePattern("[%{if-debug}Debug%{endif}"
					   "%{if-info}Info%{endif}"
					   "%{if-warning}Warning%{endif}"
					   "%{if-critical}Critical%{endif}"
					   "%{if-fatal}Fatal%{endif}]"
					   "[%{time hh:mm:ss.zzz}][%{file}:%{line}][%{function}]: %{message}");

	QApplication a(argc, argv);
	MainWindow w;
	w.show();


	DatabaseManager::init();

	DatabaseManager::add_work("work0", "Reading", "Series", NULL, NULL, NULL);
	DatabaseManager::add_work("work1", "Reading", "Series", NULL, NULL, NULL);
	DatabaseManager::add_creator("author0", "work0");
	DatabaseManager::add_creator("author1", "work0");
	DatabaseManager::add_creator("author0", "work1");
	DatabaseManager::add_creator("author1", "work1");

	DatabaseManager::remove_work("work0");

	DatabaseManager::deinit();

	return a.exec();
}
