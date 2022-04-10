#pragma once
#include <QFrame>

//==================================================================================================================================

QT_BEGIN_NAMESPACE
namespace Ui {
	class UpdateEntry;
}
QT_END_NAMESPACE

//==================================================================================================================================

class UpdateEntry : public QFrame {
	Q_OBJECT
private:
	Ui::UpdateEntry *ui{ nullptr };

public:
	UpdateEntry(QWidget *parent = nullptr);
	~UpdateEntry();
};

//==================================================================================================================================
