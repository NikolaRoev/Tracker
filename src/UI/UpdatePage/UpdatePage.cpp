#include "UpdatePage.h"
#include "./ui_UpdatePage.h"
#include "UpdateEntry/UpdateEntry.h"
#include "DatabaseManager/DatabaseManager.h"
#include "DatabaseManager/Work.h"

#include <QWidget>
#include <QString>
#include <QLayoutItem>
#include <QShortcut>
#include <QKeySequence>

//==================================================================================================================================
//==================================================================================================================================

UpdatePage::UpdatePage(QWidget *parent) : QWidget(parent), ui(new Ui::UpdatePage) {
	ui->setupUi(this);


	//Align the update layout to top so the populated entries look nice.
	ui->contentsWidget->layout()->setAlignment(Qt::AlignTop);

	//Add a shortcut that selects all text in the search bar and focuses it.
	QShortcut* shortcut = new QShortcut(QKeySequence(Qt::CTRL | Qt::Key_F), this);
	connect(shortcut, &QShortcut::activated, ui->lineEdit, [&](){ ui->lineEdit->selectAll(); ui->lineEdit->setFocus(); });


	emit ui->lineEdit->textEdited("");
}

//==================================================================================================================================

UpdatePage::~UpdatePage() {
	delete ui;
}

//==================================================================================================================================
//==================================================================================================================================

void UpdatePage::on_lineEdit_textEdited(const QString& text) {
	//Clear the current update entries.
	QLayoutItem* child{ nullptr };
	while ((child = ui->contentsWidget->layout()->takeAt(0)) != nullptr) {
		delete child->widget();
		delete child;
	}

	//Find works and populate the update list.
	const auto found_works = DatabaseManager::search_works(text, "name", "Reading", NULL);
	for (const auto& found_work : found_works) {
		ui->contentsWidget->layout()->addWidget(new UpdateEntry(found_work, ui->scrollArea));
	}

	//Update the Status Bar.
	emit message(QString("Found %1 works.").arg(found_works.size()));
}

//==================================================================================================================================
//==================================================================================================================================
