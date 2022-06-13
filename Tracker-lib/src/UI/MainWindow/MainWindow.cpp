#include "pch.h"
#include "MainWindow.h"
#include "./ui_MainWindow.h"
#include "DatabaseManager/DatabaseManager.h"
#include "RequestsManager/RequestsManager.h"
#include "UpdatePage/UpdatePage.h"
#include "BrowsePage/BrowsePage.h"

//==================================================================================================================================
//==================================================================================================================================

MainWindow::MainWindow(QWidget* parent) : QMainWindow(parent), ui(new Ui::MainWindow) {
	DatabaseManager::init("database.db");
	RequestsManager::init(this);
	ui->setupUi(this);


	//Connect the signals for the Status Bar.
	connect(ui->updatePage, &UpdatePage::message, ui->statusBar, &QStatusBar::showMessage);
	connect(ui->browsePage, &BrowsePage::message, ui->statusBar, &QStatusBar::showMessage);

	//Set default focus to the Update Search Bar.
	//As 'tabWidget' is first in the tab order it gets focused when the application starts.
	ui->tabWidget->setFocusProxy(ui->updatePage);

	//Populate the Update entries.
	emit ui->tabWidget->currentChanged(0);


	//Load settings.
	QSettings settings("settings.ini", QSettings::IniFormat, this);
	settings.beginGroup("Window");
	restoreGeometry(settings.value("geometry").toByteArray());
	restoreState(settings.value("state").toByteArray());
	settings.endGroup();
}

//==================================================================================================================================

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

//==================================================================================================================================
//==================================================================================================================================

void MainWindow::on_tabWidget_currentChanged(int index) {
	switch (index) {
		case 0: ui->updatePage->populate(""); break;
		case 1: ui->browsePage->populate(""); break;
	}
}

//==================================================================================================================================
//==================================================================================================================================
