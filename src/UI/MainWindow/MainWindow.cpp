#include "MainWindow.h"
#include "./ui_MainWindow.h"
#include "DatabaseManager/DatabaseManager.h"
#include "DatabaseManager/Work.h"
#include "DatabaseManager/Creator.h"
#include "UpdateEntry/UpdateEntry.h"
#include "AddWorkDialog/AddWorkDialog.h"
#include "AddCreatorDialog/AddCreatorDialog.h"

#include <QMainWindow>
#include <QSettings>
#include <QShortcut>
#include <QFileDialog>
#include <QString>
#include <QListWidgetItem>
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
	ui->updateContentsWidget->layout()->setAlignment(Qt::AlignTop);

	//Set default focus to the search bar.
	//As 'tabWidget' is first in the tab order it gets focused when the application starts.
	ui->tabWidget->setFocusProxy(ui->updateSearchLineEdit);


	//Add a shortcut that selects all text in the search bar and focuses it.
	QShortcut* shortcut = new QShortcut(QKeySequence(Qt::CTRL | Qt::Key_F), this);
	connect(shortcut, &QShortcut::activated, ui->updateSearchLineEdit,
			[&](){ ui->updateSearchLineEdit->selectAll(); ui->updateSearchLineEdit->setFocus(); });


	//Set the various Combo Box Items data.
	//Works, filters.
	ui->worksFilterStatusComboBox->setItemData(0, "");
	ui->worksFilterStatusComboBox->setItemData(1, "Reading");
	ui->worksFilterStatusComboBox->setItemData(2, "Completed");

	ui->worksFilterTypeComboBox->setItemData(0, "");
	ui->worksFilterTypeComboBox->setItemData(1, "Series");
	ui->worksFilterTypeComboBox->setItemData(2, "One Shot");
	ui->worksFilterTypeComboBox->setItemData(3, "Anthology");

	ui->worksFilterByComboBox->setItemData(0, "Name");
	ui->worksFilterByComboBox->setItemData(1, "Chapter");
	ui->worksFilterByComboBox->setItemData(2, "Creator");
	ui->worksFilterByComboBox->setItemData(3, "Grouping");


	//Set resize mode for the Works table widget.
	ui->worksTableWidget->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);


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
		case 0: emit ui->updateSearchLineEdit->textChanged(ui->updateSearchLineEdit->text()); break;
		case 1: emit ui->worksFilterLineEdit->textChanged(ui->worksFilterLineEdit->text()); break;
	}
}

//==================================================================================================================================
//==================================================================================================================================

void MainWindow::on_updateSearchLineEdit_textChanged(const QString& text) {
	//Clear the current update entries.
	QLayoutItem* child{ nullptr };
	while ((child = ui->updateContentsWidget->layout()->takeAt(0)) != nullptr) {
		delete child->widget();
		delete child;
	}

	//Find works and populate the update list.
	const auto found_works = DatabaseManager::search_works(text, "name", "Reading");
	for (const auto& found_work : found_works) {
		ui->updateContentsWidget->layout()->addWidget( new UpdateEntry(found_work, ui->updateScrollArea));
	}

	//Update status bar.
	ui->statusBar->showMessage(QString("Found %1 entries.").arg(found_works.size()));
}

//==================================================================================================================================
//==================================================================================================================================

void MainWindow::on_worksFilterLineEdit_textChanged(const QString& text) {
	//Clear list items.
	ui->worksTableWidget->setRowCount(0);

	//Find works and populate the list using the selected 'by' search criteria.
	QString by = ui->worksFilterByComboBox->currentData().toString();
	QVector<Work> found_works;

	if (by == "Name") {
		found_works = DatabaseManager::search_works(text, "name",
															ui->worksFilterStatusComboBox->currentData().toString(),
															ui->worksFilterTypeComboBox->currentData().toString());
	}
	//TO DO: Add the other 'by' searches.

	for (const auto& work : found_works) {
		QTableWidgetItem* name_item = new QTableWidgetItem(work.name);
		name_item->setFlags(Qt::ItemIsSelectable | Qt::ItemIsEnabled);
		name_item->setData(Qt::UserRole, work.id);

		QTableWidgetItem* chapter_item = new QTableWidgetItem(work.chapter);
		chapter_item->setFlags(Qt::NoItemFlags);


		ui->worksTableWidget->insertRow(ui->worksTableWidget->rowCount());
		ui->worksTableWidget->setItem(ui->worksTableWidget->rowCount() - 1, 0, name_item);
		ui->worksTableWidget->setItem(ui->worksTableWidget->rowCount() - 1, 1, chapter_item);
	}

	//Update status bar.
	ui->statusBar->showMessage(QString("Found %1 entries.").arg(found_works.size()));
}

//==================================================================================================================================

void MainWindow::on_worksFilterStatusComboBox_currentIndexChanged(int index) {
	emit ui->worksFilterLineEdit->textChanged(ui->worksFilterLineEdit->text());
}

//==================================================================================================================================

void MainWindow::on_worksFilterTypeComboBox_currentIndexChanged(int index) {
	emit ui->worksFilterLineEdit->textChanged(ui->worksFilterLineEdit->text());
}

//==================================================================================================================================

void MainWindow::on_worksFilterByComboBox_currentIndexChanged(int index) {
	emit ui->worksFilterLineEdit->textChanged(ui->worksFilterLineEdit->text());
}

//==================================================================================================================================
//==================================================================================================================================

void MainWindow::on_worksTableWidget_itemSelectionChanged() {
	auto selected_items = ui->worksTableWidget->selectedItems();
	if (!selected_items.isEmpty()) {
		const auto work = DatabaseManager::get_work(selected_items.first()->data(Qt::UserRole).toInt());
		qDebug() << "Open new page.";
	}
}

//==================================================================================================================================

void MainWindow::on_worksTableWidget_customContextMenuRequested(const QPoint& pos) {
	if (QTableWidgetItem* item = ui->worksTableWidget->itemAt(pos); item) {
		if (QVariant data = item->data(Qt::UserRole); data.isValid()) {
			QMenu menu(ui->worksTableWidget);
			menu.addAction("Remove", [&](){
				int result = QMessageBox::warning(this,
												  "Deleting Entry",
												  QString("Are you sure you want to delete \"%1\"?").arg(item->text()),
												  QMessageBox::Yes,
												  QMessageBox::No);

				if (result == QMessageBox::Yes) {
					DatabaseManager::remove_work(item->data(Qt::UserRole).toInt());
					ui->worksTableWidget->removeRow(item->row());
				}
			});
			menu.exec(QCursor::pos());
		}
	}
}

//==================================================================================================================================
//==================================================================================================================================
//==================================================================================================================================
