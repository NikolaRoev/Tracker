#include "pch.h"
#include "MangaUpdatesPage.h"
#include "./ui_MangaUpdatesPage.h"
#include "DatabaseManager/DatabaseManager.h"
#include "DatabaseManager/Work.h"

//==================================================================================================================================
//==================================================================================================================================
//==================================================================================================================================

void MangaUpdatesPage::validate() {
	if (!token.isEmpty()) {
		QNetworkRequest request(QUrl("https://api.mangaupdates.com/v1/account/profile"));
		request.setRawHeader("Authorization", token.toUtf8());


		QNetworkReply* reply = network_access_manager->get(request);

		QObject::connect(reply, &QNetworkReply::finished, this, [=](){
			QByteArray contents = reply->readAll();
			QJsonObject object = QJsonDocument::fromJson(contents).object();


			if (reply->error() == QNetworkReply::NoError) {
				ui->usernameLabel->setText(object["username"].toString());
				ui->stackedWidget->setCurrentIndex(1);
				ui->getButton->setEnabled(true);
			}
			else {
				qDebug() << QString("Error [%1][%2]: %3.")
								.arg(reply->attribute(QNetworkRequest::HttpStatusCodeAttribute).toInt())
								.arg(reply->error())
								.arg(contents);

				QMessageBox::warning(this, "Failed to validate MangaUpdates token.", object["reason"].toString());
			}


			reply->deleteLater();
		});
	}
}

//==================================================================================================================================
//==================================================================================================================================

MangaUpdatesPage::MangaUpdatesPage(QWidget* parent) : QWidget(parent), ui(new Ui::MangaUpdatesPage) {
	ui->setupUi(this);

	//Set resize mode for the Table Widget.
	ui->tableWidget->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
	ui->tableWidget->horizontalHeader()->setDefaultAlignment(Qt::AlignLeft);
	ui->tableWidget->verticalHeader()->setSectionResizeMode(QHeaderView::ResizeToContents);

	//Load settings.
	QSettings settings("settings.ini", QSettings::IniFormat, this);
	settings.beginGroup("MangaUpdates");
	token = settings.value("token").toString();
	settings.endGroup();
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

void MangaUpdatesPage::setup(QNetworkAccessManager* network_access_manager) {
	this->network_access_manager = network_access_manager;

	validate();
}

//==================================================================================================================================
//==================================================================================================================================
//==================================================================================================================================

void MangaUpdatesPage::on_loginButton_clicked() {
	ui->loginButton->setDisabled(true);


	QNetworkRequest request(QUrl("https://api.mangaupdates.com/v1/account/login"));
	request.setHeader(QNetworkRequest::ContentTypeHeader, "application/json");

	QJsonObject data_object;
	data_object["username"] = ui->usernameLineEdit->text();
	data_object["password"] = ui->passwordLineEdit->text();


	QNetworkReply* reply = network_access_manager->put(request, QJsonDocument(data_object).toJson());

	QObject::connect(reply, &QNetworkReply::finished, this, [=](){
		QByteArray contents = reply->readAll();
		QJsonObject reply_object = QJsonDocument::fromJson(contents).object();


		if (reply->error() == QNetworkReply::NoError) {
			token = reply_object["context"].toObject()["session_token"].toString();

			validate();
		}
		else {
			qDebug() << QString("Error [%1][%2]: %3.")
							.arg(reply->attribute(QNetworkRequest::HttpStatusCodeAttribute).toInt())
							.arg(reply->error())
							.arg(contents);

			QMessageBox::warning(this, "Failed to log in.", reply_object["reason"].toString());
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
		QJsonObject object = QJsonDocument::fromJson(contents).object();


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

			QMessageBox::warning(this, "Failed to log out.", object["reason"].toString());
		}


		ui->logoutButton->setEnabled(true);
		reply->deleteLater();
	});
}

//==================================================================================================================================

void MangaUpdatesPage::on_getButton_clicked() {
	//Clear table items.
	ui->tableWidget->setRowCount(0);

	ui->getButton->setDisabled(true);


	QNetworkRequest request(QUrl("https://api.mangaupdates.com/v1/releases/days?include_metadata=true"));
	request.setRawHeader("Authorization", token.toUtf8());


	QNetworkReply* reply = network_access_manager->get(request);

	QObject::connect(reply, &QNetworkReply::finished, this, [=](){
		QByteArray contents = reply->readAll();
		QJsonObject object = QJsonDocument::fromJson(contents).object();


		if (reply->error() == QNetworkReply::NoError) {
			QList<Work> database_works;
			DatabaseManager::search_works(database_works, "", "name", NULL, NULL);

			QHash<QString, Work> table;
			for (const auto& work : database_works) {
				table.emplace(work.mu_id, work);
			}


			QJsonArray releases = object["results"].toArray();
			for (const auto& release : releases) {
				QJsonObject record = release.toObject()["record"].toObject();
				QJsonObject metadata = release.toObject()["metadata"].toObject();
				QJsonObject series = metadata["series"].toObject();
				QJsonObject user_list = metadata["user_list"].toObject();

				//TO DO: Add a check for last time we ran this, so we can stop.

				QString id = QString::number(series["series_id"].toInteger(), 36);
				if (table.contains(id)) {
					ui->tableWidget->insertRow(ui->tableWidget->rowCount());

					QTableWidgetItem* name_item = new QTableWidgetItem(series["title"].toString());
					name_item->setData(Qt::UserRole, id);
					ui->tableWidget->setItem(ui->tableWidget->rowCount() - 1, 0, name_item);

					ui->tableWidget->setItem(ui->tableWidget->rowCount() - 1, 1,
						new QTableWidgetItem(QString("%1 (%2)").arg(table[id].status, user_list["list_type"].toString())));
					ui->tableWidget->setItem(ui->tableWidget->rowCount() - 1, 2,
						new QTableWidgetItem(QString("v.%1 c.%2").arg(record["volume"].toString(), record["chapter"].toString())));
					ui->tableWidget->setItem(ui->tableWidget->rowCount() - 1, 3, new QTableWidgetItem(table[id].chapter));
				}
			}
		}
		else {
			qDebug() << QString("Error [%1][%2]: %3.")
							.arg(reply->attribute(QNetworkRequest::HttpStatusCodeAttribute).toInt())
							.arg(reply->error())
							.arg(contents);

			QMessageBox::warning(this, "Failed to get releases.", object["reason"].toString());
		}


		ui->getButton->setEnabled(true);
		reply->deleteLater();
	});
}

//==================================================================================================================================

void MangaUpdatesPage::on_tableWidget_clicked(const QModelIndex& index) {
	if (QVariant data = ui->tableWidget->item(index.row(), 0)->data(Qt::UserRole); data.isValid()) {
		QString link = QString("https://www.mangaupdates.com/series/%1").arg(data.toString());
		if (!QDesktopServices::openUrl(QUrl(link))) {
			qDebug() << QString("Failed to open MangaUpdates link [%1].").arg(link);
			QMessageBox::warning(this, "Failed to open MangaUpdates link.", link);
		}
	}
}

//==================================================================================================================================
//==================================================================================================================================
//==================================================================================================================================
