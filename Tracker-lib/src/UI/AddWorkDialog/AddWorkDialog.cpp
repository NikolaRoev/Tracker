#include "pch.h"
#include "AddWorkDialog.h"
#include "./ui_AddWorkDialog.h"
#include "DatabaseManager/Work.h"

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

void AddWorkDialog::on_buttonBox_accepted() {
	Work work;
	work.name = ui->nameLineEdit->text();
	work.status = ui->statusComboBox->currentData().toString();
	work.type = ui->typeComboBox->currentData().toString();
	work.chapter = ui->chapterLineEdit->text();

	QString date_time = util::current_datetime();
	work.updated = date_time;
	work.added = date_time;

	emit workAdded(work);
}

//==================================================================================================================================
//==================================================================================================================================

