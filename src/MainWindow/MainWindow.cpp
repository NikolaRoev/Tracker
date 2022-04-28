#include "MainWindow.h"
#include "./ui_MainWindow.h"
#include "DatabaseManager/DatabaseManager.h"
#include "DatabaseManager/Work.h"
#include "DatabaseManager/Creator.h"
#include "UpdateEntry/UpdateEntry.h"

#include <QMainWindow>
#include <QSettings>
#include <QShortcut>
#include <QFileDialog>
#include <QString>
#include <QListWidgetItem>
#include <QMessageBox>

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

	//Works, selected work.
	ui->worksStatusComboBox->setItemData(0, "Reading");
	ui->worksStatusComboBox->setItemData(1, "Completed");

	ui->worksTypeComboBox->setItemData(0, "Series");
	ui->worksTypeComboBox->setItemData(1, "One Shot");
	ui->worksTypeComboBox->setItemData(2, "Anthology");



	//Load settings.
	QSettings settings("settings.ini", QSettings::IniFormat, this);
	settings.beginGroup("window");
	restoreGeometry(settings.value("geometry").toByteArray());
	restoreState(settings.value("state").toByteArray());
	settings.endGroup();

	settings.beginGroup("user");
	DatabaseManager::open(settings.value("default_database").toString());
	settings.endGroup();


	//Populate the update entries when the program starts initially.
	emit ui->tabWidget->currentChanged(ui->tabWidget->currentIndex());
}

//==================================================================================================================================

