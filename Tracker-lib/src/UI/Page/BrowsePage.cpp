#include "pch.h"
#include "BrowsePage.h"
#include "ui_BrowsePage.h"
#include "Page.h"
#include "DatabaseManager/DatabaseManager.h"
#include "DatabaseManager/Work.h"
#include "DatabaseManager/Creator.h"
#include "WorkPage.h"
#include "CreatorPage.h"
#include "AddWorkDialog.h"
#include "AddCreatorDialog.h"

BrowsePage::BrowsePage(QWidget* parent) : Page(parent), ui(new Ui::BrowsePage) {
	ui->setupUi(this);

	ui->tableWidget->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
	ui->tableWidget->horizontalHeader()->setDefaultAlignment(Qt::AlignLeft);
	ui->tableWidget->verticalHeader()->setSectionResizeMode(QHeaderView::ResizeToContents);

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
}

BrowsePage::~BrowsePage() {
	delete ui;
}

void BrowsePage::populate() {
	if (ui->stackedWidget->currentIndex() == 0) {
		ui->tableWidget->setRowCount(0);

		if (ui->whatComboBox->currentIndex() == 0) {
			QList<Work> found_works;
			DatabaseManager::search_works(found_works,
										  ui->lineEdit->text(),
										  ui->byComboBox->currentData().toString(),
										  ui->statusComboBox->currentData().toString(),
										  ui->typeComboBox->currentData().toString());

			for (const auto& work : found_works) {
				ui->tableWidget->insertRow(ui->tableWidget->rowCount());

				QTableWidgetItem* name_item = new QTableWidgetItem(work.name);
				name_item->setData(Qt::UserRole, work.id);

				ui->tableWidget->setItem(ui->tableWidget->rowCount() - 1, 0, name_item);
				ui->tableWidget->setItem(ui->tableWidget->rowCount() - 1, 1, new QTableWidgetItem(work.chapter));
				ui->tableWidget->setItem(ui->tableWidget->rowCount() - 1, 2, new QTableWidgetItem(work.updated));
				ui->tableWidget->setItem(ui->tableWidget->rowCount() - 1, 3, new QTableWidgetItem(work.added));
			}
		}
		else {
			QList<Creator> found_creators;
			DatabaseManager::search_creators(found_creators, ui->lineEdit->text());

			for (const auto& creator : found_creators) {
				ui->tableWidget->insertRow(ui->tableWidget->rowCount());

				QTableWidgetItem* name_item = new QTableWidgetItem(creator.name);
				name_item->setData(Qt::UserRole, creator.id);

				ui->tableWidget->setItem(ui->tableWidget->rowCount() - 1, 0, name_item);
			}
		}
	}
	else {
		dynamic_cast<Page*>(ui->stackedWidget->currentWidget())->populate();
	}
}

void BrowsePage::on_backToolButton_clicked() {
	int previous = ui->stackedWidget->currentIndex() - 1;

	if (previous >= 0) {
		ui->stackedWidget->setCurrentIndex(previous);
	}
}

void BrowsePage::on_forwardToolButton_clicked() {
	int current = ui->stackedWidget->currentIndex();
	int last = ui->stackedWidget->count() - 1;

	if (current < last) {
		ui->stackedWidget->setCurrentIndex(current + 1);
	}
}

void BrowsePage::on_homeToolButton_clicked() {
	ui->stackedWidget->setCurrentIndex(0);
}

void BrowsePage::on_addWorkToolButton_clicked() {
	AddWorkDialog* dialog = new AddWorkDialog(this);
	connect(dialog, &AddWorkDialog::workAdded, this, &BrowsePage::on_workAdded);
	dialog->exec();
}

void BrowsePage::on_addCreatorToolButton_clicked() {
	AddCreatorDialog* dialog = new AddCreatorDialog(this);
	connect(dialog, &AddCreatorDialog::creatorAdded, this, &BrowsePage::on_creatorAdded);
	dialog->exec();
}

void BrowsePage::on_stackedWidget_currentChanged(int index) {
	//Enable the Home and Back buttons if we are not on the Home Page.
	ui->backToolButton->setEnabled(index);
	ui->homeToolButton->setEnabled(index);

	//Enable the forward button if we are not on the last Page.
	int last = ui->stackedWidget->count() - 1;
	ui->forwardToolButton->setEnabled(index != last);

	populate();
}

void BrowsePage::on_lineEdit_textEdited(const QString&) {
	populate();
}

void BrowsePage::on_whatComboBox_currentIndexChanged(int index) {
	switch(index) {
		case 0:
			ui->tableWidget->setColumnCount(4);
			ui->tableWidget->setHorizontalHeaderLabels({ "Name", "Chapter", "Updated", "Added" });

			ui->statusComboBox->setEnabled(true);
			ui->typeComboBox->setEnabled(true);
			ui->byComboBox->setEnabled(true);
			break;
		case 1:
			ui->tableWidget->setColumnCount(1);
			ui->tableWidget->setHorizontalHeaderLabels({ "Name" });

			ui->statusComboBox->setDisabled(true);
			ui->typeComboBox->setDisabled(true);
			ui->byComboBox->setDisabled(true);
			break;
	}

	populate();
}

