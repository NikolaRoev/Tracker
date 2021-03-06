#pragma once
#include "pch.h"

//==================================================================================================================================

QT_BEGIN_NAMESPACE
namespace Ui {
	class SearchCreatorDialog;
}
QT_END_NAMESPACE

//==================================================================================================================================

class SearchCreatorDialog : public QDialog {
	Q_OBJECT
private:
	Ui::SearchCreatorDialog* ui{ nullptr };

public:
	SearchCreatorDialog(QWidget* parent = nullptr);
	~SearchCreatorDialog();

private slots:
	void on_filterLineEdit_textEdited(const QString& text);
	void on_listWidget_itemDoubleClicked(QListWidgetItem* item);
	void on_buttonBox_accepted();

signals:
	void creatorSelected(const int id, const QString& name);
};

//==================================================================================================================================
