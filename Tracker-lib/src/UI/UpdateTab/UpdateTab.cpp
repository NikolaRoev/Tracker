#include "pch.h"
#include "UpdateTab.h"
#include "./ui_UpdateTab.h"
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

}

void UpdateTab::on_lineEdit_textEdited(const QString& text) {

}
