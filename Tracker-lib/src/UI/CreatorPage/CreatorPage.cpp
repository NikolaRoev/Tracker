#include "pch.h"
#include "CreatorPage.h"
#include "./ui_CreatorPage.h"
#include "DatabaseManager/DatabaseManager.h"
#include "DatabaseManager/Work.h"
#include "DatabaseManager/Creator.h"

//==================================================================================================================================
//==================================================================================================================================

CreatorPage::CreatorPage(const int id, QWidget* parent) : QWidget(parent), ui(new Ui::CreatorPage), id(id) {
	ui->setupUi(this);
	ui->tableWidget->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
	ui->tableWidget->horizontalHeader()->setDefaultAlignment(Qt::AlignLeft);
	ui->tableWidget->verticalHeader()->setSectionResizeMode(QHeaderView::ResizeToContents);

	Creator creator;
	DatabaseManager::get_creator(creator, id);
	ui->idLabel->setText(QString::number(creator.id));
	ui->lineEdit->setText(creator.name);

	for (const auto& work : creator.works) {
		ui->tableWidget->insertRow(ui->tableWidget->rowCount());

		QTableWidgetItem* name_item = new QTableWidgetItem(work.name);
		name_item->setData(Qt::UserRole, work.id);
		ui->tableWidget->setItem(ui->tableWidget->rowCount() - 1, 0, name_item);
		ui->tableWidget->setItem(ui->tableWidget->rowCount() - 1, 1, new QTableWidgetItem(work.status));
		ui->tableWidget->setItem(ui->tableWidget->rowCount() - 1, 2, new QTableWidgetItem(work.chapter));
		ui->tableWidget->setItem(ui->tableWidget->rowCount() - 1, 3, new QTableWidgetItem(work.type));
		ui->tableWidget->setItem(ui->tableWidget->rowCount() - 1, 4, new QTableWidgetItem(work.updated));
		ui->tableWidget->setItem(ui->tableWidget->rowCount() - 1, 5, new QTableWidgetItem(work.added));
	}
}

//==================================================================================================================================

CreatorPage::~CreatorPage() {
	delete ui;
}

//==================================================================================================================================
//==================================================================================================================================

void CreatorPage::on_lineEdit_textEdited(const QString& text) {
	DatabaseManager::update_creator("name", id, text);
}

//==================================================================================================================================

void CreatorPage::on_tableWidget_clicked(const QModelIndex& index) {
	emit workClicked(ui->tableWidget->item(index.row(), 0)->data(Qt::UserRole).toInt());
}

//==================================================================================================================================
//==================================================================================================================================
