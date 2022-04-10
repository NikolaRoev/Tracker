#include "UpdateEntry.h"
#include "./ui_UpdateEntry.h"
#include "DatabaseManager/DatabaseManager.h"

#include <QFrame>
#include <QString>

//==================================================================================================================================
//==================================================================================================================================

UpdateEntry::UpdateEntry(const QString& name, const QString& chapter, QWidget* parent)
	: name(name), QFrame(parent), ui(new Ui::UpdateEntry) {

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

void UpdateEntry::on_nameLineEdit_textChanged(const QString& text) {
	DatabaseManager::update_work_name(name, text);
	name = text;

	//will need to rework database so we can change the name easily.
	//maybe just use an id for the primary key. makes foreign key easier and we can reuse names?
}

//==================================================================================================================================

void UpdateEntry::on_chapterLineEdit_textChanged(const QString& text) {
	DatabaseManager::update_work_chapter(name, text);
}

//==================================================================================================================================
//==================================================================================================================================
