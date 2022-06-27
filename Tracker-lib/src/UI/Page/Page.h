#pragma once
#include "pch.h"

class Page : public QWidget {
	Q_OBJECT
public:
	Page(QWidget* parent = nullptr) : QWidget(parent) {}
	virtual void populate() = 0;

signals:
	void workClicked(const int id);
	void creatorClicked(const int id);
	void workRemoved(const int id);
	void creatorRemoved(const int id);
};
