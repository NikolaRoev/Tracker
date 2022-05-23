#include "RequestsManager.h"

#include <QUrl>
#include <QNetworkRequest>
#include <QNetworkReply>
#include <QByteArray>
#include <QJsonDocument>

RequestsManager::RequestsManager(QObject* parent) {
	manager = new QNetworkAccessManager(parent);


	//const QUrl url("https://api.mangaupdates.com/v1/releases/days?include_metadata=true");
	//QNetworkRequest request(url);
	//request.setRawHeader("Authorization", "eyJ0eXAiOiJKV1QiLCJhbGciOiJIUzI1NiJ9.eyJzZXNzaW9uIjoiZGEwZTllN2QtMTlmNi00YTE1LWE5M2YtZTRlODFiZmEzM2I3IiwidGltZV9jcmVhdGVkIjoxNjUzMTUzNDc4fQ.yoy2J5vS6kqpUEK4PJpVoIrdMio-ouDUN-QiK69-b7U");
	//QNetworkReply *reply = manager->get(request);


	//QObject::connect(reply, &QNetworkReply::finished, [=](){
	//	if (reply->error() == QNetworkReply::NoError) {
	//		QString contents = QString::fromUtf8(reply->readAll());
	//		qDebug() << QJsonDocument::fromJson(contents.toUtf8());
	//	}
	//	else {
	//		qDebug() << reply->error() << QString::fromUtf8(reply->readAll());
	//	}
	//	reply->deleteLater();
	//});
}
