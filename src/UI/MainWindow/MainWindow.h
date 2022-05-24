#pragma once
#include <QWidget>
#include <QMainWindow>
#include <QNetworkAccessManager>

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
	QNetworkAccessManager* network_access_manager{ nullptr };

public:
	MainWindow(QWidget* parent = nullptr);
	~MainWindow();

private slots:
	//Menu Bar.
	//File.
	void on_actionExit_triggered();
	//Edit.
	void on_actionAdd_Work_triggered();
	void on_actionAdd_Creator_triggered();

	//Main window.
	//Selection.
	void on_listWidget_currentRowChanged(int currentRow);
};

//==================================================================================================================================
