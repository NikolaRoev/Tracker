#include "pch.h"
#include "SearchCreatorDialog.h"
#include "./ui_SearchCreatorDialog.h"
#include "DatabaseManager/DatabaseManager.h"

//==================================================================================================================================
//==================================================================================================================================

SearchCreatorDialog::SearchCreatorDialog(QWidget* parent) : QDialog(parent), ui(new Ui::SearchCreatorDialog) {
	ui->setupUi(this);
	emit ui->filterLineEdit->textEdited(QString());
}

//==================================================================================================================================

SearchCreatorDialog::~SearchCreatorDialog() {
	delete ui;
}

//==================================================================================================================================
//==================================================================================================================================

void SearchCreatorDialog::on_filterLineEdit_textEdited(const QString& text) {
	ui->listWidget->clear();

	QList<Creator> found_creators;
	if (DatabaseManager::search_creators(found_creators, text)){
		for (const auto& creator : found_creators) {
			QListWidgetItem* item = new QListWidgetItem(creator.name);
			item->setData(Qt::UserRole, creator.id);
			ui->listWidget->addItem(item);
		}
	}
	else {
		QMessageBox::warning(this, "Database Error", "Failed to search for Creators.");
	}
}

//==================================================================================================================================

void SearchCreatorDialog::on_listWidget_itemDoubleClicked(QListWidgetItem* item) {
	emit creatorSelected(item->data(Qt::UserRole).toInt(), item->text());
}

//==================================================================================================================================

void SearchCreatorDialog::on_buttonBox_accepted() {
	auto selected_items = ui->listWidget->selectedItems();
	if (!selected_items.isEmpty()) {
		emit creatorSelected(selected_items.first()->data(Qt::UserRole).toInt(), selected_items.first()->text());
	}
}

//==================================================================================================================================
//==================================================================================================================================
