#include "UpdateEntry.h"
#include "./ui_UpdateEntry.h"
#include "DatabaseManager/DatabaseManager.h"
#include "DatabaseManager/Work.h"

#include <QWidget>
#include <QString>

//==================================================================================================================================
//==================================================================================================================================

UpdateEntry::UpdateEntry(const Work& work, QWidget* parent) : id(work.id), QWidget(parent), ui(new Ui::UpdateEntry) {
	ui->setupUi(this);
	ui->nameLineEdit->setText(work.name);
	ui->chapterLineEdit->setText(work.chapter);
}

//==================================================================================================================================

UpdateEntry::~UpdateEntry() {
	delete ui;
}

//==================================================================================================================================
//==================================================================================================================================

void UpdateEntry::on_nameLineEdit_textEdited(const QString& text) {
	DatabaseManager::update_work("name", id, text);
}

//==================================================================================================================================

void UpdateEntry::on_chapterLineEdit_textEdited(const QString& text) {
	DatabaseManager::update_work("chapter", id, text);
}

//==================================================================================================================================
//==================================================================================================================================
