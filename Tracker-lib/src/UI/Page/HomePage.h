#pragma once
#include "pch.h"
#include "Page.h"
#include "DatabaseManager/Work.h"
#include "DatabaseManager/Creator.h"

QT_BEGIN_NAMESPACE
namespace Ui {
	class HomePage;
}
QT_END_NAMESPACE

class HomePage : public Page {
	Q_OBJECT
private:
	Ui::HomePage* ui{ nullptr };
	int id{};

public:
	HomePage(QWidget* parent = nullptr);
	~HomePage();

	void populate() override;

private slots:
	void on_lineEdit_textEdited(const QString&);
	void on_whatComboBox_currentIndexChanged(int index);
	void on_statusComboBox_currentIndexChanged(int);
	void on_typeComboBox_currentIndexChanged(int);
	void on_byComboBox_currentIndexChanged(int);
	void on_tableWidget_clicked(const QModelIndex& index);
	void on_tableWidget_customContextMenuRequested(const QPoint& pos);
};
