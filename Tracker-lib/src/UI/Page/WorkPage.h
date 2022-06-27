#pragma once
#include "pch.h"
#include "Page.h"

QT_BEGIN_NAMESPACE
namespace Ui {
	class WorkPage;
}
QT_END_NAMESPACE

class WorkPage : public Page {
	Q_OBJECT
private:
	Ui::WorkPage* ui{ nullptr };
	int id{};

public:
	WorkPage(const int id, QWidget* parent = nullptr);
	~WorkPage();

	void populate() override;

private slots:
	void on_removeButton_clicked();
	void on_nameLineEdit_textEdited(const QString& text);
	void on_statusComboBox_currentIndexChanged(int);
	void on_typeComboBox_currentIndexChanged(int);
	void on_chapterLineEdit_textEdited(const QString& text);

	void on_addAuthorButton_clicked();
	void on_addArtistButton_clicked();
	void on_authorsListWidget_itemClicked(QListWidgetItem* item);
	void on_artistsListWidget_itemClicked(QListWidgetItem* item);
	void on_authorsListWidget_customContextMenuRequested(const QPoint& pos);
	void on_artistsListWidget_customContextMenuRequested(const QPoint& pos);

	void on_authorSelected(const int author_id, const QString& name);
	void on_artistSelected(const int artist_id, const QString& name);
};
