#include "MainWindow.h"
#include "./ui_MainWindow.h"
#include "DatabaseManager/DatabaseManager.h"
#include "UpdateEntry/UpdateEntry.h"

#include <QMainWindow>
#include <QSettings>
#include <QShortcut>
#include <QFileDialog>

//==================================================================================================================================
//==================================================================================================================================
//==================================================================================================================================

MainWindow::MainWindow(QWidget* parent) : QMainWindow(parent), ui(new Ui::MainWindow) {
	ui->setupUi(this);
	DatabaseManager::init();


	//Align the update layout to top so the populated entries look nice.
	ui->updateContentsWidget->layout()->setAlignment(Qt::AlignTop);

	//Set default focus to the search bar.
	//As 'tabWidget' is first in the tab order it gets focused when the application starts.
	ui->tabWidget->setFocusProxy(ui->searchLineEdit);


	//Add a shortcut that selects all text in the search bar and focuses it.
	QShortcut* shortcut = new QShortcut(QKeySequence(Qt::CTRL | Qt::Key_F), this);
	connect(shortcut, &QShortcut::activated, ui->searchLineEdit, [&](){ ui->searchLineEdit->selectAll(); ui->searchLineEdit->setFocus(); });


	QSettings settings("settings.ini", QSettings::IniFormat, this);
	settings.beginGroup("window");
	restoreGeometry(settings.value("geometry").toByteArray());
	restoreState(settings.value("state").toByteArray());
	settings.endGroup();

	settings.beginGroup("user");
	DatabaseManager::open(settings.value("default_database").toString());
	settings.endGroup();


	//Populate the update layout with all entries.
	emit ui->searchLineEdit->textChanged("");
}

//==================================================================================================================================

MainWindow::~MainWindow() {
	QSettings settings("settings.ini", QSettings::IniFormat, this);
	settings.beginGroup("window");
	settings.setValue("geometry", saveGeometry());
	settings.setValue("state", saveState());
	settings.endGroup();

	settings.beginGroup("user");
	settings.setValue("default_database", DatabaseManager::get());
	settings.endGroup();


	DatabaseManager::deinit();
	delete ui;
}

//==================================================================================================================================
//==================================================================================================================================

void MainWindow::on_searchLineEdit_textChanged(const QString& text) {
	//Clear widgets from layout.
	QLayoutItem* child{ nullptr };
	while ((child = ui->updateContentsWidget->layout()->takeAt(0)) != nullptr) {
		delete child->widget();
		delete child;
	}

	const auto found_works = DatabaseManager::search_works(text);
	for (const auto& found_work : found_works) {
		ui->updateContentsWidget->layout()->addWidget(new UpdateEntry(found_work.id, found_work.name, found_work.chapter, ui->updateScrollArea));
	}

	ui->statusBar->showMessage(QString("Found %1 entries.").arg(found_works.size()));
}

//==================================================================================================================================
//==================================================================================================================================

void MainWindow::on_actionNew_triggered() {
	QString file = QFileDialog::getSaveFileName(this, "New Database", "", "Databases (*.db)");
	DatabaseManager::open(file);

	//Populate the update layout with all entries.
	emit ui->searchLineEdit->textChanged("");
}

//==================================================================================================================================

void MainWindow::on_actionOpen_triggered() {
	QString file = QFileDialog::getOpenFileName(this, "Open Database", "", "Databases (*.db)");
	DatabaseManager::open(file);

	//Populate the update layout with all entries.
	emit ui->searchLineEdit->textChanged("");
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
