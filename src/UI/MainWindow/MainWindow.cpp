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



	//Customization.
	//Align the update layout to top so the populated entries look nice.
	ui->contentsWidget->layout()->setAlignment(Qt::AlignTop);

	//Set default focus to the search bar.
	//As 'tabWidget' is first in the tab order it gets focused when the application starts.
	ui->centralwidget->setFocusProxy(ui->updateLineEdit);

	//Add a shortcut that selects all text in the search bar and focuses it.
	QShortcut* shortcut = new QShortcut(QKeySequence(Qt::CTRL | Qt::Key_F), this);
	connect(shortcut, &QShortcut::activated, ui->updateLineEdit, [&](){ ui->updateLineEdit->selectAll(); ui->updateLineEdit->setFocus(); });

	//Set resize mode for the Browse Table Widget.
	ui->browseTableWidget->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
	ui->browseTableWidget->horizontalHeader()->setDefaultAlignment(Qt::AlignLeft);
	ui->browseTableWidget->verticalHeader()->setSectionResizeMode(QHeaderView::ResizeToContents);
	//
	//Customization END.



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



	//Load settings.
	QSettings settings("settings.ini", QSettings::IniFormat, this);
	restoreGeometry(settings.value("geometry").toByteArray());
	restoreState(settings.value("state").toByteArray());



	//Populate the update entries when the program starts initially.
	ui->listWidget->setCurrentRow(0);
}

//==================================================================================================================================

MainWindow::~MainWindow() {
	QSettings settings("settings.ini", QSettings::IniFormat, this);
	settings.setValue("geometry", saveGeometry());
	settings.setValue("state", saveState());

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
	dialog->exec();
}

//==================================================================================================================================

void MainWindow::on_actionAdd_Creator_triggered() {
	AddCreatorDialog* dialog = new AddCreatorDialog(this);
	dialog->exec();
}

//==================================================================================================================================
//==================================================================================================================================

void MainWindow::on_actionBack_triggered() {
	int previous = ui->browseStackedWidget->currentIndex() - 1;

	if (previous >= 0) {
		ui->browseStackedWidget->setCurrentIndex(previous);
	}
}

//==================================================================================================================================

void MainWindow::on_actionForward_triggered() {
	int current = ui->browseStackedWidget->currentIndex();
	int last = ui->browseStackedWidget->count() - 1;

	if (current < last) {
		ui->browseStackedWidget->setCurrentIndex(current + 1);
	}
}

//==================================================================================================================================

void MainWindow::on_actionHome_triggered() {
	ui->browseStackedWidget->setCurrentIndex(0);
}

//==================================================================================================================================
//==================================================================================================================================

void MainWindow::on_listWidget_currentRowChanged(int currentRow) {
	ui->mainStackedWidget->setCurrentIndex(currentRow);

	switch (currentRow) {
		case 0: emit ui->updateLineEdit->textEdited(ui->updateLineEdit->text()); break;
		case 1: emit ui->whatComboBox->currentIndexChanged(ui->whatComboBox->currentIndex()); break;
	}
}

//==================================================================================================================================
//==================================================================================================================================

void MainWindow::on_updateLineEdit_textEdited(const QString& text) {
	//Clear the current update entries.
	QLayoutItem* child{ nullptr };
	while ((child = ui->contentsWidget->layout()->takeAt(0)) != nullptr) {
		delete child->widget();
		delete child;
	}

	//Find works and populate the update list.
	const auto found_works = DatabaseManager::search_works(text, "name", "Reading", NULL);
	for (const auto& found_work : found_works) {
		ui->contentsWidget->layout()->addWidget(new UpdateEntry(found_work, ui->scrollArea));
	}

	//Update status bar.
	ui->statusBar->showMessage(QString("Found %1 entries.").arg(found_works.size()));
}

//==================================================================================================================================
//==================================================================================================================================

void MainWindow::on_browseStackedWidget_currentChanged(int index) {
	if (index == 0) {
		ui->actionBack->setDisabled(true);
		ui->actionHome->setDisabled(true);
	}
	else {
		ui->actionBack->setEnabled(true);
		ui->actionHome->setEnabled(true);
	}

	int last = ui->browseStackedWidget->count() - 1;
	if (index == last) {
		ui->actionForward->setDisabled(true);
	}
	else {
		ui->actionForward->setEnabled(true);
	}
}

//==================================================================================================================================

