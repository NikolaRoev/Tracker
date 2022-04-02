#include "MainWindow/MainWindow.h"
#include "DatabaseManager/DatabaseManager.h"

#include <QApplication>

int main(int argc, char *argv[]) {
	QApplication a(argc, argv);
	MainWindow w;
	w.show();


	DatabaseManager::init();
	DatabaseManager::deinit();

	return a.exec();
}
