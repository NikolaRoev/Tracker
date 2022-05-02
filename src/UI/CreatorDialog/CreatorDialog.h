#pragma once
#include <QDialog>

//==================================================================================================================================

QT_BEGIN_NAMESPACE
namespace Ui {
	class CreatorDialog;
}
QT_END_NAMESPACE

//==================================================================================================================================

class CreatorDialog : public QDialog {
	Q_OBJECT
private:
	Ui::CreatorDialog* ui{ nullptr };

public:
	CreatorDialog(const int id, QWidget* parent = nullptr);
	~CreatorDialog();
};

//==================================================================================================================================
