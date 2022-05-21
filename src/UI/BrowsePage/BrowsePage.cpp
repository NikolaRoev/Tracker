#include "BrowsePage.h"
#include "ui_BrowsePage.h"
#include "DatabaseManager/DatabaseManager.h"
#include "DatabaseManager/Work.h"
#include "DatabaseManager/Creator.h"
#include "WorkPage/WorkPage.h"
#include "CreatorPage/CreatorPage.h"

#include <QWidget>
#include <QString>
#include <QModelIndex>
#include <QPoint>
#include <QHeaderView>
#include <QTableWidgetItem>
#include <QVariant>
#include <QMenu>
#include <QMessageBox>
#include <QCursor>

//==================================================================================================================================
//==================================================================================================================================
//==================================================================================================================================

BrowsePage::BrowsePage(QWidget* parent) : QWidget(parent), ui(new Ui::BrowsePage) {
	ui->setupUi(this);


	//Set resize mode for the Table Widget.
	ui->tableWidget->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
	ui->tableWidget->horizontalHeader()->setDefaultAlignment(Qt::AlignLeft);
	ui->tableWidget->verticalHeader()->setSectionResizeMode(QHeaderView::ResizeToContents);

	//Set the various Combo Box Items Data.
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

//==================================================================================================================================

BrowsePage::~BrowsePage() {
	delete ui;
}

//==================================================================================================================================
//==================================================================================================================================

void BrowsePage::populate(const QString& search) {
	//Clear list items.
	ui->tableWidget->setRowCount(0);

	if (ui->whatComboBox->currentIndex() == 0) {
		QVector<Work> found_works = DatabaseManager::search_works(search,
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

		//Update the Status Bar.
		emit message(QString("Found %1 works.").arg(found_works.size()));
	}
	else {
		QVector<Creator> found_creators = DatabaseManager::search_creators(search);

		for (const auto& creator : found_creators) {
			ui->tableWidget->insertRow(ui->tableWidget->rowCount());

			QTableWidgetItem* name_item = new QTableWidgetItem(creator.name);
			name_item->setData(Qt::UserRole, creator.id);

			ui->tableWidget->setItem(ui->tableWidget->rowCount() - 1, 0, name_item);
		}

		//Update the Status Bar.
		emit message(QString("Found %1 creators.").arg(found_creators.size()));
	}
}

//==================================================================================================================================
//==================================================================================================================================
//==================================================================================================================================

void BrowsePage::on_backToolButton_clicked() {
	int previous = ui->stackedWidget->currentIndex() - 1;

	if (previous >= 0) {
		ui->stackedWidget->setCurrentIndex(previous);
	}
}

//==================================================================================================================================

void BrowsePage::on_forwardToolButton_clicked() {
	int current = ui->stackedWidget->currentIndex();
	int last = ui->stackedWidget->count() - 1;

	if (current < last) {
		ui->stackedWidget->setCurrentIndex(current + 1);
	}
}

//==================================================================================================================================

void BrowsePage::on_homeToolButton_clicked() {
	ui->stackedWidget->setCurrentIndex(0);
}

//==================================================================================================================================
//==================================================================================================================================

void BrowsePage::on_stackedWidget_currentChanged(int index) {
	if (index == 0) {
		ui->backToolButton->setDisabled(true);
		ui->homeToolButton->setDisabled(true);
	}
	else {
		ui->backToolButton->setEnabled(true);
		ui->homeToolButton->setEnabled(true);
	}

	int last = ui->stackedWidget->count() - 1;
	if (index == last) {
		ui->forwardToolButton->setDisabled(true);
	}
	else {
		ui->forwardToolButton->setEnabled(true);
	}
}

//==================================================================================================================================

void BrowsePage::on_lineEdit_textEdited(const QString& text) {
	populate(text);
}

//==================================================================================================================================

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
			ui->tableWidget->setColumnCount(2);
			ui->tableWidget->setHorizontalHeaderLabels({ "Name", "No. of Works" });

			ui->statusComboBox->setDisabled(true);
			ui->typeComboBox->setDisabled(true);
			ui->byComboBox->setDisabled(true);
			break;
	}

	populate(ui->lineEdit->text());
}

//==================================================================================================================================

void BrowsePage::on_statusComboBox_currentIndexChanged(int index) {
	populate(ui->lineEdit->text());
}

//==================================================================================================================================

void BrowsePage::on_typeComboBox_currentIndexChanged(int index) {
	populate(ui->lineEdit->text());
}

//==================================================================================================================================

void BrowsePage::on_byComboBox_currentIndexChanged(int index) {
	populate(ui->lineEdit->text());
}

//==================================================================================================================================

void BrowsePage::on_tableWidget_clicked(const QModelIndex& index) {
	if (QVariant data = ui->tableWidget->item(index.row(), 0)->data(Qt::UserRole); data.isValid()) {
		if (ui->whatComboBox->currentIndex() == 0) {
			on_workClicked(data.toInt());
		}
		else {
			on_creatorClicked(data.toInt());
		}
	}
}

//==================================================================================================================================

void BrowsePage::on_tableWidget_customContextMenuRequested(const QPoint& pos) {
	if (QTableWidgetItem* item = ui->tableWidget->itemAt(pos); item) {
		if (QVariant data = item->data(Qt::UserRole); data.isValid()) {
			QMenu menu(ui->tableWidget);
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

					ui->tableWidget->removeRow(item->row());
				}
			});
			menu.exec(QCursor::pos());
		}
	}
}

//==================================================================================================================================
//==================================================================================================================================

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

	ui->stackedWidget->addWidget(work_page);
	ui->stackedWidget->setCurrentIndex(ui->stackedWidget->count() - 1);
}

//==================================================================================================================================

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

	ui->stackedWidget->addWidget(creator_page);
	ui->stackedWidget->setCurrentIndex(ui->stackedWidget->count() - 1);
}

//==================================================================================================================================
//==================================================================================================================================
//==================================================================================================================================
