#pragma once
#include <QWidget>
#include <QString>

//==================================================================================================================================

QT_BEGIN_NAMESPACE
namespace Ui {
	class UpdateEntry;
}
QT_END_NAMESPACE

//==================================================================================================================================

class UpdateEntry : public QWidget {
	Q_OBJECT
private:
	Ui::UpdateEntry* ui{ nullptr };
	int id{};
	QString name;

public:
	UpdateEntry(const QString& name, const QString& chapter, QWidget *parent = nullptr);
	~UpdateEntry();

private slots:
	void on_nameLineEdit_textEdited(const QString& text);
	void on_chapterLineEdit_textEdited(const QString& text);
};

//==================================================================================================================================
