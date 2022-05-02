#include "AddWorkDialog.h"
#include "./ui_AddWorkDialog.h"
#include "DatabaseManager/DatabaseManager.h"

#include <QDialog>

//==================================================================================================================================
//==================================================================================================================================

AddWorkDialog::AddWorkDialog(QWidget* parent) : QDialog(parent), ui(new Ui::AddWorkDialog) {
	ui->setupUi(this);
	setAttribute(Qt::WA_DeleteOnClose);

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
	DatabaseManager::add_work(ui->nameLineEdit->text(),
							  ui->statusComboBox->currentData().toString(),
							  ui->typeComboBox->currentData().toString(),
							  ui->groupingLineEdit->text(),
							  ui->chapterLineEdit->text());
}

//==================================================================================================================================
//==================================================================================================================================
