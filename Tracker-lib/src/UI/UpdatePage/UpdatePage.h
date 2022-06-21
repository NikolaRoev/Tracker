#pragma once
#include "pch.h"

QT_BEGIN_NAMESPACE
namespace Ui {
	class UpdatePage;
}
QT_END_NAMESPACE

class UpdatePage : public QWidget {
	Q_OBJECT
private:
	Ui::UpdatePage* ui{ nullptr };

public:
	UpdatePage(QWidget* parent = nullptr);
	~UpdatePage();

	void populate(const QString& search);

private slots:
	void on_lineEdit_textEdited(const QString& text);
};
