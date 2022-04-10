#pragma once
#include <QFrame>
#include <QString>

//==================================================================================================================================

QT_BEGIN_NAMESPACE
namespace Ui {
	class UpdateEntry;
}
QT_END_NAMESPACE

//==================================================================================================================================

class UpdateEntry : public QFrame {
	Q_OBJECT
private:
	Ui::UpdateEntry *ui{ nullptr };
	QString name;

public:
	UpdateEntry(const QString& name, const QString& chapter, QWidget *parent = nullptr);
	~UpdateEntry();

private slots:
	void on_nameLineEdit_textChanged(const QString& text);
	void on_chapterLineEdit_textChanged(const QString& text);
};

//==================================================================================================================================
