#include "AttachCreatorDialog.h"
#include "./ui_AttachCreatorDialog.h"
#include "DatabaseManager/DatabaseManager.h"

#include <QDialog>
#include <QString>

//==================================================================================================================================
//==================================================================================================================================

AttachCreatorDialog::AttachCreatorDialog(const int work_id, const QString& type, QWidget* parent)
	: work_id(work_id), type(type), QDialog(parent), ui(new Ui::AttachCreatorDialog) {

	ui->setupUi(this);
	emit ui->filterLineEdit->textEdited(ui->filterLineEdit->text());
}

//==================================================================================================================================

AttachCreatorDialog::~AttachCreatorDialog() {
	delete ui;
}

//==================================================================================================================================
//==================================================================================================================================

void AttachCreatorDialog::on_filterLineEdit_textEdited(const QString& text) {
	//Clear list items.
	ui->listWidget->clear();

	//Find creators and populate the list.
	QVector<Creator> found_creators = DatabaseManager::search_creators(text);

	for (const auto& creator : found_creators) {
		QListWidgetItem* item = new QListWidgetItem(creator.name);
		item->setData(Qt::UserRole, creator.id);

		ui->listWidget->addItem(item);
	}
}

//==================================================================================================================================

void AttachCreatorDialog::on_buttonBox_accepted() {
	auto selected_items = ui->listWidget->selectedItems();

	if (!selected_items.isEmpty()) {
		DatabaseManager::attach_creator(work_id, selected_items.first()->data(Qt::UserRole).toInt(), type);
	}
}

//==================================================================================================================================
//==================================================================================================================================
