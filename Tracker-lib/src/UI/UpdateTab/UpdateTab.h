#pragma once
#include "pch.h"

QT_BEGIN_NAMESPACE
namespace Ui {
	class UpdateTab;
}
QT_END_NAMESPACE

class UpdateTab : public QWidget {
	Q_OBJECT
private:
	Ui::UpdateTab* ui{ nullptr };

public:
	UpdateTab(QWidget* parent = nullptr);
	~UpdateTab();

	void populate();

private slots:
	void on_lineEdit_textEdited(const QString&);
};
