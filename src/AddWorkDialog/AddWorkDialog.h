#pragma once
#include <QDialog>
#include <QListWidgetItem>

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
	void on_authorAddButton_clicked();
	void on_artistAddButton_clicked();
	void on_authorListWidget_itemClicked(QListWidgetItem *item);
	void on_artistListWidget_itemClicked(QListWidgetItem *item);

	void on_buttonBox_accepted();
};

//==================================================================================================================================
