#include "MainWindow.h"
#include "./ui_MainWindow.h"
#include "DatabaseManager/DatabaseManager.h"
#include "DatabaseManager/Work.h"
#include "DatabaseManager/Creator.h"
#include "UpdateEntry/UpdateEntry.h"
#include "AddWorkDialog/AddWorkDialog.h"
#include "AddCreatorDialog/AddCreatorDialog.h"
#include "AttachCreatorDialog/AttachCreatorDialog.h"

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

	//Works, selected work.
	ui->worksStatusComboBox->setItemData(0, "Reading");
	ui->worksStatusComboBox->setItemData(1, "Completed");

	ui->worksTypeComboBox->setItemData(0, "Series");
	ui->worksTypeComboBox->setItemData(1, "One Shot");
	ui->worksTypeComboBox->setItemData(2, "Anthology");


	//Set resize mode for the Works table widget.
	ui->worksTableWidget->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);


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
	const auto found_works = DatabaseManager::search_works_by_name(text, "Reading");
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
	ui->worksTableWidget->setRowCount(0);

	//Find works and populate the list using the selected 'by' search criteria.
	QString by = ui->worksFilterByComboBox->currentData().toString();
	QVector<Work> found_works;

	if (by == "Name") {
		found_works = DatabaseManager::search_works_by_name(text,
															ui->worksFilterStatusComboBox->currentData().toString(),
															ui->worksFilterTypeComboBox->currentData().toString());
	}

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
	//Clear list items.
	ui->worksAuthorListWidget->clear();
	ui->worksArtistListWidget->clear();


	auto selected_items = ui->worksTableWidget->selectedItems();

	if (!selected_items.isEmpty()) {
		//Enable input widgets when an item is selected.
		ui->worksNameLineEdit->setEnabled(true);
		ui->worksStatusComboBox->setEnabled(true);
		ui->worksTypeComboBox->setEnabled(true);
		ui->worksGroupingLineEdit->setEnabled(true);
		ui->worksChapterLineEdit->setEnabled(true);

		ui->worksAuthorAddButton->setEnabled(true);
		ui->worksAuthorListWidget->setEnabled(true);
		ui->worksArtistAddButton->setEnabled(true);
		ui->worksArtistListWidget->setEnabled(true);


		const auto work = DatabaseManager::get_work(selected_items.first()->data(Qt::UserRole).toInt());

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

		ui->worksAuthorAddButton->setDisabled(true);
		ui->worksAuthorListWidget->setDisabled(true);
		ui->worksArtistAddButton->setDisabled(true);
		ui->worksArtistListWidget->setDisabled(true);
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

void MainWindow::on_worksAddButton_clicked() {
	AddWorkDialog dialog(this);
	if (dialog.exec() == QDialog::Accepted) {
		emit ui->worksFilterLineEdit->textChanged(ui->worksFilterLineEdit->text());
	}
}

//==================================================================================================================================

void MainWindow::on_worksNameLineEdit_textEdited(const QString& text) {
	DatabaseManager::update_work("name", ui->worksIdLabel->text().toInt(), text);

	//Update the name of the selected work.
	ui->worksTableWidget->selectedItems().first()->setText(text);
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

	//Update the Updated datetime label when we change the chapter.
	emit ui->worksTableWidget->itemSelectionChanged();

	//Update the chapter in the table widget.
	int row = ui->worksTableWidget->selectedItems().first()->row();
	ui->worksTableWidget->item(row, 1)->setText(text);
}

//==================================================================================================================================

void MainWindow::on_worksAuthorAddButton_clicked() {
	AttachCreatorDialog dialog(ui->worksIdLabel->text().toInt(), "Author", this);
	if (dialog.exec() == QDialog::Accepted) {
		emit ui->worksTableWidget->itemSelectionChanged();
	}
}

//==================================================================================================================================

void MainWindow::on_worksArtistAddButton_clicked() {
	AttachCreatorDialog dialog(ui->worksIdLabel->text().toInt(), "Artist", this);
	if (dialog.exec() == QDialog::Accepted) {
		emit ui->worksTableWidget->itemSelectionChanged();
	}
}

//==================================================================================================================================

void MainWindow::on_worksAuthorListWidget_customContextMenuRequested(const QPoint& pos) {
	if (QListWidgetItem* item = ui->worksAuthorListWidget->itemAt(pos); item) {
		if (QVariant data = item->data(Qt::UserRole); data.isValid()) {
			QMenu menu(ui->worksAuthorListWidget);
			menu.addAction("Remove", [&](){
				int result = QMessageBox::warning(this,
												  "Removing Author",
												  QString("Are you sure you want to remove \"%1\" as Author?").arg(item->text()),
												  QMessageBox::Yes,
												  QMessageBox::No);

				if (result == QMessageBox::Yes) {
					DatabaseManager::detach_creator(ui->worksIdLabel->text().toInt(), item->data(Qt::UserRole).toInt(), "Author");
					ui->worksAuthorListWidget->removeItemWidget(item);
					delete item;
				}
			});
			menu.exec(QCursor::pos());
		}
	}
}

//==================================================================================================================================

void MainWindow::on_worksArtistListWidget_customContextMenuRequested(const QPoint& pos) {
	if (QListWidgetItem* item = ui->worksArtistListWidget->itemAt(pos); item) {
		if (QVariant data = item->data(Qt::UserRole); data.isValid()) {
			QMenu menu(ui->worksArtistListWidget);
			menu.addAction("Remove", [&](){
				int result = QMessageBox::warning(this,
												  "Removing Artist",
												  QString("Are you sure you want to remove \"%1\" as Artist?").arg(item->text()),
												  QMessageBox::Yes,
												  QMessageBox::No);

				if (result == QMessageBox::Yes) {
					DatabaseManager::detach_creator(ui->worksIdLabel->text().toInt(), item->data(Qt::UserRole).toInt(), "Artist");
					ui->worksArtistListWidget->removeItemWidget(item);
					delete item;
				}
			});
			menu.exec(QCursor::pos());
		}
	}
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
	//Clear list items.
	ui->creatorsListWidget->clear();

	//Find creators and populate the list.
	QVector<Creator> found_creators = DatabaseManager::search_creators(text);

	for (const auto& creator : found_creators) {
		QListWidgetItem* item = new QListWidgetItem(creator.name);
		item->setData(Qt::UserRole, creator.id);

		ui->creatorsListWidget->addItem(item);
	}

	//Update status bar.
	ui->statusBar->showMessage(QString("Found %1 entries.").arg(found_creators.size()));
}

//==================================================================================================================================

void MainWindow::on_creatorsListWidget_itemSelectionChanged() {
	//Clear list items.
	ui->creatorsWorksListWidget->clear();


	auto selected_items = ui->creatorsListWidget->selectedItems();

	if (!selected_items.isEmpty()) {
		//Enable input widgets when an item is selected.
		ui->creatorsNameLineEdit->setEnabled(true);
		ui->creatorsWorksListWidget->setEnabled(true);


		const auto creator = DatabaseManager::get_creator(selected_items.first()->data(Qt::UserRole).toInt());

		ui->creatorsIdLabel->setText(QString::number(creator.id));
		ui->creatorsNameLineEdit->setText(creator.name);

		for (const auto& work : creator.works) {
			QListWidgetItem* item = new QListWidgetItem(work.name);
			item->setData(Qt::UserRole, work.id);

			ui->creatorsWorksListWidget->addItem(item);
		}
	}
	else {
		ui->creatorsIdLabel->clear();
		ui->creatorsNameLineEdit->clear();

		//Disable input widgets when no item is selected.
		ui->creatorsNameLineEdit->setDisabled(true);
		ui->creatorsWorksListWidget->setDisabled(true);
	}
}

//==================================================================================================================================

void MainWindow::on_creatorsListWidget_customContextMenuRequested(const QPoint& pos) {
	if (QListWidgetItem* item = ui->creatorsListWidget->itemAt(pos); item) {
		if (QVariant data = item->data(Qt::UserRole); data.isValid()) {
			QMenu menu(ui->creatorsListWidget);
			menu.addAction("Remove", [&](){
				int result = QMessageBox::warning(this,
												  "Deleting Entry",
												  QString("Are you sure you want to delete \"%1\"?").arg(item->text()),
												  QMessageBox::Yes,
												  QMessageBox::No);

				if (result == QMessageBox::Yes) {
					DatabaseManager::remove_creator(item->data(Qt::UserRole).toInt());
					ui->creatorsListWidget->removeItemWidget(item);
					delete item;
				}
			});
			menu.exec(QCursor::pos());
		}
	}
}

//==================================================================================================================================

void MainWindow::on_creatorsAddButton_clicked() {
	AddCreatorDialog dialog(this);
	if (dialog.exec() == QDialog::Accepted) {
		emit ui->creatorsFilterLineEdit->textChanged(ui->creatorsFilterLineEdit->text());
	}
}

//==================================================================================================================================

void MainWindow::on_creatorsNameLineEdit_textEdited(const QString& text) {
	DatabaseManager::update_creator("name", ui->creatorsIdLabel->text().toInt(), text);

	//Update the name of the selected work.
	ui->creatorsListWidget->selectedItems().first()->setText(text);
}

//==================================================================================================================================

void MainWindow::on_creatorsWorksListWidget_itemClicked(QListWidgetItem* item) {
	//TO DO: Add me.
}

//==================================================================================================================================
//==================================================================================================================================
//==================================================================================================================================
