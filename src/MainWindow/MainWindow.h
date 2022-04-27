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
	//Menu bar.
	void on_actionNew_triggered();
	void on_actionOpen_triggered();
	void on_actionClose_triggered();
	void on_actionExit_triggered();

	//Update tab.
	void on_updateSearchLineEdit_textChanged(const QString& text);

	//Works tab.
	void on_worksFilterLineEdit_textChanged(const QString& text);
	void on_worksFilterStatusComboBox_currentIndexChanged(int index);
	void on_worksFilterTypeComboBox_currentIndexChanged(int index);
	void on_worksFilterByComboBox_currentIndexChanged(int index);

	void on_worksListWidget_itemSelectionChanged();
	void on_worksListWidget_itemDoubleClicked(QListWidgetItem* item);

	void on_worksAddButton_clicked();
	void on_worksNameLineEdit_textEdited(const QString& text);
	void on_worksStatusComboBox_currentIndexChanged(int index);
	void on_worksTypeComboBox_currentIndexChanged(int index);
	void on_worksGroupingLineEdit_textEdited(const QString& text);
	void on_worksChapterLineEdit_textEdited(const QString& text);
	void on_worksAuthorEditButton_clicked();
	void on_worksArtistEditButton_clicked();
	void on_worksAuthorListWidget_itemClicked(QListWidgetItem* item);
	void on_worksArtistListWidget_itemClicked(QListWidgetItem* item);

	//Creator tab.
	void on_creatorsFilterLineEdit_textChanged(const QString& text);
	void on_creatorsListWidget_itemSelectionChanged();
	void on_creatorsListWidget_itemDoubleClicked(QListWidgetItem* item);
	void on_creatorsAddButton_clicked();
	void on_creatorsNameLineEdit_textEdited(const QString& text);
	void on_creatorsWorksListWidget_itemClicked(QListWidgetItem* item);
};

//==================================================================================================================================
