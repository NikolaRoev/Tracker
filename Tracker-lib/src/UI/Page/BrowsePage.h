#pragma once
#include "pch.h"
#include "Page.h"
#include "DatabaseManager/Work.h"
#include "DatabaseManager/Creator.h"

QT_BEGIN_NAMESPACE
namespace Ui {
	class BrowsePage;
}
QT_END_NAMESPACE

class BrowsePage : public Page {
	Q_OBJECT
private:
	Ui::BrowsePage* ui{ nullptr };

public:
	BrowsePage(QWidget* parent = nullptr);
	~BrowsePage();

	void populate() override;

private slots:
	void on_backToolButton_clicked();
	void on_forwardToolButton_clicked();
	void on_homeToolButton_clicked();

	void on_addWorkToolButton_clicked();
	void on_addCreatorToolButton_clicked();

	void on_stackedWidget_currentChanged(int index);
	void on_lineEdit_textEdited(const QString&);
	void on_whatComboBox_currentIndexChanged(int index);
	void on_statusComboBox_currentIndexChanged(int);
	void on_typeComboBox_currentIndexChanged(int);
	void on_byComboBox_currentIndexChanged(int);
	void on_tableWidget_clicked(const QModelIndex& index);
	void on_tableWidget_customContextMenuRequested(const QPoint& pos);

	void on_workClicked(const int id);
	void on_creatorClicked(const int id);
	void on_workAdded(const Work& work);
	void on_creatorAdded(const Creator& creator);
	void on_workRemoved(const int id);
	void on_creatorRemoved(const int id);
};
