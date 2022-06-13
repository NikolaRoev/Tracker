#pragma once
#include <QLineEdit>
#include <QString>

//==================================================================================================================================

class NullLineEdit : public QLineEdit {
	Q_OBJECT
public:
	NullLineEdit(QWidget* parent = nullptr);

	QString text() const;
	void setText(const QString&);
};

//==================================================================================================================================
