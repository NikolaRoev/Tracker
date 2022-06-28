#include "pch.h"
#include "UpdateTab.h"
#include "./ui_UpdateTab.h"
#include "UpdateEntry.h"
#include "DatabaseManager/DatabaseManager.h"
#include "DatabaseManager/Work.h"

UpdateTab::UpdateTab(QWidget* parent) : QWidget(parent), ui(new Ui::UpdateTab) {
	ui->setupUi(this);

	//Align the update list layout to top so the populated entries look nice.
	ui->scrollAreaWidgetContents->layout()->setAlignment(Qt::AlignTop);

	//Focus on the Search Line Edit.
	setFocusProxy(ui->lineEdit);

	//Add a shortcut that focuses the search bar and selects all the text.
	QShortcut* shortcut = new QShortcut(QKeySequence(Qt::CTRL | Qt::Key_F), this);
	connect(shortcut, &QShortcut::activated, ui->lineEdit, [&](){ ui->lineEdit->selectAll(); ui->lineEdit->setFocus(); });
}

UpdateTab::~UpdateTab() {
	delete ui;
}

void UpdateTab::populate() {
	//Clear the Scroll Area.
	QLayoutItem* child{ nullptr };
	while ((child = ui->scrollAreaWidgetContents->layout()->takeAt(0)) != nullptr) {
		delete child->widget();
		delete child;
	}

	QList<Work> found_works;
	DatabaseManager::search_works(found_works, ui->lineEdit->text(), "name", "Reading", NULL);
	for (const auto& found_work : found_works) {
		ui->scrollAreaWidgetContents->layout()->addWidget(new UpdateEntry(found_work, ui->scrollArea));
	}
}

void UpdateTab::on_lineEdit_textEdited(const QString&) {
	populate();
}
