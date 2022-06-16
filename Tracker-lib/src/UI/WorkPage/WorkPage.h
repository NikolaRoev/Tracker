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
	void on_removeButton_clicked();
	void on_nameLineEdit_textEdited(const QString& text);
	void on_statusComboBox_currentIndexChanged(int index);
	void on_typeComboBox_currentIndexChanged(int index);
	void on_chapterLineEdit_textEdited(const QString& text);

	void on_addAuthorButton_clicked();
	void on_addArtistButton_clicked();
	void on_authorsListWidget_itemClicked(QListWidgetItem* item);
	void on_artistsListWidget_itemClicked(QListWidgetItem* item);
	void on_authorsListWidget_customContextMenuRequested(const QPoint& pos);
	void on_artistsListWidget_customContextMenuRequested(const QPoint& pos);

	void on_authorSelected(const int author_id, const QString& name);
	void on_artistSelected(const int artist_id, const QString& name);

signals:
	void creatorClicked(const int id);
	void workRemoved(const int id);
};

//==================================================================================================================================
