#pragma once
#include "pch.h"

//==================================================================================================================================

QT_BEGIN_NAMESPACE
namespace Ui {
	class AddCreatorDialog;
}
QT_END_NAMESPACE

//==================================================================================================================================

class AddCreatorDialog : public QDialog {
	Q_OBJECT
private:
	Ui::AddCreatorDialog* ui{ nullptr };

public:
	AddCreatorDialog(QWidget* parent = nullptr);
	~AddCreatorDialog();

private slots:
	void on_addButton_clicked();
};

//==================================================================================================================================
