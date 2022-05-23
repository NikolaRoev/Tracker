#include "MainWindow/MainWindow.h"

#include <QApplication>
#include <QFile>
#include <QtGlobal>

//==================================================================================================================================

int main(int argc, char* argv[]) {
	qSetMessagePattern("[%{if-debug}Debug%{endif}"
					   "%{if-info}Info%{endif}"
					   "%{if-warning}Warning%{endif}"
					   "%{if-critical}Critical%{endif}"
					   "%{if-fatal}Fatal%{endif}]"
					   "[%{time hh:mm:ss.zzz}][%{file}:%{line}][%{function}]:\n%{message}");


	QApplication app(argc, argv);

	QFile file(":/stylesheets/stylesheets/stylesheet.qss");
	if (!file.open(QFile::ReadOnly)) {
		qWarning() << file.errorString();
	}
	//app.setStyleSheet(file.readAll());

	MainWindow window;
	window.show();
	return app.exec();
}

//==================================================================================================================================
