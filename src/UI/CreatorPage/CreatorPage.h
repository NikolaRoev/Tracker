#pragma once
#include <QDialog>

//==================================================================================================================================

QT_BEGIN_NAMESPACE
namespace Ui {
	class CreatorPage;
}
QT_END_NAMESPACE

//==================================================================================================================================

class CreatorDialog : public QDialog {
	Q_OBJECT
private:
	Ui::CreatorPage* ui{ nullptr };

public:
	CreatorDialog(const int id, QWidget* parent = nullptr);
	~CreatorDialog();
};

//==================================================================================================================================