void BrowsePage::on_statusComboBox_currentIndexChanged(int) {
	populate();
}

void BrowsePage::on_typeComboBox_currentIndexChanged(int) {
	populate();
}

void BrowsePage::on_byComboBox_currentIndexChanged(int) {
	populate();
}

void BrowsePage::on_tableWidget_clicked(const QModelIndex& index) {
	if (QVariant user_data = ui->tableWidget->item(index.row(), 0)->data(Qt::UserRole); user_data.isValid()) {
		if (ui->whatComboBox->currentIndex() == 0) {
			on_workClicked(user_data.toInt());
		}
		else {
			on_creatorClicked(user_data.toInt());
		}
	}
}

void BrowsePage::on_tableWidget_customContextMenuRequested(const QPoint& pos) {
	if (QTableWidgetItem* item = ui->tableWidget->itemAt(pos); item) {
		if (QVariant user_data = item->data(Qt::UserRole); user_data.isValid()) {
			QMenu menu(ui->tableWidget);
			menu.addAction("Remove", [&](){
				int result = QMessageBox::question(this,
												  "Removing",
												  QString("Are you sure you want to remove \"%1\"?").arg(item->text()));

				if (result == QMessageBox::Yes) {
					if (ui->whatComboBox->currentIndex() == 0) {
						on_workRemoved(user_data.toInt());
					}
					else {
						on_creatorRemoved(user_data.toInt());
					}

					ui->tableWidget->removeRow(item->row());
				}
			});
			menu.exec(QCursor::pos());
		}
	}
}

void BrowsePage::on_workClicked(const int id) {
	int last = ui->stackedWidget->count() - 1;
	int current = ui->stackedWidget->currentIndex();

	if (current < last) {
		for (int i = last; i > current; --i) {
			QWidget* widget = ui->stackedWidget->widget(i);
			ui->stackedWidget->removeWidget(widget);
			delete widget;
		}
	}


	WorkPage* work_page = new WorkPage(id);
	connect(work_page, &WorkPage::creatorClicked, this, &BrowsePage::on_creatorClicked);
	connect(work_page, &WorkPage::workRemoved, this, &BrowsePage::on_workRemoved);

	ui->stackedWidget->addWidget(work_page);
	ui->stackedWidget->setCurrentIndex(ui->stackedWidget->count() - 1);
}

void BrowsePage::on_creatorClicked(const int id) {
	int last = ui->stackedWidget->count() - 1;
	int current = ui->stackedWidget->currentIndex();

	if (current < last) {
		for (int i = last; i > current; --i) {
			QWidget* widget = ui->stackedWidget->widget(i);
			ui->stackedWidget->removeWidget(widget);
			delete widget;
		}
	}


	CreatorPage* creator_page = new CreatorPage(id);
	connect(creator_page, &CreatorPage::workClicked, this, &BrowsePage::on_workClicked);
	connect(creator_page, &CreatorPage::creatorRemoved, this, &BrowsePage::on_creatorRemoved);

	ui->stackedWidget->addWidget(creator_page);
	ui->stackedWidget->setCurrentIndex(ui->stackedWidget->count() - 1);
}

void BrowsePage::on_workAdded(const Work& work) {
	if (DatabaseManager::add_work(work)) {
		populate();
	}
	else {
		QMessageBox::warning(this, "Database Error", "Failed to add Work.");
	}
}

void BrowsePage::on_creatorAdded(const Creator& creator) {
	if (DatabaseManager::add_creator(creator)) {
		populate();
	}
	else {
		QMessageBox::warning(this, "Database Error", "Failed to add Creator.");
	}
}

void BrowsePage::on_workRemoved(const int id) {
	if (DatabaseManager::remove_work(id)) {
		while (ui->stackedWidget->count() > 1) {
			QWidget* widget = ui->stackedWidget->widget(ui->stackedWidget->count() - 1);
			ui->stackedWidget->removeWidget(widget);
			delete widget;
		}

		ui->stackedWidget->setCurrentIndex(0);
	}
	else {
		QMessageBox::warning(this, "Database Error", "Failed to remove Work.");
	}
}

void BrowsePage::on_creatorRemoved(const int id) {
	if (DatabaseManager::remove_creator(id)) {
		while (ui->stackedWidget->count() > 1) {
			QWidget* widget = ui->stackedWidget->widget(ui->stackedWidget->count() - 1);
			ui->stackedWidget->removeWidget(widget);
			delete widget;
		}

		ui->stackedWidget->setCurrentIndex(0);
	}
	else {
		QMessageBox::warning(this, "Database Error", "Failed to remove Creator.");
	}
}
