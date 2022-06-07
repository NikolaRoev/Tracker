#pragma once
#include "pch.h"

//==================================================================================================================================

class RequestsManager {
private:
	static QNetworkAccessManager* manager;

public:
	static void init(QObject* parent = nullptr);

	static QNetworkReply* get(const QNetworkRequest& request);
	static QNetworkReply* post(const QNetworkRequest& request, const QByteArray& data = NULL);
	static QNetworkReply* put(const QNetworkRequest& request, const QByteArray& data = NULL);
};

//==================================================================================================================================
