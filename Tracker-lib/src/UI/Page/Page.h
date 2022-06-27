#pragma once
#include "pch.h"

class Page : public QWidget {
	Q_OBJECT
public:
	Page(QWidget* parent = nullptr) : QWidget(parent) {}
	virtual void populate() = 0;
};
