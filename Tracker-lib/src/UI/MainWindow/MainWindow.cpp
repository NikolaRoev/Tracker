#include "pch.h"
#include "MainWindow.h"
#include "./ui_MainWindow.h"
#include "DatabaseManager/DatabaseManager.h"
#include "UpdateTab/UpdateTab.h"
#include "BrowseTab/BrowseTab.h"

MainWindow::MainWindow(QWidget* parent) : QMainWindow(parent), ui(new Ui::MainWindow) {
	DatabaseManager::init("database.db");
	ui->setupUi(this);


	//Set default focus to the Update Search Bar.
	//As 'tabWidget' is first in the tab order it gets focused when the application starts.
	ui->tabWidget->setFocusProxy(ui->updateTab);

	//Populate the Update entries.
	emit ui->tabWidget->currentChanged(0);


	//Load settings.
	QSettings settings("settings.ini", QSettings::IniFormat, this);
	settings.beginGroup("Window");
	restoreGeometry(settings.value("geometry").toByteArray());
	restoreState(settings.value("state").toByteArray());
	settings.endGroup();
}

MainWindow::~MainWindow() {
	//Save settings.
	QSettings settings("settings.ini", QSettings::IniFormat, this);
	settings.beginGroup("Window");
	settings.setValue("geometry", saveGeometry());
	settings.setValue("state", saveState());
	settings.endGroup();

	DatabaseManager::deinit();
	delete ui;
}

void MainWindow::on_tabWidget_currentChanged(int index) {
	switch (index) {
		case 0: ui->updateTab->populate(); break;
		case 1: ui->browseTab->populate(); break;
	}
}
