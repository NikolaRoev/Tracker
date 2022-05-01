#include "WorkDialog.h"
#include "./ui_WorkDialog.h"
#include "DatabaseManager/DatabaseManager.h"

#include <QDialog>

//==================================================================================================================================

WorkDialog::WorkDialog(const int id, QWidget* parent) : QDialog(parent), ui(new Ui::WorkDialog) {
	ui->setupUi(this);
	setAttribute(Qt::WA_DeleteOnClose);

	Work work = DatabaseManager::get_work(id);

	setWindowTitle(work.name);
	ui->idLabel->setText(QString::number(work.id));
	ui->nameLabel->setText(work.name);
	ui->statusLabel->setText(work.status);
	ui->typeLabel->setText(work.status);
	ui->groupingLabel->setText(work.grouping);
	ui->chapterLabel->setText(work.chapter);
	ui->updated_label->setText(work.updated);
	ui->addedLabel->setText(work.added);

	for (const auto& creator : work.creators) {
		if (creator.type == "Author") {
			ui->authorListWidget->addItem(creator.name);
		}
		else if (creator.type == "Artist") {
			ui->artistListWidget->addItem(creator.name);
		}
	}
}

//==================================================================================================================================

WorkDialog::~WorkDialog() {
	delete ui;
}

//==================================================================================================================================
