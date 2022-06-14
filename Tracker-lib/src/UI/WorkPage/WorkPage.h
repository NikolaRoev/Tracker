#pragma once
#include "pch.h"
#include "DatabaseManager/Work.h"

//==================================================================================================================================

QT_BEGIN_NAMESPACE
namespace Ui {
	class WorkPage;
}
QT_END_NAMESPACE

//==================================================================================================================================

class WorkPage : public QWidget {
	Q_OBJECT
private:
	Ui::WorkPage* ui{ nullptr };
	Work work{};


	bool pending_change();

public:
	WorkPage(const int id, QWidget* parent = nullptr);
	~WorkPage();

private slots:
	void on_nameLineEdit_textEdited(const QString&);
	void on_statusComboBox_currentIndexChanged(int);
	void on_typeComboBox_currentIndexChanged(int);
	void on_chapterLineEdit_textEdited(const QString&);
	void on_mdLineEdit_textEdited(const QString&);
	void on_muLineEdit_textEdited(const QString&);

	void on_applyButton_clicked();

	void on_addPushButton_clicked();
	void on_tableWidget_clicked(const QModelIndex& index);
	void on_tableWidget_customContextMenuRequested(const QPoint& pos);

	void on_creatorSelected(const int creator_id, const QString& name, const QString& type);

signals:
	void creatorClicked(const int id);
};

//==================================================================================================================================
