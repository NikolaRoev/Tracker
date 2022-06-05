#pragma once
#include "pch.h"

//==================================================================================================================================

class RequestsManager {
private:
	static QNetworkAccessManager* manager;

public:
	static void init(QObject* parent = nullptr);
	static QNetworkAccessManager* get();
};

//==================================================================================================================================
