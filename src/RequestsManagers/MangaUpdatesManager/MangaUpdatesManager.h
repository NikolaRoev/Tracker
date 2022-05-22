#pragma once
#include <QObject>
#include <QNetworkAccessManager>

//==================================================================================================================================

class MangaUpdatesManager {
private:
	QNetworkAccessManager* manager{ nullptr };

public:
	MangaUpdatesManager(QObject* parent = nullptr);
	~MangaUpdatesManager();
};

//==================================================================================================================================
