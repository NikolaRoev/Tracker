#pragma once
#include <QWidget>

//==================================================================================================================================

QT_BEGIN_NAMESPACE
namespace Ui {
	class MangaUpdatesPage;
}
QT_END_NAMESPACE

//==================================================================================================================================

class MangaUpdatesPage : public QWidget {
	Q_OBJECT
private:
	Ui::MangaUpdatesPage* ui{ nullptr };

public:
	MangaUpdatesPage(QWidget* parent = nullptr);
	~MangaUpdatesPage();
};

//==================================================================================================================================
