#include "UpdateEntry.h"
#include "./ui_UpdateEntry.h"
#include "DatabaseManager/DatabaseManager.h"
#include "DatabaseManager/UpdateWork.h"

#include <QWidget>
#include <QString>

//==================================================================================================================================
//==================================================================================================================================

UpdateEntry::UpdateEntry(const UpdateWork& update_work, QWidget* parent)
	: id(update_work.id), QWidget(parent), ui(new Ui::UpdateEntry) {

	ui->setupUi(this);
	ui->nameLineEdit->setText(update_work.name);
	ui->chapterLineEdit->setText(update_work.chapter);
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
