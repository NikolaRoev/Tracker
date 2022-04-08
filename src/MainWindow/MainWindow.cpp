#include "MainWindow.h"
#include "./ui_MainWindow.h"
#include "DatabaseManager/DatabaseManager.h"

#include <QMainWindow>
#include <QSettings>

//==================================================================================================================================
//==================================================================================================================================
//==================================================================================================================================

MainWindow::MainWindow(QWidget *parent) : QMainWindow(parent), ui(new Ui::MainWindow) {
	ui->setupUi(this);
	ui->tabWidget->setFocusProxy(ui->searchLineEdit);

	QSettings settings("settings.ini", QSettings::IniFormat, this);
	settings.beginGroup("window");
	restoreGeometry(settings.value("geometry").toByteArray());
	restoreState(settings.value("state").toByteArray());
	settings.endGroup();

	DatabaseManager::init();
}

//==================================================================================================================================

MainWindow::~MainWindow() {
	DatabaseManager::deinit();

	QSettings settings("settings.ini", QSettings::IniFormat, this);
	settings.beginGroup("window");
	settings.setValue("geometry", saveGeometry());
	settings.setValue("state", saveState());
	settings.endGroup();

	delete ui;
}

//==================================================================================================================================
//==================================================================================================================================

void MainWindow::on_searchLineEdit_textChanged(const QString& text) {
	qDebug() << text;
}

//==================================================================================================================================
//==================================================================================================================================

void MainWindow::on_actionNew_triggered() {

}

//==================================================================================================================================

void MainWindow::on_actionOpen_triggered() {
	//TO DO: temp.
	DatabaseManager::open("tracker");
}

//==================================================================================================================================

void MainWindow::on_actionClose_triggered() {
	DatabaseManager::close();
}

//==================================================================================================================================

void MainWindow::on_actionExit_triggered() {
	QApplication::exit();
}

//==================================================================================================================================
//==================================================================================================================================
//==================================================================================================================================
