#include "WorkPage.h"
#include "./ui_WorkPage.h"
#include "DatabaseManager/DatabaseManager.h"
#include "DatabaseManager/Work.h"
#include "DatabaseManager/Creator.h"
#include "SearchCreatorDialog/SearchCreatorDialog.h"

#include <QWidget>
#include <QListWidgetItem>
#include <QDateTime>
#include <QMenu>
#include <QMessageBox>

//==================================================================================================================================
//==================================================================================================================================
//==================================================================================================================================

WorkPage::WorkPage(const int id, QWidget* parent) : id(id), QWidget(parent), ui(new Ui::WorkPage) {
	ui->setupUi(this);
	ui->statusComboBox->setItemData(0, "Reading");
	ui->statusComboBox->setItemData(1, "Completed");

	ui->typeComboBox->setItemData(0, "Series");
	ui->typeComboBox->setItemData(1, "One Shot");
	ui->typeComboBox->setItemData(2, "Anthology");


	Work work = DatabaseManager::get_work(id);
	ui->idLabel->setText(QString::number(work.id));
	ui->nameLineEdit->setText(work.name);
	ui->statusComboBox->setCurrentText(work.status);
	ui->typeComboBox->setCurrentText(work.type);
	ui->groupingLineEdit->setText(work.grouping);
	ui->chapterLineEdit->setText(work.chapter);
	ui->updatedLabel->setText(work.updated);
	ui->addedLabel->setText(work.added);

	for (const auto& creator : work.creators) {
		QListWidgetItem* item = new QListWidgetItem(creator.name);
		item->setData(Qt::UserRole, creator.id);

		if (creator.type == "Author") {
			ui->authorListWidget->addItem(item);
		}
		else if (creator.type == "Artist") {
			ui->artistListWidget->addItem(item);
		}
	}
}

//==================================================================================================================================

WorkPage::~WorkPage() {
	delete ui;
}

//==================================================================================================================================
//==================================================================================================================================
//==================================================================================================================================

void WorkPage::on_nameLineEdit_textChanged(const QString& text) {
	DatabaseManager::update_work("name", id, text);
}

//==================================================================================================================================

void WorkPage::on_statusComboBox_currentIndexChanged(int index) {
	DatabaseManager::update_work("status", id, ui->statusComboBox->currentData().toString());
}

//==================================================================================================================================

void WorkPage::on_typeComboBox_currentIndexChanged(int index) {
	DatabaseManager::update_work("type", id, ui->typeComboBox->currentData().toString());
}

//==================================================================================================================================

void WorkPage::on_groupingLineEdit_textChanged(const QString& text) {
	DatabaseManager::update_work("grouping", id, text);
}

//==================================================================================================================================

void WorkPage::on_chapterLineEdit_textChanged(const QString& text) {
	DatabaseManager::update_work("chapter", id, text);

	QString date_time = QDateTime::currentDateTime().toString("yyyy-MM-dd hh:mm:ss");
	DatabaseManager::update_work("updated", id, date_time);
	ui->updatedLabel->setText(date_time);
}

//==================================================================================================================================
//==================================================================================================================================

void WorkPage::on_authorPushButton_clicked() {
	SearchCreatorDialog* dialog = new SearchCreatorDialog(this);
	connect(dialog, &SearchCreatorDialog::creatorSelected, this, &WorkPage::on_authorSelected);
	dialog->show();
}

//==================================================================================================================================

void WorkPage::on_artistPushButton_clicked() {
	SearchCreatorDialog* dialog = new SearchCreatorDialog(this);
	connect(dialog, &SearchCreatorDialog::creatorSelected, this, &WorkPage::on_artistSelected);
	dialog->show();
}

//==================================================================================================================================

void WorkPage::on_authorListWidget_itemClicked(QListWidgetItem* item) {
	emit creatorClicked(item->data(Qt::UserRole).toInt());
}

//==================================================================================================================================

void WorkPage::on_artistListWidget_itemClicked(QListWidgetItem* item) {
	emit creatorClicked(item->data(Qt::UserRole).toInt());
}

//==================================================================================================================================

void WorkPage::on_authorListWidget_customContextMenuRequested(const QPoint& pos) {
	if (QListWidgetItem* item = ui->authorListWidget->itemAt(pos); item) {
		if (QVariant data = item->data(Qt::UserRole); data.isValid()) {
			QMenu menu(ui->authorListWidget);
			menu.addAction("Remove", [&](){
				int result = QMessageBox::warning(this,
												  "Removing Author",
												  QString("Are you sure you want to remove \"%1\" as Author?").arg(item->text()),
												  QMessageBox::Yes,
												  QMessageBox::No);

				if (result == QMessageBox::Yes) {
					DatabaseManager::detach_creator(id, data.toInt(), "Author");
					ui->authorListWidget->removeItemWidget(item);
					delete item;
				}
			});
			menu.exec(QCursor::pos());
		}
	}
}

//==================================================================================================================================

void WorkPage::on_artistListWidget_customContextMenuRequested(const QPoint& pos) {
	if (QListWidgetItem* item = ui->artistListWidget->itemAt(pos); item) {
		if (QVariant data = item->data(Qt::UserRole); data.isValid()) {
			QMenu menu(ui->artistListWidget);
			menu.addAction("Remove", [&](){
				int result = QMessageBox::warning(this,
												  "Removing Artist",
												  QString("Are you sure you want to remove \"%1\" as Artist?").arg(item->text()),
												  QMessageBox::Yes,
												  QMessageBox::No);

				if (result == QMessageBox::Yes) {
					DatabaseManager::detach_creator(id, data.toInt(), "Artist");
					ui->artistListWidget->removeItemWidget(item);
					delete item;
				}
			});
			menu.exec(QCursor::pos());
		}
	}
}

//==================================================================================================================================
//==================================================================================================================================

void WorkPage::on_authorSelected(const int creator_id, const QString& name) {
   DatabaseManager::attach_creator(id, creator_id, "Author");

   ui->authorListWidget->clear();
   ui->artistListWidget->clear();

   Work work = DatabaseManager::get_work(id);
   for (const auto& creator : work.creators) {
	   QListWidgetItem* item = new QListWidgetItem(creator.name);
	   item->setData(Qt::UserRole, creator.id);

	   if (creator.type == "Author") {
		   ui->authorListWidget->addItem(item);
	   }
	   else if (creator.type == "Artist") {
		   ui->artistListWidget->addItem(item);
	   }
   }
}

//==================================================================================================================================

void WorkPage::on_artistSelected(const int creator_id, const QString& name) {
   DatabaseManager::attach_creator(id, creator_id, "Artist");

   ui->authorListWidget->clear();
   ui->artistListWidget->clear();

   Work work = DatabaseManager::get_work(id);
   for (const auto& creator : work.creators) {
	   QListWidgetItem* item = new QListWidgetItem(creator.name);
	   item->setData(Qt::UserRole, creator.id);

	   if (creator.type == "Author") {
		   ui->authorListWidget->addItem(item);
	   }
	   else if (creator.type == "Artist") {
		   ui->artistListWidget->addItem(item);
	   }
   }
}

//==================================================================================================================================
//==================================================================================================================================
//==================================================================================================================================
