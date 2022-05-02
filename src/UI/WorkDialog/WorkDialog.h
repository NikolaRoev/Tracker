#pragma once
#include <QDialog>

//==================================================================================================================================

QT_BEGIN_NAMESPACE
namespace Ui {
	class WorkDialog;
}
QT_END_NAMESPACE

//==================================================================================================================================

class WorkDialog : public QDialog {
	Q_OBJECT
private:
	Ui::WorkDialog* ui{ nullptr };

public:
	WorkDialog(const int id, QWidget* parent = nullptr);
	~WorkDialog();
};

//==================================================================================================================================
