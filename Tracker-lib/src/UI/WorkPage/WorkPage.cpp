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

bool WorkPage::pending_change() {
	if (work.name != ui->nameLineEdit->text()) {
		return true;
	}
	if (work.status != ui->statusComboBox->currentData().toString()) {
		return true;
	}
	if (work.type != ui->typeComboBox->currentData().toString()) {
		return true;
	}
	if (work.chapter != ui->chapterLineEdit->text()) {
		return true;
	}
	if (work.md_id != ui->mdLineEdit->text()) {
		return true;
	}
	if (work.mu_id != ui->muLineEdit->text()) {
		return true;
	}

	return false;
}

//==================================================================================================================================
//==================================================================================================================================

WorkPage::WorkPage(const int id, QWidget* parent) : QWidget(parent), ui(new Ui::WorkPage) {
	ui->setupUi(this);
	ui->tableWidget->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
	ui->tableWidget->horizontalHeader()->setDefaultAlignment(Qt::AlignLeft);
	ui->tableWidget->verticalHeader()->setSectionResizeMode(QHeaderView::ResizeToContents);

	ui->statusComboBox->setItemData(0, "Reading");
	ui->statusComboBox->setItemData(1, "Completed");

	ui->typeComboBox->setItemData(0, "Series");
	ui->typeComboBox->setItemData(1, "One Shot");
	ui->typeComboBox->setItemData(2, "Anthology");


	DatabaseManager::get_work(work, id);
	ui->idLabel->setText(QString::number(work.id));
	ui->nameLineEdit->setText(work.name);
	ui->statusComboBox->setCurrentText(work.status);
	ui->typeComboBox->setCurrentText(work.type);
	ui->chapterLineEdit->setText(work.chapter);
	ui->updatedLabel->setText(work.updated);
	ui->addedLabel->setText(work.added);
	ui->mdLineEdit->setText(work.md_id);
	ui->muLineEdit->setText(work.mu_id);

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

void WorkPage::on_nameLineEdit_textEdited(const QString&) {
	ui->applyButton->setEnabled(pending_change());
}

//==================================================================================================================================

void WorkPage::on_statusComboBox_currentIndexChanged(int) {
	ui->applyButton->setEnabled(pending_change());
}

//==================================================================================================================================

void WorkPage::on_typeComboBox_currentIndexChanged(int) {
	ui->applyButton->setEnabled(pending_change());
}

//==================================================================================================================================

void WorkPage::on_chapterLineEdit_textEdited(const QString&) {
	ui->applyButton->setEnabled(pending_change());
}

//==================================================================================================================================

void WorkPage::on_mdLineEdit_textEdited(const QString&) {
	ui->applyButton->setEnabled(pending_change());
}

//==================================================================================================================================

void WorkPage::on_muLineEdit_textEdited(const QString&) {
	ui->applyButton->setEnabled(pending_change());
}

//==================================================================================================================================
//==================================================================================================================================

void WorkPage::on_applyButton_clicked() {
	ui->applyButton->setDisabled(true);

	//Name.
	QString name = ui->nameLineEdit->text();
	if (work.name != name) {
		if (QString error = DatabaseManager::update_work("name", work.id, name); error.isNull()) {
			work.name = name;
		}
		else {
			QMessageBox::warning(this, "Failed to update Work name.", error);
		}
	}

	//Status.
	QString status = ui->statusComboBox->currentData().toString();
	if (work.status != status) {
		if (QString error = DatabaseManager::update_work("status", work.id, status); error.isNull()) {
			work.status = status;
		}
		else {
			QMessageBox::warning(this, "Failed to update Work status.", error);
		}
	}

	//Type.
	QString type = ui->typeComboBox->currentData().toString();
	if (work.type != type) {
		if (QString error = DatabaseManager::update_work("type", work.id, type); error.isNull()) {
			work.type = type;
		}
		else {
			QMessageBox::warning(this, "Failed to update Work type.", error);
		}
	}

	//Chapter.
	QString chapter = ui->chapterLineEdit->text();
	if (work.chapter != chapter) {
		if (QString error = DatabaseManager::update_work("chapter", work.id, chapter); error.isNull()) {
			work.chapter = chapter;

			QString date_time = QDateTime::currentDateTime().toString("yyyy-MM-dd hh:mm:ss");
			if (error = DatabaseManager::update_work("updated", work.id, date_time); error.isNull()) {
				work.updated = date_time;
				ui->updatedLabel->setText(date_time);
			}
			else {
				QMessageBox::warning(this, "Failed to update Work datetime updated.", error);
			}
		}
		else {
			QMessageBox::warning(this, "Failed to update Work chapter.", error);
		}
	}

	//MangaDex ID.
	QString md_id = ui->mdLineEdit->text();
	if (work.md_id != md_id) {
		if (QString error = DatabaseManager::update_work("md_id", work.id, md_id); error.isNull()) {
		}
		else {
			QMessageBox::warning(this, "Failed to update Work MangaDex ID.", error);
		}
	}

	//MangaUpdates ID.
	QString mu_id = ui->muLineEdit->text();
	if (work.mu_id != mu_id) {
		if (QString error = DatabaseManager::update_work("mu_id", work.id, mu_id); error.isNull()) {
		}
		else {
			QMessageBox::warning(this, "Failed to update Work MangaUpdates ID.", error);
		}
	}
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
					DatabaseManager::detach_creator(work.id, data.toInt());
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
	DatabaseManager::attach_creator(work.id, creator_id, type);

	ui->tableWidget->setRowCount(0);

	DatabaseManager::get_work(work, work.id);
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
