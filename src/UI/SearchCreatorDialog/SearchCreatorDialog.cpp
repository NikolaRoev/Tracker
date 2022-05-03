#include "SearchCreatorDialog.h"
#include "./ui_SearchCreatorDialog.h"
#include "DatabaseManager/DatabaseManager.h"

#include <QDialog>
#include <QString>

//==================================================================================================================================
//==================================================================================================================================

SearchCreatorDialog::SearchCreatorDialog(QWidget* parent) : QDialog(parent), ui(new Ui::SearchCreatorDialog) {
	ui->setupUi(this);
	setAttribute(Qt::WA_DeleteOnClose);
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

	QVector<Creator> found_creators = DatabaseManager::search_creators(text);
	for (const auto& creator : found_creators) {
		QListWidgetItem* item = new QListWidgetItem(creator.name);
		item->setData(Qt::UserRole, creator.id);
		ui->listWidget->addItem(item);
	}
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
