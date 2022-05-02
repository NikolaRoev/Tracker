#include "WorkPage.h"
#include "./ui_WorkPage.h"
#include "DatabaseManager/DatabaseManager.h"

#include <QDialog>

//==================================================================================================================================

WorkDialog::WorkDialog(const int id, QWidget* parent) : QDialog(parent), ui(new Ui::WorkPage) {
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

/*
 * void AddWorkDialog::on_authorSelected(const int id, const QString& name) {
	QListWidgetItem* item = new QListWidgetItem(name);
	item->setData(Qt::UserRole, id);
	ui->authorListWidget->addItem(item);
}

//==================================================================================================================================

void AddWorkDialog::on_artistSelected(const int id, const QString& name) {
	QListWidgetItem* item = new QListWidgetItem(name);
	item->setData(Qt::UserRole, id);
	ui->artistListWidget->addItem(item);
}

//==================================================================================================================================

void AddWorkDialog::on_authorAddButton_clicked() {
	SearchCreatorDialog* dialog = new SearchCreatorDialog(this);
	connect(dialog, &SearchCreatorDialog::creatorSelected, this, &AddWorkDialog::on_authorSelected);
	dialog->show();
}

//==================================================================================================================================

void AddWorkDialog::on_artistAddButton_clicked() {
	SearchCreatorDialog* dialog = new SearchCreatorDialog(this);
	connect(dialog, &SearchCreatorDialog::creatorSelected, this, &AddWorkDialog::on_artistSelected);
	dialog->show();
}

//==================================================================================================================================

void AddWorkDialog::on_authorListWidget_customContextMenuRequested(const QPoint &pos) {
	if (QListWidgetItem* item = ui->authorListWidget->itemAt(pos); item) {
		if (QVariant data = item->data(Qt::UserRole); data.isValid()) {
			QMenu menu(ui->authorListWidget);
			menu.addAction("Remove", [&](){
				int result = QMessageBox::warning(this,
												  "Removing Author",
												  QString("Are you sure you want to remove \"%1\" as Author?").arg(item->text()),
												  QMessageBox::Yes,
												  QMessageBox::No);

				if (result == QMessageBox::Yes) {
					ui->authorListWidget->removeItemWidget(item);
					delete item;
				}
			});
			menu.exec(QCursor::pos());
		}
	}
}

//==================================================================================================================================

void AddWorkDialog::on_artistListWidget_customContextMenuRequested(const QPoint &pos) {
	if (QListWidgetItem* item = ui->artistListWidget->itemAt(pos); item) {
		if (QVariant data = item->data(Qt::UserRole); data.isValid()) {
			QMenu menu(ui->artistListWidget);
			menu.addAction("Remove", [&](){
				int result = QMessageBox::warning(this,
												  "Removing Artist",
												  QString("Are you sure you want to remove \"%1\" as Artist?").arg(item->text()),
												  QMessageBox::Yes,
												  QMessageBox::No);

				if (result == QMessageBox::Yes) {
					ui->artistListWidget->removeItemWidget(item);
					delete item;
				}
			});
			menu.exec(QCursor::pos());
		}
	}
}

*/
