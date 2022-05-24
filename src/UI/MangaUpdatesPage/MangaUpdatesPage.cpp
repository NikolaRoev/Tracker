#include "MangaUpdatesPage.h"
#include "./ui_MangaUpdatesPage.h"

#include <QWidget>
#include <QNetworkAccessManager>
#include <QNetworkRequest>
#include <QNetworkReply>
#include <QJsonObject>
#include <QJsonDocument>
#include <QByteArray>

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
	//ui->loginButton->setDisabled(true);

	const QUrl url("https://api.mangaupdates.com/v1/account/login");
	QNetworkRequest req(url);
	req.setHeader(QNetworkRequest::ContentTypeHeader, "application/json");

	QJsonObject object;
	object["username"] = ui->usernameLineEdit->text();
	object["password"] = ui->passwordLineEdit->text();
	QJsonDocument document(object);
	QByteArray data = document.toJson();

	emit request(QNetworkAccessManager::Operation::PutOperation, req, data);
}

//==================================================================================================================================

void MangaUpdatesPage::on_logoutButton_clicked() {
	//try to log out

	ui->stackedWidget->setCurrentIndex(0);
}

//==================================================================================================================================

void MangaUpdatesPage::on_getButton_clicked() {
	//populate releases
	//request.setRawHeader("Authorization", "");
}

//==================================================================================================================================
//==================================================================================================================================

void MangaUpdatesPage::on_requestSent(QNetworkReply* reply) {
	QObject::connect(reply, &QNetworkReply::finished, [=](){
		if (reply->error() == QNetworkReply::NoError) {
			QString contents = QString::fromUtf8(reply->readAll());
			qDebug() << QJsonDocument::fromJson(contents.toUtf8());

			//ui->stackedWidget->setCurrentIndex(1);
		}
		else {
			qDebug() << reply->error() << QString::fromUtf8(reply->readAll());
		}

		reply->deleteLater();
	});
}

//==================================================================================================================================
//==================================================================================================================================
//==================================================================================================================================
