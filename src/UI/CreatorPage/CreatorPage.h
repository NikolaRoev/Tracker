#pragma once
#include <QWidget>
#include <QTableWidgetItem>

//==================================================================================================================================

QT_BEGIN_NAMESPACE
namespace Ui {
	class CreatorPage;
}
QT_END_NAMESPACE

//==================================================================================================================================

class CreatorPage : public QWidget {
	Q_OBJECT
private:
	Ui::CreatorPage* ui{ nullptr };
	int id{};

public:
	CreatorPage(const int id, QWidget* parent = nullptr);
	~CreatorPage();

private slots:
	void on_lineEdit_textChanged(const QString& text);
	void on_tableWidget_clicked(const QModelIndex &index);

signals:
	void workClicked(const int id);
};

//==================================================================================================================================
