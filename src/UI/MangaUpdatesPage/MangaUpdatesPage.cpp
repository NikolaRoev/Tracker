#include "MangaUpdatesPage.h"
#include "./ui_MangaUpdatesPage.h"

#include <QWidget>
#include <QNetworkAccessManager>
#include <QNetworkRequest>
#include <QNetworkReply>
#include <QJsonObject>
#include <QJsonDocument>
#include <QByteArray>
#include <QSettings>
#include <QMessageBox>

//==================================================================================================================================
//==================================================================================================================================
//==================================================================================================================================

MangaUpdatesPage::MangaUpdatesPage(QWidget* parent) : QWidget(parent), ui(new Ui::MangaUpdatesPage) {
	ui->setupUi(this);

	//Load settings.
	QSettings settings("settings.ini", QSettings::IniFormat, this);
	settings.beginGroup("MangaUpdates");
	token = settings.value("token").toString();
	settings.endGroup();

	//If token is not null:
	//Try to get the account info here to see if the token is still valid.
}

//==================================================================================================================================

MangaUpdatesPage::~MangaUpdatesPage() {
	QSettings settings("settings.ini", QSettings::IniFormat, this);
	settings.beginGroup("MangaUpdates");
	settings.setValue("token", token);
	settings.endGroup();

	delete ui;
}

//==================================================================================================================================
//==================================================================================================================================
//==================================================================================================================================

void MangaUpdatesPage::on_loginButton_clicked() {
	ui->loginButton->setDisabled(true);

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
	QObject::connect(reply, &QNetworkReply::finished, this, [=](){
		QByteArray contents = reply->readAll();
		QJsonDocument document = QJsonDocument::fromJson(contents);
		QJsonObject object = document.object();


		if (reply->error() == QNetworkReply::NoError) {
			token = object["context"].toObject()["session_token"].toString();

			//TO DO: Add the account request.

			ui->stackedWidget->setCurrentIndex(1);
		}
		else {
			qDebug() << QString("Error [%1][%2]: %3.")
							.arg(reply->attribute(QNetworkRequest::HttpStatusCodeAttribute).toInt())
							.arg(reply->error())
							.arg(contents);

			QMessageBox::warning(this, "Failed to Log in.", object["reason"].toString());
		}


		ui->loginButton->setEnabled(true);
		reply->deleteLater();
	});
}

//==================================================================================================================================
//==================================================================================================================================
//==================================================================================================================================
