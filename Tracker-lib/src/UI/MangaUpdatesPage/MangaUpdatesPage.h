#pragma once
#include "pch.h"

//==================================================================================================================================

QT_BEGIN_NAMESPACE
namespace Ui {
	class MangaUpdatesPage;
}
QT_END_NAMESPACE

//==================================================================================================================================

class MangaUpdatesPage : public QWidget {
	Q_OBJECT
private:
	Ui::MangaUpdatesPage* ui{ nullptr };
	QString token;


	void validate();

public:
	MangaUpdatesPage(QWidget* parent = nullptr);
	~MangaUpdatesPage();

private slots:
	void on_loginButton_clicked();
	void on_logoutButton_clicked();

	void on_openButton_clicked();
	void on_getButton_clicked();
	void on_tableWidget_doubleClicked(const QModelIndex& index);

	void on_login(const QString& username);
	void on_logout();

signals:
	void login(const QString& username);
	void logout();
};

//==================================================================================================================================