void MainWindow::on_browseLineEdit_textEdited(const QString& text) {
	//Clear list items.
	ui->browseTableWidget->setRowCount(0);

	if (ui->whatComboBox->currentIndex() == 0) {
		QVector<Work> found_works = DatabaseManager::search_works(text,
																  ui->byComboBox->currentData().toString(),
																  ui->statusComboBox->currentData().toString(),
																  ui->typeComboBox->currentData().toString());

		for (const auto& work : found_works) {
			ui->browseTableWidget->insertRow(ui->browseTableWidget->rowCount());

			QTableWidgetItem* name_item = new QTableWidgetItem(work.name);
			name_item->setData(Qt::UserRole, work.id);

			ui->browseTableWidget->setItem(ui->browseTableWidget->rowCount() - 1, 0, name_item);
			ui->browseTableWidget->setItem(ui->browseTableWidget->rowCount() - 1, 1, new QTableWidgetItem(work.chapter));
			ui->browseTableWidget->setItem(ui->browseTableWidget->rowCount() - 1, 2, new QTableWidgetItem(work.updated));
			ui->browseTableWidget->setItem(ui->browseTableWidget->rowCount() - 1, 3, new QTableWidgetItem(work.added));
		}

		//Update status bar.
		ui->statusBar->showMessage(QString("Found %1 works.").arg(found_works.size()));
	}
	else {
		QVector<Creator> found_creators = DatabaseManager::search_creators(text);

		for (const auto& creator : found_creators) {
			ui->browseTableWidget->insertRow(ui->browseTableWidget->rowCount());

			QTableWidgetItem* name_item = new QTableWidgetItem(creator.name);
			name_item->setData(Qt::UserRole, creator.id);

			ui->browseTableWidget->setItem(ui->browseTableWidget->rowCount() - 1, 0, name_item);
		}

		//Update status bar.
		ui->statusBar->showMessage(QString("Found %1 creators.").arg(found_creators.size()));
	}
}

//==================================================================================================================================

void MainWindow::on_whatComboBox_currentIndexChanged(int index) {
	switch(index) {
		case 0:
			ui->browseTableWidget->setColumnCount(4);
			ui->browseTableWidget->setHorizontalHeaderLabels({ "Name", "Chapter", "Updated", "Added" });

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

	emit ui->browseLineEdit->textEdited(ui->browseLineEdit->text());
}

//==================================================================================================================================

void MainWindow::on_statusComboBox_currentIndexChanged(int index) {
	emit ui->browseLineEdit->textEdited(ui->browseLineEdit->text());
}

//==================================================================================================================================

void MainWindow::on_typeComboBox_currentIndexChanged(int index) {
	emit ui->browseLineEdit->textEdited(ui->browseLineEdit->text());
}

//==================================================================================================================================

void MainWindow::on_byComboBox_currentIndexChanged(int index) {
	emit ui->browseLineEdit->textEdited(ui->browseLineEdit->text());
}

//==================================================================================================================================

void MainWindow::on_browseTableWidget_clicked(const QModelIndex& index) {
	if (QVariant data = ui->browseTableWidget->item(index.row(), 0)->data(Qt::UserRole); data.isValid()) {
		if (ui->whatComboBox->currentIndex() == 0) {
			on_workClicked(data.toInt());
		}
		else {
			on_creatorClicked(data.toInt());
		}
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

void MainWindow::on_workClicked(const int id) {
	int last = ui->browseStackedWidget->count() - 1;
	int current = ui->browseStackedWidget->currentIndex();

	if (current < last) {
		for (int i = last; i > current; --i) {
			QWidget* widget = ui->browseStackedWidget->widget(i);
			ui->browseStackedWidget->removeWidget(widget);
			delete widget;
		}
	}


	WorkPage* work_page = new WorkPage(id);
	connect(work_page, &WorkPage::creatorClicked, this, &MainWindow::on_creatorClicked);

	ui->browseStackedWidget->addWidget(work_page);
	ui->browseStackedWidget->setCurrentIndex(ui->browseStackedWidget->count() - 1);
}

//==================================================================================================================================

void MainWindow::on_creatorClicked(const int id) {
	int last = ui->browseStackedWidget->count() - 1;
	int current = ui->browseStackedWidget->currentIndex();

	if (current < last) {
		for (int i = last; i > current; --i) {
			QWidget* widget = ui->browseStackedWidget->widget(i);
			ui->browseStackedWidget->removeWidget(widget);
			delete widget;
		}
	}


	CreatorPage* creator_page = new CreatorPage(id);
	connect(creator_page, &CreatorPage::workClicked, this, &MainWindow::on_workClicked);

	ui->browseStackedWidget->addWidget(creator_page);
	ui->browseStackedWidget->setCurrentIndex(ui->browseStackedWidget->count() - 1);
}

//==================================================================================================================================
//==================================================================================================================================
//==================================================================================================================================
