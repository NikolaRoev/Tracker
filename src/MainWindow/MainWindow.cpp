#include "MainWindow.h"
#include "./ui_MainWindow.h"
#include "DatabaseManager/DatabaseManager.h"
#include "UpdateEntry/UpdateEntry.h"

#include <QMainWindow>
#include <QSettings>
#include <QShortcut>

//==================================================================================================================================
//==================================================================================================================================
//==================================================================================================================================

MainWindow::MainWindow(QWidget* parent) : QMainWindow(parent), ui(new Ui::MainWindow) {
	ui->setupUi(this);

	//Set default focus to the search bar.
	ui->tabWidget->setFocusProxy(ui->searchLineEdit);
	//Add a shortcut that selects and focuses the search bar.
	QShortcut* shortcut = new QShortcut(QKeySequence(Qt::CTRL | Qt::Key_F), this);
	connect(shortcut, &QShortcut::activated, ui->searchLineEdit, [&](){ ui->searchLineEdit->selectAll(); ui->searchLineEdit->setFocus(); });


	QSettings settings("settings.ini", QSettings::IniFormat, this);
	settings.beginGroup("window");
	restoreGeometry(settings.value("geometry").toByteArray());
	restoreState(settings.value("state").toByteArray());
	settings.endGroup();

	DatabaseManager::init();



	for (int i = 0; i < 100; i++) {
		ui->updateVBoxLayout->addWidget(new UpdateEntry(this));
	}
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
