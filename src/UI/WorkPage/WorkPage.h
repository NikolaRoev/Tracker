#pragma once
#include <QWidget>
#include <QListWidgetItem>

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
	void on_nameLineEdit_textChanged(const QString& text);
	void on_statusComboBox_currentIndexChanged(int index);
	void on_typeComboBox_currentIndexChanged(int index);
	void on_groupingLineEdit_textChanged(const QString& text);
	void on_chapterLineEdit_textChanged(const QString& text);

	void on_authorPushButton_clicked();
	void on_artistPushButton_clicked();
	void on_authorListWidget_itemClicked(QListWidgetItem* item);
	void on_artistListWidget_itemClicked(QListWidgetItem* item);
	void on_authorListWidget_customContextMenuRequested(const QPoint& pos);
	void on_artistListWidget_customContextMenuRequested(const QPoint& pos);

	void on_authorSelected(const int creator_id, const QString& name);
	void on_artistSelected(const int creator_id, const QString& name);

signals:
	void creatorClicked(const int id);
};

//==================================================================================================================================
