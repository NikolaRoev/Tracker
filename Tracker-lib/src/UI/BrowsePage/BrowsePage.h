#pragma once
#include "pch.h"
#include "DatabaseManager/Work.h"
#include "DatabaseManager/Creator.h"

//==================================================================================================================================

QT_BEGIN_NAMESPACE
namespace Ui {
	class BrowsePage;
}
QT_END_NAMESPACE

//==================================================================================================================================

class BrowsePage : public QWidget {
	Q_OBJECT
private:
	Ui::BrowsePage* ui{ nullptr };

public:
	BrowsePage(QWidget* parent = nullptr);
	~BrowsePage();

	void populate(const QString& search);

private slots:
	void on_backToolButton_clicked();
	void on_forwardToolButton_clicked();
	void on_homeToolButton_clicked();

	void on_addWorkToolButton_clicked();
	void on_addCreatorToolButton_clicked();

	void on_stackedWidget_currentChanged(int index);
	void on_lineEdit_textEdited(const QString& text);
	void on_whatComboBox_currentIndexChanged(int index);
	void on_statusComboBox_currentIndexChanged(int index);
	void on_typeComboBox_currentIndexChanged(int index);
	void on_byComboBox_currentIndexChanged(int index);
	void on_tableWidget_clicked(const QModelIndex& index);
	void on_tableWidget_customContextMenuRequested(const QPoint& pos);

	void on_workClicked(const int id);
	void on_creatorClicked(const int id);
	void on_workAdded(const Work& work);
	void on_creatorAdded(const Creator& creator);

signals:
	void message(const QString& message, int timeout = 0);
};

//==================================================================================================================================
