#include "MangaDexPage.h"
#include "./ui_MangaDexPage.h"
#include "DatabaseManager/DatabaseManager.h"
#include "DatabaseManager/Work.h"

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
#include <QVector>
#include <QHash>
#include <QJsonArray>
#include <QDesktopServices>

//==================================================================================================================================
//==================================================================================================================================
//==================================================================================================================================

void MangaDexPage::validate() {

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
	token = settings.value("token").toString();
	settings.endGroup();
}

//==================================================================================================================================

MangaDexPage::~MangaDexPage() {
	QSettings settings("settings.ini", QSettings::IniFormat, this);
	settings.beginGroup("MangaDex");
	settings.setValue("token", token);
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
