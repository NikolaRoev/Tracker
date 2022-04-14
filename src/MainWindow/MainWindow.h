#pragma once
#include <QMainWindow>
#include <QWidget>
#include <QString>
#include <QListWidgetItem>

//==================================================================================================================================

QT_BEGIN_NAMESPACE
namespace Ui {
	class MainWindow;
}
QT_END_NAMESPACE

//==================================================================================================================================

class MainWindow : public QMainWindow {
	Q_OBJECT
private:
	Ui::MainWindow *ui{ nullptr };

public:
	MainWindow(QWidget* parent = nullptr);
	~MainWindow();

private slots:
	void on_actionNew_triggered();
	void on_actionOpen_triggered();
	void on_actionClose_triggered();
	void on_actionExit_triggered();

	void on_updateSearchLineEdit_textChanged(const QString& text);
	void on_listsListWidget_itemSelectionChanged();
	void on_listsListWidget_itemDoubleClicked(QListWidgetItem* item);
	void on_statusListsComboBox_currentIndexChanged(int index);
	void on_statusSelectListsComboBox_currentIndexChanged(int index);
	void on_typeSelectListsComboBox_currentIndexChanged(int index);
};

//==================================================================================================================================
