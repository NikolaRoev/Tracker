#include "pch.h"
#include "UpdatePage.h"
#include "./ui_UpdatePage.h"
#include "Page.h"
#include "UpdateEntry.h"
#include "DatabaseManager/DatabaseManager.h"
#include "DatabaseManager/Work.h"

UpdatePage::UpdatePage(QWidget* parent) : Page(parent), ui(new Ui::UpdatePage) {
	ui->setupUi(this);

	//Focus on the Search Line Edit.
	setFocusProxy(ui->lineEdit);

	//Add a shortcut that focuses the search bar and selects all the text.
	QShortcut* shortcut = new QShortcut(QKeySequence(Qt::CTRL | Qt::Key_F), this);
	connect(shortcut, &QShortcut::activated, ui->lineEdit, [&](){ ui->lineEdit->selectAll(); ui->lineEdit->setFocus(); });
}

UpdatePage::~UpdatePage() {
	delete ui;
}

void UpdatePage::populate() {
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

void UpdatePage::on_lineEdit_textEdited(const QString&) {
	populate();
}
