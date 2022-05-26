#pragma once
#include "pch.h"

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
	int id{};

public:
	WorkPage(const int id, QWidget* parent = nullptr);
	~WorkPage();

private slots:
	void on_nameLineEdit_textEdited(const QString& text);
	void on_statusComboBox_currentIndexChanged(int index);
	void on_typeComboBox_currentIndexChanged(int index);
	void on_chapterLineEdit_textEdited(const QString& text);
	void on_mdLineEdit_textEdited(const QString& text);
	void on_muLineEdit_textEdited(const QString& text);

	void on_addPushButton_clicked();
	void on_tableWidget_clicked(const QModelIndex& index);
	void on_tableWidget_customContextMenuRequested(const QPoint& pos);

	void on_creatorSelected(const int creator_id, const QString& name, const QString& type);

signals:
	void creatorClicked(const int id);
};

//==================================================================================================================================
