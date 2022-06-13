#include "pch.h"
#include "ReleasesPage.h"
#include "./ui_ReleasesPage.h"


ReleasesPage::ReleasesPage(QWidget* parent) : QWidget(parent), ui(new Ui::ReleasesPage) {
	ui->setupUi(this);

}

ReleasesPage::~ReleasesPage() {
	delete ui;
}

/*
//==================================================================================================================================
//==================================================================================================================================
//==================================================================================================================================

void MangaUpdatesPage::validate() {
	if (!token.isEmpty()) {
		QNetworkRequest request(QUrl("https://api.mangaupdates.com/v1/account/profile"));
		request.setRawHeader("Authorization", token.toUtf8());
		QNetworkReply* reply = RequestsManager::get(request);


		QObject::connect(reply, &QNetworkReply::finished, this, [this, reply](){
			QByteArray contents = reply->readAll();
			QJsonObject object = QJsonDocument::fromJson(contents).object();


			if (reply->error() == QNetworkReply::NoError) {
				emit login(object["username"].toString());
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

	connect(this, &MangaUpdatesPage::login, this, &MangaUpdatesPage::on_login);
	connect(this, &MangaUpdatesPage::logout, this, &MangaUpdatesPage::on_logout);

	//Set resize mode for the Table Widget.
	ui->tableWidget->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
	ui->tableWidget->horizontalHeader()->setDefaultAlignment(Qt::AlignLeft);
	ui->tableWidget->verticalHeader()->setSectionResizeMode(QHeaderView::ResizeToContents);

	//Load settings.
	QSettings settings("settings.ini", QSettings::IniFormat, this);
	settings.beginGroup("MangaUpdates");
	token = settings.value("token").toString();
	settings.endGroup();

	validate();
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


	QNetworkRequest request(QUrl("https://api.mangaupdates.com/v1/account/login"));
	request.setHeader(QNetworkRequest::ContentTypeHeader, "application/json");
	QJsonObject data_object;
	data_object["username"] = ui->usernameLineEdit->text();
	data_object["password"] = ui->passwordLineEdit->text();
	QNetworkReply* reply = RequestsManager::put(request, QJsonDocument(data_object).toJson());


	QObject::connect(reply, &QNetworkReply::finished, this, [this, reply](){
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
	QNetworkReply* reply = RequestsManager::post(request);


	QObject::connect(reply, &QNetworkReply::finished, this, [this, reply](){
		QByteArray contents = reply->readAll();
		QJsonObject object = QJsonDocument::fromJson(contents).object();


		if (reply->error() == QNetworkReply::NoError) {
			token = NULL;

			emit logout();
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

void MangaUpdatesPage::on_openButton_clicked() {
	QModelIndexList indexes = ui->tableWidget->selectionModel()->selectedRows();

	for (const auto& index : indexes) {
		if (QVariant user_data = index.data(Qt::UserRole); user_data.isValid()) {
			QString link = QString("https://www.mangaupdates.com/series/%1").arg(user_data.toString());
			if (!QDesktopServices::openUrl(QUrl(link))) {
				qDebug() << QString("Failed to open MangaUpdates link [%1].").arg(link);
				QMessageBox::warning(this, "Failed to open MangaUpdates link.", link);
			}
		}
	}
}

//==================================================================================================================================

void MangaUpdatesPage::on_getButton_clicked() {
	ui->tableWidget->setRowCount(0);
	ui->getButton->setDisabled(true);


	QNetworkRequest request(QUrl("https://api.mangaupdates.com/v1/releases/days?include_metadata=true"));
	request.setRawHeader("Authorization", token.toUtf8());
	QNetworkReply* reply = RequestsManager::get(request);


	QObject::connect(reply, &QNetworkReply::finished, this, [this, reply](){
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

void MangaUpdatesPage::on_tableWidget_doubleClicked(const QModelIndex& index) {
	if (QVariant user_data = ui->tableWidget->item(index.row(), 0)->data(Qt::UserRole); user_data.isValid()) {
		QString link = QString("https://www.mangaupdates.com/series/%1").arg(user_data.toString());
		if (!QDesktopServices::openUrl(QUrl(link))) {
			qDebug() << QString("Failed to open MangaUpdates link [%1].").arg(link);
			QMessageBox::warning(this, "Failed to open MangaUpdates link.", link);
		}
	}
}

//==================================================================================================================================
//==================================================================================================================================

void MangaUpdatesPage::on_login(const QString& username) {
	ui->stackedWidget->setCurrentIndex(1);
	ui->usernameLabel->setText(username);
	ui->openButton->setEnabled(true);
	ui->getButton->setEnabled(true);
}

//==================================================================================================================================

void MangaUpdatesPage::on_logout() {
	ui->stackedWidget->setCurrentIndex(0);
	ui->openButton->setDisabled(true);
	ui->getButton->setDisabled(true);
}

//==================================================================================================================================
//==================================================================================================================================
//==================================================================================================================================

//==================================================================================================================================
//==================================================================================================================================
//==================================================================================================================================



 * 	QJsonDocument jdocHeader = QJsonDocument::fromJson( QByteArray::fromBase64( QString("eyJ0eXAiOiJyZWZyZXNoIiwiaXNzIjoibWFuZ2FkZXgub3JnIiwiYXVkIjoibWFuZ2FkZXgub3JnIiwiaWF0IjoxNjUzNjY1NTA0LCJuYmYiOjE2NTM2NjU1MDQsImV4cCI6MTY1NjI1NzUwNCwidWlkIjoiYTJhMTIxZWUtZWE1My00ZTRhLTg0MTEtM2UzOTM4MzdmYjI5Iiwic2lkIjoiODUwOTAyYmMtYTEzNi00MmM4LWEzMDgtMDMyZWQ5OGQxMDBhIn0").toUtf8() ) );
	QJsonObject jobjHeader = jdocHeader.object();
	qDebug() << jobjHeader;
	QString szFull=QDateTime::fromSecsSinceEpoch(jobjHeader["exp"].toInteger()).toString("dddd d MMMM yyyy hh:mm:ss");
	qDebug() << szFull;
 *


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
*/

