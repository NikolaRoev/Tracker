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
	QNetworkAccessManager* network_access_manager{ nullptr };
	QString token;


	void validate();

public:
	MangaUpdatesPage(QWidget* parent = nullptr);
	~MangaUpdatesPage();

	void setup(QNetworkAccessManager* network_access_manager);

private slots:
	void on_loginButton_clicked();
	void on_logoutButton_clicked();

	void on_getButton_clicked();
	void on_tableWidget_clicked(const QModelIndex& index);
};

//==================================================================================================================================
