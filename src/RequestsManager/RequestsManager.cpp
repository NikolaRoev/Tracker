#include "RequestsManager.h"

#include <QWidget>
#include <QNetworkAccessManager>
#include <QNetworkRequest>
#include <QNetworkReply>
#include <QIODevice>

//==================================================================================================================================

RequestsManager::RequestsManager(QWidget* parent) : QNetworkAccessManager(parent) {}

//==================================================================================================================================

void RequestsManager::on_request(QNetworkAccessManager::Operation op, const QNetworkRequest& request, QIODevice* data) {
	emit request_sent(createRequest(op, request, data));
}

//==================================================================================================================================
