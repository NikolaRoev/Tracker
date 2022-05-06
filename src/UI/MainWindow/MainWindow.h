#pragma once
#include <QMainWindow>
#include <QWidget>
#include <QString>
#include <QTableWidgetItem>

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
	//Menu bar.
	//File.
	void on_actionNew_triggered();
	void on_actionOpen_triggered();
	void on_actionClose_triggered();
	void on_actionExit_triggered();
	//Edit.
	void on_actionAdd_Work_triggered();
	void on_actionAdd_Creator_triggered();

	//Tool bar.
	void on_actionBack_triggered();
	void on_actionForward_triggered();
	void on_actionHome_triggered();

	//Stacked widget.
	void on_stackedWidget_currentChanged(int index);

	//Tab widget.
	void on_tabWidget_currentChanged(int index);
	//Update tab.
	void on_updateLineEdit_textEdited(const QString& text);
	//Browse tab.
	void on_browseLineEdit_textEdited(const QString& text);
	void on_whatComboBox_currentIndexChanged(int index);
	void on_statusComboBox_currentIndexChanged(int index);
	void on_typeComboBox_currentIndexChanged(int index);
	void on_byComboBox_currentIndexChanged(int index);
	void on_browseTableWidget_clicked(const QModelIndex& index);
	void on_browseTableWidget_customContextMenuRequested(const QPoint& pos);

	//Custom.
	void on_workClicked(const int id);
	void on_creatorClicked(const int id);
};

//==================================================================================================================================
