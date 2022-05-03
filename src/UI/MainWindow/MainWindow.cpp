#include "MainWindow.h"
#include "./ui_MainWindow.h"
#include "DatabaseManager/DatabaseManager.h"
#include "DatabaseManager/Work.h"
#include "DatabaseManager/Creator.h"
#include "UpdateEntry/UpdateEntry.h"
#include "AddWorkDialog/AddWorkDialog.h"
#include "AddCreatorDialog/AddCreatorDialog.h"
#include "WorkPage/WorkPage.h"
#include "CreatorPage/CreatorPage.h"

#include <QMainWindow>
#include <QSettings>
#include <QShortcut>
#include <QFileDialog>
#include <QString>
#include <QTableWidgetItem>
#include <QMessageBox>
#include <QMenu>
#include <QCursor>

//==================================================================================================================================
//==================================================================================================================================
//==================================================================================================================================

MainWindow::MainWindow(QWidget* parent) : QMainWindow(parent), ui(new Ui::MainWindow) {
	ui->setupUi(this);
	DatabaseManager::init();


	//Align the update layout to top so the populated entries look nice.
	ui->contentsWidget->layout()->setAlignment(Qt::AlignTop);

	//Set default focus to the search bar.
	//As 'tabWidget' is first in the tab order it gets focused when the application starts.
	ui->tabWidget->setFocusProxy(ui->updateLineEdit);

	//Add a shortcut that selects all text in the search bar and focuses it.
	QShortcut* shortcut = new QShortcut(QKeySequence(Qt::CTRL | Qt::Key_F), this);
	connect(shortcut, &QShortcut::activated, ui->updateLineEdit, [&](){ ui->updateLineEdit->selectAll(); ui->updateLineEdit->setFocus(); });

	//Set resize mode for the Browse Table Widget.
	ui->browseTableWidget->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
	ui->browseTableWidget->verticalHeader()->setSectionResizeMode(QHeaderView::ResizeToContents);

	//Set the various Combo Box Items data.
	ui->statusComboBox->setItemData(0, "");
	ui->statusComboBox->setItemData(1, "Reading");
	ui->statusComboBox->setItemData(2, "Completed");

	ui->typeComboBox->setItemData(0, "");
	ui->typeComboBox->setItemData(1, "Series");
	ui->typeComboBox->setItemData(2, "One Shot");
	ui->typeComboBox->setItemData(3, "Anthology");

	ui->byComboBox->setItemData(0, "name");
	ui->byComboBox->setItemData(1, "chapter");
	ui->byComboBox->setItemData(2, "creator");
	ui->byComboBox->setItemData(3, "grouping");



	//Load settings.
	QSettings settings("settings.ini", QSettings::IniFormat, this);
	restoreGeometry(settings.value("geometry").toByteArray());
	restoreState(settings.value("state").toByteArray());

	if (auto default_database = settings.value("default_database").toString(); !default_database.isEmpty()) {
		DatabaseManager::open(default_database);
		ui->actionAdd_Work->setEnabled(true);
		ui->actionAdd_Creator->setEnabled(true);
	}



	//Populate the update entries when the program starts initially.
	emit ui->tabWidget->currentChanged(ui->tabWidget->currentIndex());
}

//==================================================================================================================================

MainWindow::~MainWindow() {
	QSettings settings("settings.ini", QSettings::IniFormat, this);
	settings.setValue("geometry", saveGeometry());
	settings.setValue("state", saveState());
	settings.setValue("default_database", DatabaseManager::get());

	DatabaseManager::deinit();
	delete ui;
}

//==================================================================================================================================
//==================================================================================================================================

void MainWindow::on_actionNew_triggered() {
	QString file = QFileDialog::getSaveFileName(this, "New Database", "", "Databases (*.db)");

	if (!file.isNull()) {
		DatabaseManager::open(file);
		ui->actionAdd_Work->setEnabled(true);
		ui->actionAdd_Creator->setEnabled(true);

		emit ui->tabWidget->currentChanged(ui->tabWidget->currentIndex());
	}
}

//==================================================================================================================================

void MainWindow::on_actionOpen_triggered() {
	QString file = QFileDialog::getOpenFileName(this, "Open Database", "", "Databases (*.db)");

	if (!file.isNull()) {
		DatabaseManager::open(file);
		ui->actionAdd_Work->setEnabled(true);
		ui->actionAdd_Creator->setEnabled(true);

		emit ui->tabWidget->currentChanged(ui->tabWidget->currentIndex());
	}
}

//==================================================================================================================================

void MainWindow::on_actionClose_triggered() {
	DatabaseManager::close();
	ui->actionAdd_Work->setDisabled(true);
	ui->actionAdd_Creator->setDisabled(true);

	emit ui->tabWidget->currentChanged(ui->tabWidget->currentIndex());
}

//==================================================================================================================================

void MainWindow::on_actionExit_triggered() {
	QApplication::exit();
}

//==================================================================================================================================

void MainWindow::on_actionAdd_Work_triggered() {
	AddWorkDialog* dialog = new AddWorkDialog(this);
	dialog->show();
}

//==================================================================================================================================

void MainWindow::on_actionAdd_Creator_triggered() {
	AddCreatorDialog* dialog = new AddCreatorDialog(this);
	dialog->show();
}

//==================================================================================================================================
//==================================================================================================================================

void MainWindow::on_actionBack_triggered() {
	//TO DO:
}

//==================================================================================================================================

void MainWindow::on_actionForward_triggered() {
	//TO DO:
}

//==================================================================================================================================

void MainWindow::on_actionHome_triggered() {
	//TO DO:
}

//==================================================================================================================================
//==================================================================================================================================

