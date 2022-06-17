#include "pch.h"
#include "UpdateEntry.h"
#include "./ui_UpdateEntry.h"
#include "DatabaseManager/DatabaseManager.h"
#include "DatabaseManager/Work.h"

//==================================================================================================================================
//==================================================================================================================================

UpdateEntry::UpdateEntry(const Work& work, QWidget* parent) : QWidget(parent), ui(new Ui::UpdateEntry), id(work.id) {
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
	DatabaseManager::update_work("updated", id, util::current_datetime());
}

//==================================================================================================================================
//==================================================================================================================================
