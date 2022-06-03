#include "pch.h"
#include "MangaDexPage.h"
#include "./ui_MangaDexPage.h"
#include "DatabaseManager/DatabaseManager.h"
#include "DatabaseManager/Work.h"

//==================================================================================================================================
//==================================================================================================================================
//==================================================================================================================================

void MangaDexPage::validate() {
	QJsonDocument jdocHeader = QJsonDocument::fromJson( QByteArray::fromBase64( QString("eyJ0eXAiOiJyZWZyZXNoIiwiaXNzIjoibWFuZ2FkZXgub3JnIiwiYXVkIjoibWFuZ2FkZXgub3JnIiwiaWF0IjoxNjUzNjY1NTA0LCJuYmYiOjE2NTM2NjU1MDQsImV4cCI6MTY1NjI1NzUwNCwidWlkIjoiYTJhMTIxZWUtZWE1My00ZTRhLTg0MTEtM2UzOTM4MzdmYjI5Iiwic2lkIjoiODUwOTAyYmMtYTEzNi00MmM4LWEzMDgtMDMyZWQ5OGQxMDBhIn0").toUtf8() ) );
	QJsonObject jobjHeader = jdocHeader.object();
	qDebug() << jobjHeader;
	QString szFull=QDateTime::fromSecsSinceEpoch(jobjHeader["exp"].toInteger()).toString("dddd d MMMM yyyy hh:mm:ss");
	qDebug() << szFull;
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

void MangaDexPage::setup(QNetworkAccessManager* network_access_manager) {
	this->network_access_manager = network_access_manager;

	validate();
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

void MangaDexPage::on_getButton_clicked() {
	//Clear table items.
	ui->tableWidget->setRowCount(0);


}

//==================================================================================================================================

void MangaDexPage::on_tableWidget_clicked(const QModelIndex& index) {

}

//==================================================================================================================================
//==================================================================================================================================
//==================================================================================================================================