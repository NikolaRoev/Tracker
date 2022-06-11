#pragma once
#include "pch.h"

//==================================================================================================================================

QT_BEGIN_NAMESPACE
namespace Ui {
	class MangaDexPage;
}
QT_END_NAMESPACE

//==================================================================================================================================

class MangaDexPage : public QWidget {
	Q_OBJECT
private:
	Ui::MangaDexPage* ui{ nullptr };
	QString session_token;
	QString refresh_token;


	void refresh();
	void validate();

public:
	MangaDexPage(QWidget* parent = nullptr);
	~MangaDexPage();

private slots:
	void on_loginButton_clicked();
	void on_logoutButton_clicked();

	void on_openButton_clicked();
	void on_getButton_clicked();
	void on_tableWidget_doubleClicked(const QModelIndex& index);
};

//==================================================================================================================================
