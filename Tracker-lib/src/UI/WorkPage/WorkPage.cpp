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
		QListWidgetItem* item = new QListWidgetItem(creator.name);
		item->setData(Qt::UserRole, creator.id);

		if (creator.type == "Author") {
			ui->authorsListWidget->addItem(item);
		}
		else if (creator.type == "Artist") {
			ui->artistsListWidget->addItem(item);
		}
		else {
			qCritical() << QString("Failed to categorize Creator [%1].").arg(creator.type);
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

void WorkPage::on_addAuthorButton_clicked() {
	SearchCreatorDialog* dialog = new SearchCreatorDialog(this);
	connect(dialog, &SearchCreatorDialog::creatorSelected, this, &WorkPage::on_authorSelected);
	dialog->exec();
}

//==================================================================================================================================

void WorkPage::on_addArtistButton_clicked() {
	SearchCreatorDialog* dialog = new SearchCreatorDialog(this);
	connect(dialog, &SearchCreatorDialog::creatorSelected, this, &WorkPage::on_artistSelected);
	dialog->exec();
}

//==================================================================================================================================
//==================================================================================================================================

void WorkPage::on_authorsListWidget_itemClicked(QListWidgetItem* item) {
	emit creatorClicked(item->data(Qt::UserRole).toInt());
}

//==================================================================================================================================

void WorkPage::on_artistsListWidget_itemClicked(QListWidgetItem* item) {
	emit creatorClicked(item->data(Qt::UserRole).toInt());
}

//==================================================================================================================================
//==================================================================================================================================

void WorkPage::on_authorsListWidget_customContextMenuRequested(const QPoint& pos) {
	if (QListWidgetItem* item = ui->authorsListWidget->itemAt(pos); item) {
		if (QVariant user_data = item->data(Qt::UserRole); user_data.isValid()) {
			QMenu menu(ui->authorsListWidget);
			menu.addAction("Remove", [&](){
				int result = QMessageBox::warning(this,
												  "Removing",
												  QString("Are you sure you want to remove \"%1\" as Author?").arg(item->text()),
												  QMessageBox::Yes,
												  QMessageBox::No);

				if (result == QMessageBox::Yes) {
					DatabaseManager::detach_creator(id, user_data.toInt(), "Author");
					delete item;
				}
			});
			menu.exec(QCursor::pos());
		}
	}
}

//==================================================================================================================================

void WorkPage::on_artistsListWidget_customContextMenuRequested(const QPoint& pos) {
	if (QListWidgetItem* item = ui->artistsListWidget->itemAt(pos); item) {
		if (QVariant user_data = item->data(Qt::UserRole); user_data.isValid()) {
			QMenu menu(ui->artistsListWidget);
			menu.addAction("Remove", [&](){
				int result = QMessageBox::warning(this,
												  "Removing",
												  QString("Are you sure you want to remove \"%1\" as Artist?").arg(item->text()),
												  QMessageBox::Yes,
												  QMessageBox::No);

				if (result == QMessageBox::Yes) {
					DatabaseManager::detach_creator(id, user_data.toInt(), "Artist");
					delete item;
				}
			});
			menu.exec(QCursor::pos());
		}
	}
}

//==================================================================================================================================
//==================================================================================================================================

void WorkPage::on_authorSelected(const int author_id, const QString& name) {
	if (QString error = DatabaseManager::attach_creator(id, author_id, "Author"); error.isNull()) {
		QListWidgetItem* item = new QListWidgetItem(name);
		item->setData(Qt::UserRole, author_id);
		ui->authorsListWidget->addItem(item);
	}
	else {
		qWarning() << error;
		QMessageBox::warning(this, "Failed to attach Author.", error);
	}
}

//==================================================================================================================================

void WorkPage::on_artistSelected(const int artist_id, const QString& name) {
	if (QString error = DatabaseManager::attach_creator(id, artist_id, "Artist"); error.isNull()) {
		QListWidgetItem* item = new QListWidgetItem(name);
		item->setData(Qt::UserRole, artist_id);
		ui->artistsListWidget->addItem(item);
	}
	else {
		qWarning() << error;
		QMessageBox::warning(this, "Failed to attach Artist.", error);
	}
}

//==================================================================================================================================
//==================================================================================================================================
//==================================================================================================================================