MainWindow::~MainWindow() {
	//Save settings.
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

void MainWindow::on_actionNew_triggered() {
	QString file = QFileDialog::getSaveFileName(this, "New Database", "", "Databases (*.db)");
	DatabaseManager::open(file);

	emit ui->tabWidget->currentChanged(ui->tabWidget->currentIndex());
}

//==================================================================================================================================

void MainWindow::on_actionOpen_triggered() {
	QString file = QFileDialog::getOpenFileName(this, "Open Database", "", "Databases (*.db)");
	DatabaseManager::open(file);

	emit ui->tabWidget->currentChanged(ui->tabWidget->currentIndex());
}

//==================================================================================================================================

void MainWindow::on_actionClose_triggered() {
	DatabaseManager::close();

	emit ui->tabWidget->currentChanged(ui->tabWidget->currentIndex());
}

//==================================================================================================================================

void MainWindow::on_actionExit_triggered() {
	QApplication::exit();
}

//==================================================================================================================================
//==================================================================================================================================

void MainWindow::on_tabWidget_currentChanged(int index) {
	switch (index) {
		case 0: emit ui->updateSearchLineEdit->textChanged(ui->updateSearchLineEdit->text()); break;
		case 1: emit ui->worksFilterLineEdit->textChanged(ui->worksFilterLineEdit->text()); break;
		case 2: emit ui->creatorsFilterLineEdit->textChanged(ui->creatorsFilterLineEdit->text()); break;
	}
}

//==================================================================================================================================
//==================================================================================================================================

void MainWindow::on_updateSearchLineEdit_textChanged(const QString& text) {
	//Clear widgets from layout.
	QLayoutItem* child{ nullptr };
	while ((child = ui->updateContentsWidget->layout()->takeAt(0)) != nullptr) {
		delete child->widget();
		delete child;
	}

	//Find works and populate the update list.
	const auto found_works = DatabaseManager::search_update_works(text);
	for (const auto& found_work : found_works) {
		ui->updateContentsWidget->layout()->addWidget(
			new UpdateEntry(found_work, ui->updateScrollArea)
		);
	}

	//Update status bar.
	ui->statusBar->showMessage(QString("Found %1 entries.").arg(found_works.size()));
}

//==================================================================================================================================
//==================================================================================================================================

void MainWindow::on_worksFilterLineEdit_textChanged(const QString& text) {
	//Clear list items.
	ui->worksListWidget->clear();

	//Find works and populate the list using the selected 'by' search criteria.
	QString by = ui->worksFilterByComboBox->currentData().toString();
	QVector<Work> found_works;

	if (by == "Name") {
		found_works = DatabaseManager::search_works_by_name(text,
															ui->worksFilterStatusComboBox->currentData().toString(),
															ui->worksFilterTypeComboBox->currentData().toString());
	}

	for (const auto& work : found_works) {
		QListWidgetItem* item = new QListWidgetItem(work.name);
		item->setData(Qt::UserRole, work.id);
		ui->worksListWidget->addItem(item);
	}
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

void MainWindow::on_worksListWidget_itemSelectionChanged() {
	//Clear list items.
	ui->worksAuthorListWidget->clear();
	ui->worksArtistListWidget->clear();


	auto selected_items = ui->worksListWidget->selectedItems();

	if (!selected_items.isEmpty()) {
		//Enable input widgets when an item is selected.
		ui->worksNameLineEdit->setEnabled(true);
		ui->worksStatusComboBox->setEnabled(true);
		ui->worksTypeComboBox->setEnabled(true);
		ui->worksGroupingLineEdit->setEnabled(true);
		ui->worksChapterLineEdit->setEnabled(true);

		ui->worksAuthorEditButton->setEnabled(true);
		ui->worksAuthorListWidget->setEnabled(true);
		ui->worksArtistEditButton->setEnabled(true);
		ui->worksArtistListWidget->setEnabled(true);


		const auto work = DatabaseManager::search_work(selected_items.first()->data(Qt::UserRole).toInt());

		ui->worksIdLabel->setText(QString::number(work.id));
		ui->worksNameLineEdit->setText(work.name);
		ui->worksStatusComboBox->setCurrentText(work.status);
		ui->worksTypeComboBox->setCurrentText(work.type);
		ui->worksGroupingLineEdit->setText(work.grouping);
		ui->worksChapterLineEdit->setText(work.chapter);
		ui->worksUpdatedLabel->setText(work.updated);
		ui->worksAddedLabel->setText(work.added);

		for (const auto& creator : work.creators) {
			QListWidgetItem* item = new QListWidgetItem(creator.name);
			item->setData(Qt::UserRole, creator.id);

			if (creator.type == "Author") {
				ui->worksAuthorListWidget->addItem(item);
			}
			else if (creator.type == "Artist") {
				ui->worksArtistListWidget->addItem(item);
			}
		}
	}
	else {
		ui->worksIdLabel->clear();
		ui->worksNameLineEdit->clear();
		ui->worksStatusComboBox->setCurrentIndex(0);
		ui->worksTypeComboBox->setCurrentIndex(0);
		ui->worksGroupingLineEdit->clear();
		ui->worksChapterLineEdit->clear();
		ui->worksUpdatedLabel->clear();
		ui->worksAddedLabel->clear();

		//Disable input widgets when no item is selected.
		ui->worksNameLineEdit->setDisabled(true);
		ui->worksStatusComboBox->setDisabled(true);
		ui->worksTypeComboBox->setDisabled(true);
		ui->worksGroupingLineEdit->setDisabled(true);
		ui->worksChapterLineEdit->setDisabled(true);

		ui->worksAuthorEditButton->setDisabled(true);
		ui->worksAuthorListWidget->setDisabled(true);
		ui->worksArtistEditButton->setDisabled(true);
		ui->worksArtistListWidget->setDisabled(true);
	}
}

//==================================================================================================================================

void MainWindow::on_worksListWidget_itemDoubleClicked(QListWidgetItem* item) {
	int result = QMessageBox::warning(this,
									  "Deleting Entry",
									  QString("Are you sure you want to delete \"%1\"?").arg(item->text()),
									  QMessageBox::Yes,
									  QMessageBox::No);

	if (result == QMessageBox::Yes) {
		DatabaseManager::remove_work(item->data(Qt::UserRole).toInt());
		delete item;
	}
}

//==================================================================================================================================
//==================================================================================================================================

void MainWindow::on_worksAddButton_clicked() {
	//TO DO: Add.
}

//==================================================================================================================================

void MainWindow::on_worksNameLineEdit_textEdited(const QString& text) {
	DatabaseManager::update_work("name", ui->worksIdLabel->text().toInt(), text);

	//Update the name of the selected work.
	ui->worksListWidget->selectedItems().first()->setText(text);
}

//==================================================================================================================================

void MainWindow::on_worksStatusComboBox_currentIndexChanged(int index) {
	DatabaseManager::update_work("status", ui->worksIdLabel->text().toInt(), ui->worksStatusComboBox->itemData(index).toString());
}

//==================================================================================================================================

void MainWindow::on_worksTypeComboBox_currentIndexChanged(int index) {
	DatabaseManager::update_work("type", ui->worksIdLabel->text().toInt(), ui->worksTypeComboBox->itemData(index).toString());
}

//==================================================================================================================================

void MainWindow::on_worksGroupingLineEdit_textEdited(const QString& text) {
	DatabaseManager::update_work("grouping", ui->worksIdLabel->text().toInt(), text);
}

//==================================================================================================================================

void MainWindow::on_worksChapterLineEdit_textEdited(const QString& text) {
	DatabaseManager::update_work("chapter", ui->worksIdLabel->text().toInt(), text);
}

//==================================================================================================================================

void MainWindow::on_worksAuthorEditButton_clicked() {
	//TO DO: Add.
}

//==================================================================================================================================

void MainWindow::on_worksArtistEditButton_clicked() {
	//TO DO: Add.
}

//==================================================================================================================================

void MainWindow::on_worksAuthorListWidget_itemClicked(QListWidgetItem* item) {
	//TO DO: Add.
}

//==================================================================================================================================

void MainWindow::on_worksArtistListWidget_itemClicked(QListWidgetItem* item) {
	//TO DO: Add.
}

//==================================================================================================================================
//==================================================================================================================================
//==================================================================================================================================

void MainWindow::on_creatorsFilterLineEdit_textChanged(const QString& text) {

}

//==================================================================================================================================

void MainWindow::on_creatorsListWidget_itemSelectionChanged() {

}

//==================================================================================================================================

void MainWindow::on_creatorsListWidget_itemDoubleClicked(QListWidgetItem* item) {

}

//==================================================================================================================================

void MainWindow::on_creatorsAddButton_clicked() {

}

//==================================================================================================================================

void MainWindow::on_creatorsNameLineEdit_textEdited(const QString& text) {

}

//==================================================================================================================================

void MainWindow::on_creatorsWorksListWidget_itemClicked(QListWidgetItem* item) {

}

//==================================================================================================================================
//==================================================================================================================================
//==================================================================================================================================
