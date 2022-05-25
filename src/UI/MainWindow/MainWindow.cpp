#include "MainWindow.h"
#include "./ui_MainWindow.h"
#include "DatabaseManager/DatabaseManager.h"
#include "AddWorkDialog/AddWorkDialog.h"
#include "AddCreatorDialog/AddCreatorDialog.h"
#include "UpdatePage/UpdatePage.h"
#include "BrowsePage/BrowsePage.h"
#include "MangaUpdatesPage/MangaUpdatesPage.h"

#include <QWidget>
#include <QMainWindow>
#include <QNetworkAccessManager>
#include <QSettings>
#include <QStatusBar>

//==================================================================================================================================
//==================================================================================================================================

MainWindow::MainWindow(QWidget* parent) : QMainWindow(parent), ui(new Ui::MainWindow) {
	DatabaseManager::init(this);
	ui->setupUi(this);
	network_access_manager = new QNetworkAccessManager(this);


	//Set the QNetworkAccessManager dependency to the pages.
	MangaUpdatesPage* mangaupdates_page = static_cast<MangaUpdatesPage*>(ui->mainStackedWidget->widget(4));
	mangaupdates_page->setup(network_access_manager);

	//Connect the signals for the Status Bar.
	connect(ui->updatePage, &UpdatePage::message, ui->statusBar, &QStatusBar::showMessage);
	connect(ui->browsePage, &BrowsePage::message, ui->statusBar, &QStatusBar::showMessage);

	//Set default focus to the Update Search Bar.
	//As 'listWidget' is first in the tab order it gets focused when the application starts.
	ui->listWidget->setFocusProxy(ui->updatePage);

	//Preselect the Update section, populating the Update entries.
	ui->listWidget->setCurrentRow(0);


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

void MainWindow::on_actionExit_triggered() {
	QApplication::exit();
}

//==================================================================================================================================

void MainWindow::on_actionAdd_Work_triggered() {
	AddWorkDialog* dialog = new AddWorkDialog(this);
	if (int result = dialog->exec(); result == QDialog::Accepted) {
		on_listWidget_currentRowChanged(ui->listWidget->currentRow());
	}
}

//==================================================================================================================================

void MainWindow::on_actionAdd_Creator_triggered() {
	AddCreatorDialog* dialog = new AddCreatorDialog(this);
	if (int result = dialog->exec(); result == QDialog::Accepted) {
		on_listWidget_currentRowChanged(ui->listWidget->currentRow());
	}
}

//==================================================================================================================================
//==================================================================================================================================

void MainWindow::on_listWidget_currentRowChanged(int currentRow) {
	ui->mainStackedWidget->setCurrentIndex(currentRow);

	switch (currentRow) {
		case 0: ui->updatePage->populate(""); break;
		case 1: ui->browsePage->populate(""); break;
	}
}

//==================================================================================================================================
//==================================================================================================================================
