#pragma once
#include "pch.h"

//==================================================================================================================================

QT_BEGIN_NAMESPACE
namespace Ui {
	class ReleaseEntry;
}
QT_END_NAMESPACE

//==================================================================================================================================

class ReleaseEntry : public QWidget {
	Q_OBJECT
private:
	Ui::ReleaseEntry* ui{ nullptr };

public:
	ReleaseEntry(QWidget* parent = nullptr);
	~ReleaseEntry();
};

//==================================================================================================================================
