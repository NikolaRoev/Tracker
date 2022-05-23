#pragma once
#include <QWidget>
#include <QNetworkAccessManager>
#include <QNetworkRequest>
#include <QNetworkReply>

//==================================================================================================================================

class RequestsManager : QNetworkAccessManager {
	Q_OBJECT
public:
	RequestsManager(QWidget* parent = nullptr);

public slots:
	void on_request(QNetworkAccessManager::Operation op, const QNetworkRequest& request, QIODevice* data = nullptr);

signals:
	void request_sent(QNetworkReply* reply);
};

//==================================================================================================================================
