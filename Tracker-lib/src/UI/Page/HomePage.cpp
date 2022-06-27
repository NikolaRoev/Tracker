#include "HomePage.h"
#include "ui_HomePage.h"
#include "Page.h"
#include "DatabaseManager/DatabaseManager.h"
#include "DatabaseManager/Work.h"
#include "DatabaseManager/Creator.h"

HomePage::HomePage(QWidget* parent) : Page(parent), ui(new Ui::HomePage) {
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

HomePage::~HomePage() {
	delete ui;
}

void HomePage::populate() {
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

void HomePage::on_lineEdit_textEdited(const QString&) {
	populate();
}

void HomePage::on_whatComboBox_currentIndexChanged(int index) {
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

void HomePage::on_statusComboBox_currentIndexChanged(int) {
	populate();
}

void HomePage::on_typeComboBox_currentIndexChanged(int) {
	populate();
}

void HomePage::on_byComboBox_currentIndexChanged(int) {
	populate();
}

void HomePage::on_tableWidget_clicked(const QModelIndex& index) {
	if (QVariant user_data = ui->tableWidget->item(index.row(), 0)->data(Qt::UserRole); user_data.isValid()) {
		if (ui->whatComboBox->currentIndex() == 0) {
			emit workClicked(user_data.toInt());
		}
		else {
			emit creatorClicked(user_data.toInt());
		}
	}
}

void HomePage::on_tableWidget_customContextMenuRequested(const QPoint& pos) {
	if (int row = ui->tableWidget->rowAt(pos.y()); row != -1) {
		QTableWidgetItem* item = ui->tableWidget->item(row, 0);
		if (QVariant user_data = item->data(Qt::UserRole); user_data.isValid()) {
			QMenu menu(ui->tableWidget);
			menu.addAction("Remove", [&](){
				int result = QMessageBox::question(this,
												  "Removing",
												  QString("Are you sure you want to remove \"%1\"?").arg(item->text()));

				if (result == QMessageBox::Yes) {
					if (ui->whatComboBox->currentIndex() == 0) {
						emit workRemoved(user_data.toInt());
					}
					else {
						emit creatorRemoved(user_data.toInt());
					}
				}
			});
			menu.exec(QCursor::pos());
		}
	}
}
