#pragma once
#include <QWidget>
#include <QNetworkAccessManager>
#include <QNetworkRequest>
#include <QNetworkReply>
#include <QByteArray>

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

public:
	MangaUpdatesPage(QWidget* parent = nullptr);
	~MangaUpdatesPage();

private slots:
	void on_loginButton_clicked();
	void on_logoutButton_clicked();
	void on_getButton_clicked();

public slots:
	void on_requestSent(QNetworkReply* reply);

signals:
	void request(QNetworkAccessManager::Operation op, const QNetworkRequest& request, const QByteArray& data);
};

//==================================================================================================================================
