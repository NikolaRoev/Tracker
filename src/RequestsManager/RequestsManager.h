#pragma once
#include <QObject>
#include <QNetworkAccessManager>

//==================================================================================================================================

class RequestsManager {
private:
	QNetworkAccessManager* manager{ nullptr };

public:
	RequestsManager(QObject* parent = nullptr);
};

//==================================================================================================================================
