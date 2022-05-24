#include "MangaUpdatesPage.h"
#include "./ui_MangaUpdatesPage.h"

#include <QWidget>
#include <QString>
#include <QNetworkAccessManager>
#include <QNetworkRequest>
#include <QNetworkReply>
#include <QUrl>
#include <QJsonObject>
#include <QJsonDocument>
#include <QByteArray>
#include <QSettings>
#include <QMessageBox>

//==================================================================================================================================
//==================================================================================================================================
//==================================================================================================================================

void MangaUpdatesPage::get_account() {


	ui->stackedWidget->setCurrentIndex(1);
	ui->getButton->setEnabled(true);
}

//==================================================================================================================================
//==================================================================================================================================

MangaUpdatesPage::MangaUpdatesPage(QWidget* parent) : QWidget(parent), ui(new Ui::MangaUpdatesPage) {
	ui->setupUi(this);

	//Load settings.
	QSettings settings("settings.ini", QSettings::IniFormat, this);
	settings.beginGroup("MangaUpdates");
	token = settings.value("token").toString();
	settings.endGroup();

	if (!token.isNull()) {
		get_account();
	}
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

void MangaUpdatesPage::set_network_access_manager(QNetworkAccessManager* network_access_manager) {
	this->network_access_manager = network_access_manager;
}

//==================================================================================================================================
//==================================================================================================================================
//==================================================================================================================================

void MangaUpdatesPage::on_loginButton_clicked() {
	ui->loginButton->setDisabled(true);


	QNetworkRequest request(QUrl("https://api.mangaupdates.com/v1/account/login"));
	request.setHeader(QNetworkRequest::ContentTypeHeader, "application/json");

	QJsonObject object;
	object["username"] = ui->usernameLineEdit->text();
	object["password"] = ui->passwordLineEdit->text();


	QNetworkReply* reply = network_access_manager->put(request, QJsonDocument(object).toJson());

	QObject::connect(reply, &QNetworkReply::finished, this, [=](){
		QByteArray contents = reply->readAll();
		QJsonDocument document = QJsonDocument::fromJson(contents);
		QJsonObject object = document.object();


		if (reply->error() == QNetworkReply::NoError) {
			token = object["context"].toObject()["session_token"].toString();

			get_account();
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

void MangaUpdatesPage::on_logoutButton_clicked() {
	ui->logoutButton->setDisabled(true);


	QNetworkRequest request(QUrl("https://api.mangaupdates.com/v1/account/logout"));
	request.setRawHeader("Authorization", token.toUtf8());


	QNetworkReply* reply = network_access_manager->post(request, QByteArray());

	QObject::connect(reply, &QNetworkReply::finished, this, [=](){
		QByteArray contents = reply->readAll();
		QJsonDocument document = QJsonDocument::fromJson(contents);
		QJsonObject object = document.object();


		if (reply->error() == QNetworkReply::NoError) {
			token = NULL;

			ui->stackedWidget->setCurrentIndex(0);
			ui->getButton->setDisabled(true);
		}
		else {
			qDebug() << QString("Error [%1][%2]: %3.")
							.arg(reply->attribute(QNetworkRequest::HttpStatusCodeAttribute).toInt())
							.arg(reply->error())
							.arg(contents);

			QMessageBox::warning(this, "Failed to Log out.", object["reason"].toString());
		}


		ui->logoutButton->setEnabled(true);
		reply->deleteLater();
	});
}

//==================================================================================================================================

void MangaUpdatesPage::on_getButton_clicked() {

}

//==================================================================================================================================
//==================================================================================================================================
//==================================================================================================================================
