#pragma once
#include <QWidget>
#include <QNetworkAccessManager>
#include <QNetworkRequest>
#include <QNetworkReply>

//==================================================================================================================================

class RequestsManager : public QNetworkAccessManager {
	Q_OBJECT
public:
	RequestsManager(QWidget* parent = nullptr);

public slots:
	void on_request(QNetworkAccessManager::Operation op, const QNetworkRequest& request, const QByteArray& data);

signals:
	void requestSent(QNetworkReply* reply);
};

//==================================================================================================================================
