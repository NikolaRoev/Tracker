#include "UpdateEntry.h"
#include "./ui_UpdateEntry.h"

#include <QFrame>

//==================================================================================================================================
//==================================================================================================================================

UpdateEntry::UpdateEntry(QWidget* parent) : QFrame(parent), ui(new Ui::UpdateEntry) {
	ui->setupUi(this);
}

//==================================================================================================================================

UpdateEntry::~UpdateEntry() {
	delete ui;
}

//==================================================================================================================================
//==================================================================================================================================
