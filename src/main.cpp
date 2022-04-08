#include "MainWindow/MainWindow.h"

#include <QApplication>
#include <QtGlobal>

//==================================================================================================================================

int main(int argc, char *argv[]) {
	qSetMessagePattern("[%{if-debug}Debug%{endif}"
					   "%{if-info}Info%{endif}"
					   "%{if-warning}Warning%{endif}"
					   "%{if-critical}Critical%{endif}"
					   "%{if-fatal}Fatal%{endif}]"
					   "[%{time hh:mm:ss.zzz}][%{file}:%{line}][%{function}]: %{message}");


	QApplication app(argc, argv);
	MainWindow window;
	window.show();
	return app.exec();
}

//==================================================================================================================================
