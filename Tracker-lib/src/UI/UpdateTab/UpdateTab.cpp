#include "pch.h"
#include "UpdateTab.h"
#include "./ui_UpdateTab.h"
#include "UpdateEntry.h"
#include "DatabaseManager/DatabaseManager.h"
#include "DatabaseManager/Work.h"

UpdateTab::UpdateTab(QWidget* parent) : QWidget(parent), ui(new Ui::UpdateTab) {
	ui->setupUi(this);

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
	ui->listWidget->clear();

	QList<Work> found_works;
	DatabaseManager::search_works(found_works, ui->lineEdit->text(), "name", "Reading", NULL);
	for (const auto& found_work : found_works) {
		UpdateEntry* widget = new UpdateEntry(found_work, ui->listWidget);
		QListWidgetItem* item = new QListWidgetItem;
		item->setSizeHint(widget->sizeHint());

		ui->listWidget->addItem(item);
		ui->listWidget->setItemWidget(item, widget);
	}
}

void UpdateTab::on_lineEdit_textEdited(const QString&) {
	populate();
}
