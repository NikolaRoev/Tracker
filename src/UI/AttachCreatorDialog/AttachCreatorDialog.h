#pragma once
#include <QDialog>
#include <QString>

//==================================================================================================================================

QT_BEGIN_NAMESPACE
namespace Ui {
	class AttachCreatorDialog;
}
QT_END_NAMESPACE

//==================================================================================================================================

class AttachCreatorDialog : public QDialog {
	Q_OBJECT
private:
	Ui::AttachCreatorDialog* ui{ nullptr };
	int work_id{};
	QString type;

public:
	AttachCreatorDialog(const int work_id, const QString& type, QWidget* parent = nullptr);
	~AttachCreatorDialog();

private slots:
	void on_filterLineEdit_textEdited(const QString& text);
	void on_buttonBox_accepted();
};

//==================================================================================================================================
