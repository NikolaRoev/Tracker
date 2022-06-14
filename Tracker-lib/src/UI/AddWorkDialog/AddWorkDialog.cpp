#include "pch.h"
#include "AddWorkDialog.h"
#include "./ui_AddWorkDialog.h"
#include "DatabaseManager/DatabaseManager.h"

//==================================================================================================================================
//==================================================================================================================================

AddWorkDialog::AddWorkDialog(QWidget* parent) : QDialog(parent), ui(new Ui::AddWorkDialog) {
	ui->setupUi(this);

	ui->statusComboBox->setItemData(0, "Reading");
	ui->statusComboBox->setItemData(1, "Completed");

	ui->typeComboBox->setItemData(0, "Series");
	ui->typeComboBox->setItemData(1, "One Shot");
	ui->typeComboBox->setItemData(2, "Anthology");
}

//==================================================================================================================================

AddWorkDialog::~AddWorkDialog() {
	delete ui;
}

//==================================================================================================================================
//==================================================================================================================================

void AddWorkDialog::on_addButton_clicked() {
	Work work;
	work.name = ui->nameLineEdit->text();
	work.status = ui->statusComboBox->currentData().toString();
	work.type = ui->typeComboBox->currentData().toString();
	work.chapter = ui->chapterLineEdit->text();

	QString date_time = QDateTime::currentDateTime().toString("yyyy-MM-dd hh:mm:ss");
	work.updated = date_time;
	work.added = date_time;

	if (QString error = DatabaseManager::add_work(work); error.isNull()) {
		accept();
	}
	else {
		qWarning() << error;
		QMessageBox::warning(this, "Failed to add Work.", error);
	}
}

//==================================================================================================================================
//==================================================================================================================================
