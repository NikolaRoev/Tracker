#pragma once
#include <QDialog>

//==================================================================================================================================

QT_BEGIN_NAMESPACE
namespace Ui {
	class AddWorkDialog;
}
QT_END_NAMESPACE

//==================================================================================================================================

class AddWorkDialog : public QDialog {
	Q_OBJECT
private:
	Ui::AddWorkDialog* ui{ nullptr };

public:
	AddWorkDialog(QWidget* parent = nullptr);
	~AddWorkDialog();

private slots:
	void on_authorEditButton_clicked();
	void on_artistEditButton_clicked();

	void on_buttonBox_accepted();
};

//==================================================================================================================================
