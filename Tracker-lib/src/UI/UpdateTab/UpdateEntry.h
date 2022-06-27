#pragma once
#include "pch.h"
#include "DatabaseManager/Work.h"

QT_BEGIN_NAMESPACE
namespace Ui {
	class UpdateEntry;
}
QT_END_NAMESPACE

class UpdateEntry : public QWidget {
	Q_OBJECT
private:
	Ui::UpdateEntry* ui{ nullptr };
	int id{};

public:
	UpdateEntry(const Work& work, QWidget* parent = nullptr);
	~UpdateEntry();

private slots:
	void on_nameLineEdit_textEdited(const QString& text);
	void on_chapterLineEdit_textEdited(const QString& text);
};
