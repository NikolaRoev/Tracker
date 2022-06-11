#include "pch.h"
#include "MangaDexPage.h"
#include "./ui_MangaDexPage.h"
#include "DatabaseManager/DatabaseManager.h"
#include "DatabaseManager/Work.h"
#include "RequestsManager/RequestsManager.h"

//==================================================================================================================================
//==================================================================================================================================
//==================================================================================================================================


/*
 * 	QJsonDocument jdocHeader = QJsonDocument::fromJson( QByteArray::fromBase64( QString("eyJ0eXAiOiJyZWZyZXNoIiwiaXNzIjoibWFuZ2FkZXgub3JnIiwiYXVkIjoibWFuZ2FkZXgub3JnIiwiaWF0IjoxNjUzNjY1NTA0LCJuYmYiOjE2NTM2NjU1MDQsImV4cCI6MTY1NjI1NzUwNCwidWlkIjoiYTJhMTIxZWUtZWE1My00ZTRhLTg0MTEtM2UzOTM4MzdmYjI5Iiwic2lkIjoiODUwOTAyYmMtYTEzNi00MmM4LWEzMDgtMDMyZWQ5OGQxMDBhIn0").toUtf8() ) );
	QJsonObject jobjHeader = jdocHeader.object();
	qDebug() << jobjHeader;
	QString szFull=QDateTime::fromSecsSinceEpoch(jobjHeader["exp"].toInteger()).toString("dddd d MMMM yyyy hh:mm:ss");
	qDebug() << szFull;
 *
*/

//==================================================================================================================================
//==================================================================================================================================

void MangaDexPage::refresh() {
	if (!refresh_token.isEmpty()) {
		QNetworkRequest request(QUrl("https://api.mangadex.org/auth/refresh"));
		request.setHeader(QNetworkRequest::ContentTypeHeader, "application/json");
		QJsonObject data_object;
		data_object["token"] = refresh_token;
		QNetworkReply* reply = RequestsManager::put(request, QJsonDocument(data_object).toJson());


		QObject::connect(reply, &QNetworkReply::finished, this, [this, reply](){
			QByteArray contents = reply->readAll();
			QJsonObject object = QJsonDocument::fromJson(contents).object();


			if (reply->error() == QNetworkReply::NoError) {
				session_token = object["token"].toObject()["session"].toString();

				//TO DO:
			}
			else {
				qDebug() << QString("Error [%1][%2]: %3.")
								.arg(reply->attribute(QNetworkRequest::HttpStatusCodeAttribute).toInt())
								.arg(reply->error())
								.arg(contents);

				QJsonObject errors = object["errors"].toObject();
				QMessageBox::warning(this, "Failed to refresh MangaDex session token.",
									 QString("%1: %2.").arg(errors["title"].toString(), errors["detail"].toString()));
			}


			reply->deleteLater();
		});
	}
}

//==================================================================================================================================

void MangaDexPage::validate() {
	if (!session_token.isEmpty()) {
		QNetworkRequest request(QUrl("https://api.mangadex.org/auth/check"));
		request.setRawHeader("Authorization", session_token.toUtf8());
		QNetworkReply* reply = RequestsManager::get(request);


		QObject::connect(reply, &QNetworkReply::finished, this, [this, reply](){
			QByteArray contents = reply->readAll();
			QJsonObject object = QJsonDocument::fromJson(contents).object();


			bool result{ false };
			if (reply->error() == QNetworkReply::NoError) {
				result = object["isAuthenticated"].toBool();
			}
			else {
				qDebug() << QString("Error [%1][%2]: %3.")
								.arg(reply->attribute(QNetworkRequest::HttpStatusCodeAttribute).toInt())
								.arg(reply->error())
								.arg(contents);

				QMessageBox::warning(this, "Failed to validate MangaDex token.", contents);
			}


			if (result) {
				//TO DO: Allow the program to continue.
			}
			else {
				refresh();
			}


			reply->deleteLater();
		});
	}
}

//==================================================================================================================================
//==================================================================================================================================

MangaDexPage::MangaDexPage(QWidget* parent) : QWidget(parent), ui(new Ui::MangaDexPage) {
	ui->setupUi(this);

	//Set resize mode for the Table Widget.
	ui->tableWidget->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
	ui->tableWidget->horizontalHeader()->setDefaultAlignment(Qt::AlignLeft);
	ui->tableWidget->verticalHeader()->setSectionResizeMode(QHeaderView::ResizeToContents);

	//Load settings.
	QSettings settings("settings.ini", QSettings::IniFormat, this);
	settings.beginGroup("MangaDex");
	session_token = settings.value("session_token").toString();
	refresh_token = settings.value("refresh_token").toString();
	settings.endGroup();

	validate();
}

//==================================================================================================================================

MangaDexPage::~MangaDexPage() {
	QSettings settings("settings.ini", QSettings::IniFormat, this);
	settings.beginGroup("MangaDex");
	settings.setValue("session_token", session_token);
	settings.setValue("refresh_token", refresh_token);
	settings.endGroup();

	delete ui;
}

//==================================================================================================================================
//==================================================================================================================================
//==================================================================================================================================

void MangaDexPage::on_loginButton_clicked() {

}

//==================================================================================================================================

void MangaDexPage::on_logoutButton_clicked() {

}

//==================================================================================================================================

void MangaDexPage::on_openButton_clicked() {

}

//==================================================================================================================================

void MangaDexPage::on_getButton_clicked() {
	ui->tableWidget->setRowCount(0);


}

//==================================================================================================================================

void MangaDexPage::on_tableWidget_doubleClicked(const QModelIndex& index) {

}

//==================================================================================================================================
//==================================================================================================================================
//==================================================================================================================================
