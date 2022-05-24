#include "RequestsManager.h"

#include <QWidget>
#include <QNetworkAccessManager>
#include <QNetworkRequest>
#include <QNetworkReply>
#include <QIODevice>

//==================================================================================================================================

RequestsManager::RequestsManager(QWidget* parent) : QNetworkAccessManager(parent) {}

//==================================================================================================================================

void RequestsManager::on_request(QNetworkAccessManager::Operation op, const QNetworkRequest& request, const QByteArray& data) {
	QNetworkReply* reply{ nullptr };

	switch(op) {
		case QNetworkAccessManager::HeadOperation: reply = head(request); break;
		case QNetworkAccessManager::GetOperation: reply = get(request); break;
		case QNetworkAccessManager::PutOperation: reply = put(request, data); break;
		case QNetworkAccessManager::PostOperation: reply = post(request, data); break;
		case QNetworkAccessManager::DeleteOperation: reply = deleteResource(request); break;
		case QNetworkAccessManager::CustomOperation: reply = sendCustomRequest(request, data); break;
		case QNetworkAccessManager::UnknownOperation: qDebug() << QString("Uknown operation for: [%1].").arg(data); break;
	}

	emit requestSent(reply);
}

//==================================================================================================================================
