#include "pch.h"
#include "WorkPage.h"
#include "./ui_WorkPage.h"
#include "DatabaseManager/DatabaseManager.h"
#include "DatabaseManager/Work.h"
#include "DatabaseManager/Creator.h"
#include "SearchCreatorDialog/SearchCreatorDialog.h"

//==================================================================================================================================
//==================================================================================================================================
//==================================================================================================================================

WorkPage::WorkPage(const int id, QWidget* parent) : QWidget(parent), ui(new Ui::WorkPage), id(id) {
	ui->setupUi(this);
	ui->tableWidget->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
	ui->tableWidget->horizontalHeader()->setDefaultAlignment(Qt::AlignLeft);
	ui->tableWidget->verticalHeader()->setSectionResizeMode(QHeaderView::ResizeToContents);

	ui->statusComboBox->setItemData(0, "Reading");
	ui->statusComboBox->setItemData(1, "Completed");

	ui->typeComboBox->setItemData(0, "Series");
	ui->typeComboBox->setItemData(1, "One Shot");
	ui->typeComboBox->setItemData(2, "Anthology");


	Work work;
	DatabaseManager::get_work(work, id);
	ui->idLabel->setText(QString::number(work.id));
	ui->nameLineEdit->setText(work.name);
	ui->statusComboBox->setCurrentText(work.status);
	ui->typeComboBox->setCurrentText(work.type);
	ui->chapterLineEdit->setText(work.chapter);
	ui->updatedLabel->setText(work.updated);
	ui->addedLabel->setText(work.added);

	for (const auto& creator : work.creators) {
		ui->tableWidget->insertRow(ui->tableWidget->rowCount());

		QTableWidgetItem* name_item = new QTableWidgetItem(creator.name);
		name_item->setData(Qt::UserRole, creator.id);
		ui->tableWidget->setItem(ui->tableWidget->rowCount() - 1, 0, name_item);
		ui->tableWidget->setItem(ui->tableWidget->rowCount() - 1, 1, new QTableWidgetItem(creator.type));
	}
}

//==================================================================================================================================

WorkPage::~WorkPage() {
	delete ui;
}

//==================================================================================================================================
//==================================================================================================================================
//==================================================================================================================================

void WorkPage::on_nameLineEdit_textEdited(const QString& text) {
	DatabaseManager::update_work("name", id, text);
}

//==================================================================================================================================

void WorkPage::on_statusComboBox_currentIndexChanged(int index) {
	Q_UNUSED(index);
	DatabaseManager::update_work("status", id, ui->statusComboBox->currentData().toString());
}

//==================================================================================================================================

void WorkPage::on_typeComboBox_currentIndexChanged(int index) {
	Q_UNUSED(index);
	DatabaseManager::update_work("type", id, ui->typeComboBox->currentData().toString());
}

//==================================================================================================================================

void WorkPage::on_chapterLineEdit_textEdited(const QString& text) {
	DatabaseManager::update_work("chapter", id, text);

	QString date_time = QDateTime::currentDateTime().toString("yyyy-MM-dd hh:mm:ss");
	DatabaseManager::update_work("updated", id, date_time);
	ui->updatedLabel->setText(date_time);
}

//==================================================================================================================================
//==================================================================================================================================

void WorkPage::on_addPushButton_clicked() {
	SearchCreatorDialog* dialog = new SearchCreatorDialog(this);
	connect(dialog, &SearchCreatorDialog::creatorSelected, this, &WorkPage::on_creatorSelected);
	dialog->exec();
}

//==================================================================================================================================

void WorkPage::on_tableWidget_clicked(const QModelIndex& index) {
	emit creatorClicked(ui->tableWidget->item(index.row(), 0)->data(Qt::UserRole).toInt());
}

//==================================================================================================================================

void WorkPage::on_tableWidget_customContextMenuRequested(const QPoint& pos) {
	if (QTableWidgetItem* item = ui->tableWidget->itemAt(pos); item) {
		if (QVariant data = item->data(Qt::UserRole); data.isValid()) {
			QMenu menu(ui->tableWidget);
			menu.addAction("Remove", [&](){
				int result = QMessageBox::warning(this,
												  "Removing",
												  QString("Are you sure you want to remove \"%1\" as Creator?").arg(item->text()),
												  QMessageBox::Yes,
												  QMessageBox::No);

				if (result == QMessageBox::Yes) {
					DatabaseManager::detach_creator(id, data.toInt());
					ui->tableWidget->removeRow(item->row());
				}
			});
			menu.exec(QCursor::pos());
		}
	}
}

//==================================================================================================================================
//==================================================================================================================================

void WorkPage::on_creatorSelected(const int creator_id, const QString& name, const QString& type) {
	DatabaseManager::attach_creator(id, creator_id, type);

	ui->tableWidget->setRowCount(0);

	Work work;
	DatabaseManager::get_work(work, id);
	for (const auto& creator : work.creators) {
		ui->tableWidget->insertRow(ui->tableWidget->rowCount());

		QTableWidgetItem* name_item = new QTableWidgetItem(creator.name);
		name_item->setData(Qt::UserRole, creator.id);

		ui->tableWidget->setItem(ui->tableWidget->rowCount() - 1, 0, name_item);
		ui->tableWidget->setItem(ui->tableWidget->rowCount() - 1, 1, new QTableWidgetItem(creator.type));
	}
}

//==================================================================================================================================
//==================================================================================================================================
//==================================================================================================================================
