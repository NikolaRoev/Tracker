#include "MangaUpdatesPage.h"
#include "./ui_MangaUpdatesPage.h"

#include <QWidget>

//==================================================================================================================================
//==================================================================================================================================
//==================================================================================================================================

MangaUpdatesPage::MangaUpdatesPage(QWidget* parent) : QWidget(parent), ui(new Ui::MangaUpdatesPage) {
	ui->setupUi(this);
}

//==================================================================================================================================

MangaUpdatesPage::~MangaUpdatesPage() {
	delete ui;
}

//==================================================================================================================================
//==================================================================================================================================
//==================================================================================================================================

void MangaUpdatesPage::on_loginButton_clicked() {
	//try to log in

	ui->stackedWidget->setCurrentIndex(1);
}

//==================================================================================================================================

void MangaUpdatesPage::on_logoutButton_clicked() {
	//try to log out

	ui->stackedWidget->setCurrentIndex(0);
}

//==================================================================================================================================
//==================================================================================================================================

void MangaUpdatesPage::on_getButton_clicked() {
	//populate releases
}

//==================================================================================================================================
//==================================================================================================================================
//==================================================================================================================================
/*
const QUrl url("https://api.mangaupdates.com/v1/releases/days?include_metadata=true");
QNetworkRequest request(url);
request.setRawHeader("Authorization", "eyJ0eXAiOiJKV1QiLCJhbGciOiJIUzI1NiJ9.eyJzZXNzaW9uIjoiZGEwZTllN2QtMTlmNi00YTE1LWE5M2YtZTRlODFiZmEzM2I3IiwidGltZV9jcmVhdGVkIjoxNjUzMTUzNDc4fQ.yoy2J5vS6kqpUEK4PJpVoIrdMio-ouDUN-QiK69-b7U");
QNetworkReply *reply = createRequest(QNetworkAccessManager::Operation::GetOperation, request);


QObject::connect(reply, &QNetworkReply::finished, [=](){
	if (reply->error() == QNetworkReply::NoError) {
		QString contents = QString::fromUtf8(reply->readAll());
		qDebug() << QJsonDocument::fromJson(contents.toUtf8());
	}
	else {
		qDebug() << reply->error() << QString::fromUtf8(reply->readAll());
	}
	reply->deleteLater();
});
*/
//connect a signal from this class aka a request to the on_request slot of the request object in the main window.
//Then we can connect the request sent to a slot here so we can handle the reply object.
