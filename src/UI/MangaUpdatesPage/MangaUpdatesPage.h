#pragma once
#include <QWidget>
#include <QString>
#include <QNetworkAccessManager>

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

public:
	MangaUpdatesPage(QWidget* parent = nullptr);
	~MangaUpdatesPage();

	void set_network_access_manager(QNetworkAccessManager* network_access_manager);

private slots:
	void on_loginButton_clicked();
	void on_logoutButton_clicked();
	void on_getButton_clicked();
};

//==================================================================================================================================
