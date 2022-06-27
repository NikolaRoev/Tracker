#pragma once
#include "pch.h"
#include "Page.h"

QT_BEGIN_NAMESPACE
namespace Ui {
	class CreatorPage;
}
QT_END_NAMESPACE

class CreatorPage : public Page {
	Q_OBJECT
private:
	Ui::CreatorPage* ui{ nullptr };
	int id{};

public:
	CreatorPage(const int id, QWidget* parent = nullptr);
	~CreatorPage();

	void populate() override;

private slots:
	void on_removeButton_clicked();
	void on_nameLineEdit_textEdited(const QString& text);
	void on_tableWidget_clicked(const QModelIndex &index);

signals:
	void workClicked(const int id);
	void creatorRemoved(const int id);
};
