#pragma once
#include "pch.h"

//==================================================================================================================================

QT_BEGIN_NAMESPACE
namespace Ui {
	class ReleasesPage;
}
QT_END_NAMESPACE

//==================================================================================================================================

class ReleasesPage : public QWidget {
	Q_OBJECT
private:
	Ui::ReleasesPage* ui{ nullptr };

public:
	ReleasesPage(QWidget* parent = nullptr);
	~ReleasesPage();
};

//==================================================================================================================================