void MainWindow::on_tabWidget_currentChanged(int index) {
	switch (index) {
		case 0: emit ui->updateLineEdit->textChanged(ui->updateLineEdit->text()); break;
		case 1: emit ui->whatComboBox->currentIndexChanged(ui->whatComboBox->currentIndex()); break;
	}
}

//==================================================================================================================================
//==================================================================================================================================

void MainWindow::on_updateLineEdit_textChanged(const QString& text) {
	//Clear the current update entries.
	QLayoutItem* child{ nullptr };
	while ((child = ui->contentsWidget->layout()->takeAt(0)) != nullptr) {
		delete child->widget();
		delete child;
	}

	//Find works and populate the update list.
	const auto found_works = DatabaseManager::search_works(text, "name", "Reading");
	for (const auto& found_work : found_works) {
		ui->contentsWidget->layout()->addWidget(new UpdateEntry(found_work, ui->scrollArea));
	}

	//Update status bar.
	ui->statusBar->showMessage(QString("Found %1 entries.").arg(found_works.size()));
}

//==================================================================================================================================
//==================================================================================================================================

void MainWindow::on_browseLineEdit_textChanged(const QString& text) {
	//Clear list items.
	ui->browseTableWidget->setRowCount(0);

	QVector<Work> found_works = DatabaseManager::search_works(text,
															  ui->byComboBox->currentData().toString(),
															  ui->statusComboBox->currentData().toString(),
															  ui->typeComboBox->currentData().toString());

	for (const auto& work : found_works) {
		QTableWidgetItem* name_item = new QTableWidgetItem(work.name);
		name_item->setFlags(Qt::ItemIsSelectable | Qt::ItemIsEnabled);
		name_item->setData(Qt::UserRole, work.id);

		QTableWidgetItem* chapter_item = new QTableWidgetItem(work.chapter);
		chapter_item->setFlags(Qt::NoItemFlags);

		QTableWidgetItem* grouping_item = new QTableWidgetItem(work.grouping);
		grouping_item->setFlags(Qt::NoItemFlags);

		QTableWidgetItem* updated_item = new QTableWidgetItem(work.updated);
		updated_item->setFlags(Qt::NoItemFlags);


		ui->browseTableWidget->insertRow(ui->browseTableWidget->rowCount());
		ui->browseTableWidget->setItem(ui->browseTableWidget->rowCount() - 1, 0, name_item);
		ui->browseTableWidget->setItem(ui->browseTableWidget->rowCount() - 1, 1, chapter_item);
		ui->browseTableWidget->setItem(ui->browseTableWidget->rowCount() - 1, 2, grouping_item);
		ui->browseTableWidget->setItem(ui->browseTableWidget->rowCount() - 1, 3, updated_item);
	}

	//Update status bar.
	ui->statusBar->showMessage(QString("Found %1 entries.").arg(found_works.size()));
}

//==================================================================================================================================

void MainWindow::on_whatComboBox_currentIndexChanged(int index) {
	switch(index) {
		case 0:
			ui->browseTableWidget->setColumnCount(4);
			ui->browseTableWidget->setHorizontalHeaderLabels({ "Name", "Chapter", "Grouping", "Updated" });

			ui->statusComboBox->setEnabled(true);
			ui->typeComboBox->setEnabled(true);
			ui->byComboBox->setEnabled(true);
			break;
		case 1:
			ui->browseTableWidget->setColumnCount(2);
			ui->browseTableWidget->setHorizontalHeaderLabels({ "Name", "No. of Works" });

			ui->statusComboBox->setDisabled(true);
			ui->typeComboBox->setDisabled(true);
			ui->byComboBox->setDisabled(true);
			break;
	}

	emit ui->browseLineEdit->textChanged(ui->browseLineEdit->text());
}

//==================================================================================================================================

void MainWindow::on_statusComboBox_currentIndexChanged(int index) {
	emit ui->browseLineEdit->textChanged(ui->browseLineEdit->text());
}

//==================================================================================================================================

void MainWindow::on_typeComboBox_currentIndexChanged(int index) {
	emit ui->browseLineEdit->textChanged(ui->browseLineEdit->text());
}

//==================================================================================================================================

void MainWindow::on_byComboBox_currentIndexChanged(int index) {
	emit ui->browseLineEdit->textChanged(ui->browseLineEdit->text());
}

//==================================================================================================================================

void MainWindow::on_browseTableWidget_itemClicked(QTableWidgetItem *item) {
	if (QVariant data = item->data(Qt::UserRole); data.isValid()) {
		qDebug() << "Open new page.";

		ui->stackedWidget->addWidget(new CreatorPage(1));
		ui->stackedWidget->setCurrentIndex(1);
	}
}

//==================================================================================================================================

void MainWindow::on_browseTableWidget_customContextMenuRequested(const QPoint& pos) {
	if (QTableWidgetItem* item = ui->browseTableWidget->itemAt(pos); item) {
		if (QVariant data = item->data(Qt::UserRole); data.isValid()) {
			QMenu menu(ui->browseTableWidget);
			menu.addAction("Remove", [&](){
				int result = QMessageBox::warning(this,
												  "Removing",
												  QString("Are you sure you want to remove \"%1\"?").arg(item->text()),
												  QMessageBox::Yes,
												  QMessageBox::No);

				if (result == QMessageBox::Yes) {
					if (ui->whatComboBox->currentIndex() == 0) {
						DatabaseManager::remove_work(data.toInt());
					}
					else {
						DatabaseManager::remove_creator(data.toInt());
					}

					ui->browseTableWidget->removeRow(item->row());
				}
			});
			menu.exec(QCursor::pos());
		}
	}
}

//==================================================================================================================================
//==================================================================================================================================
//==================================================================================================================================
