#include "CreatorDialog.h"
#include "./ui_CreatorDialog.h"
#include "DatabaseManager/DatabaseManager.h"

#include <QDialog>

//==================================================================================================================================

CreatorDialog::CreatorDialog(const int id, QWidget* parent) : QDialog(parent), ui(new Ui::CreatorDialog) {
	ui->setupUi(this);
	setAttribute(Qt::WA_DeleteOnClose);

	Creator creator = DatabaseManager::get_creator(id);

	setWindowTitle(creator.name);
	ui->idLabel->setText(QString::number(creator.id));
	ui->nameLabel->setText(creator.name);

	for (const auto& work : creator.works) {
		ui->listWidget->addItem(work.name);
	}
}

//==================================================================================================================================

CreatorDialog::~CreatorDialog() {
	delete ui;
}

//==================================================================================================================================
