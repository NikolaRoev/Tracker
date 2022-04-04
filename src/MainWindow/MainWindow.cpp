#include "MainWindow.h"
#include "./ui_MainWindow.h"

#include <QMainWindow>
#include <QSettings>

MainWindow::MainWindow(QWidget *parent) : QMainWindow(parent), ui(new Ui::MainWindow) {
	ui->setupUi(this);
}

MainWindow::~MainWindow() {
	QSettings settings("settings.ini", QSettings::IniFormat, this);
	settings.beginGroup("window");
	settings.setValue("geomety", saveGeometry());
	settings.setValue("state", saveState());
	settings.endGroup();

	delete ui;
}

