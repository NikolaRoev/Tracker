#include "pch.h"
#include "AddCreatorDialog.h"
#include "./ui_AddCreatorDialog.h"
#include "DatabaseManager/Creator.h"

AddCreatorDialog::AddCreatorDialog(QWidget* parent) : QDialog(parent), ui(new Ui::AddCreatorDialog) {
	ui->setupUi(this);
}

AddCreatorDialog::~AddCreatorDialog() {
	delete ui;
}

void AddCreatorDialog::on_buttonBox_accepted() {
	Creator creator;
	creator.name = ui->nameLineEdit->text();

	emit creatorAdded(creator);
}
