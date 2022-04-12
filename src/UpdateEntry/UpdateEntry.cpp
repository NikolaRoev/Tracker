#include "UpdateEntry.h"
#include "./ui_UpdateEntry.h"
#include "DatabaseManager/DatabaseManager.h"

#include <QWidget>
#include <QString>

//==================================================================================================================================
//==================================================================================================================================

UpdateEntry::UpdateEntry(const int id, const QString& name, const QString& chapter, QWidget* parent)
	: id(id), QWidget(parent), ui(new Ui::UpdateEntry) {

	ui->setupUi(this);
	ui->nameLineEdit->setText(name);
	ui->chapterLineEdit->setText(chapter);
}

//==================================================================================================================================

UpdateEntry::~UpdateEntry() {
	delete ui;
}

//==================================================================================================================================
//==================================================================================================================================

void UpdateEntry::on_nameLineEdit_textEdited(const QString& text) {
	DatabaseManager::update_work_name(id, text);
}

//==================================================================================================================================

void UpdateEntry::on_chapterLineEdit_textEdited(const QString& text) {
	DatabaseManager::update_work_chapter(id, text);
}

//==================================================================================================================================
//==================================================================================================================================
