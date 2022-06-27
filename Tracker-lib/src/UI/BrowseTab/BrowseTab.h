#pragma once
#include "pch.h"
#include "DatabaseManager/Work.h"
#include "DatabaseManager/Creator.h"

QT_BEGIN_NAMESPACE
namespace Ui {
	class BrowseTab;
}
QT_END_NAMESPACE

class BrowseTab : public QWidget {
	Q_OBJECT
private:
	Ui::BrowseTab* ui{ nullptr };

public:
	BrowseTab(QWidget* parent = nullptr);
	~BrowseTab();

	void populate();

private slots:
	void on_backToolButton_clicked();
	void on_forwardToolButton_clicked();
	void on_homeToolButton_clicked();
	void on_addWorkToolButton_clicked();
	void on_addCreatorToolButton_clicked();
	void on_stackedWidget_currentChanged(int index);

	void on_workAdded(const Work& work);
	void on_creatorAdded(const Creator& creator);
	void on_workRemoved(const int id);
	void on_creatorRemoved(const int id);
	void on_workClicked(const int id);
	void on_creatorClicked(const int id);
};
