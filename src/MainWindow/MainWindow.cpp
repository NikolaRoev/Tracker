#include "MainWindow.h"
#include "./ui_MainWindow.h"
#include "DatabaseManager/DatabaseManager.h"
#include "UpdateEntry/UpdateEntry.h"

#include <QMainWindow>
#include <QSettings>
#include <QShortcut>
#include <QFileDialog>
#include <QString>
#include <QListWidgetItem>
#include <QMessageBox>

//==================================================================================================================================
//==================================================================================================================================
//==================================================================================================================================

MainWindow::MainWindow(QWidget* parent) : QMainWindow(parent), ui(new Ui::MainWindow) {
	ui->setupUi(this);
	DatabaseManager::init();


	//Align the update layout to top so the populated entries look nice.
	ui->updateContentsWidget->layout()->setAlignment(Qt::AlignTop);

	//Set default focus to the search bar.
	//As 'tabWidget' is first in the tab order it gets focused when the application starts.
	ui->tabWidget->setFocusProxy(ui->updateSearchLineEdit);


	//Add a shortcut that selects all text in the search bar and focuses it.
	QShortcut* shortcut = new QShortcut(QKeySequence(Qt::CTRL | Qt::Key_F), this);
	connect(shortcut, &QShortcut::activated, ui->updateSearchLineEdit,
			[&](){ ui->updateSearchLineEdit->selectAll(); ui->updateSearchLineEdit->setFocus(); });


	//Set the various Combo Box Items data.
	//Works, filters.
	ui->worksFilterStatusComboBox->setItemData(0, "Any Status");
	ui->worksFilterStatusComboBox->setItemData(1, "Reading");
	ui->worksFilterStatusComboBox->setItemData(2, "Completed");

	ui->worksFilterTypeComboBox->setItemData(0, "Any Type");
	ui->worksFilterTypeComboBox->setItemData(1, "Series");
	ui->worksFilterTypeComboBox->setItemData(2, "One Shot");
	ui->worksFilterTypeComboBox->setItemData(3, "Anthology");

	ui->worksFilterByComboBox->setItemData(0, "Title");
	ui->worksFilterByComboBox->setItemData(1, "Chapter");
	ui->worksFilterByComboBox->setItemData(2, "Creator");
	ui->worksFilterByComboBox->setItemData(3, "Grouping");

	//Works, selected work.
	ui->statusComboBox->setItemData(0, "Reading");
	ui->statusComboBox->setItemData(1, "Completed");

	ui->typeComboBox->setItemData(0, "Series");
	ui->typeComboBox->setItemData(1, "One Shot");
	ui->typeComboBox->setItemData(2, "Anthology");



	//Load settings.
	QSettings settings("settings.ini", QSettings::IniFormat, this);
	settings.beginGroup("window");
	restoreGeometry(settings.value("geometry").toByteArray());
	restoreState(settings.value("state").toByteArray());
	settings.endGroup();

	settings.beginGroup("user");
	DatabaseManager::open(settings.value("default_database").toString());
	settings.endGroup();


	//Populate the update entries.
	emit ui->updateSearchLineEdit->textChanged("");
}

//==================================================================================================================================

MainWindow::~MainWindow() {
	//Save settings.
	QSettings settings("settings.ini", QSettings::IniFormat, this);
	settings.beginGroup("window");
	settings.setValue("geometry", saveGeometry());
	settings.setValue("state", saveState());
	settings.endGroup();

	settings.beginGroup("user");
	settings.setValue("default_database", DatabaseManager::get());
	settings.endGroup();


	DatabaseManager::deinit();
	delete ui;
}

//==================================================================================================================================
//==================================================================================================================================

void MainWindow::on_actionNew_triggered() {
	QString file = QFileDialog::getSaveFileName(this, "New Database", "", "Databases (*.db)");
	DatabaseManager::open(file);
	emit ui->updateSearchLineEdit->textChanged("");
}

//==================================================================================================================================

void MainWindow::on_actionOpen_triggered() {
	QString file = QFileDialog::getOpenFileName(this, "Open Database", "", "Databases (*.db)");
	DatabaseManager::open(file);
	emit ui->updateSearchLineEdit->textChanged("");
}

//==================================================================================================================================

void MainWindow::on_actionClose_triggered() {
	DatabaseManager::close();
	emit ui->updateSearchLineEdit->textChanged("");
}

//==================================================================================================================================

void MainWindow::on_actionExit_triggered() {
	QApplication::exit();
}

//==================================================================================================================================
//==================================================================================================================================

void MainWindow::on_updateSearchLineEdit_textChanged(const QString& text) {
	//Clear widgets from layout.
	QLayoutItem* child{ nullptr };
	while ((child = ui->updateContentsWidget->layout()->takeAt(0)) != nullptr) {
		delete child->widget();
		delete child;
	}

	//Find works and populate the update list.
	const auto found_works = DatabaseManager::search_update_works(text);
	for (const auto& found_work : found_works) {
		ui->updateContentsWidget->layout()->addWidget(
			new UpdateEntry(found_work, ui->updateScrollArea)
		);
	}

	//Update status bar.
	ui->statusBar->showMessage(QString("Found %1 entries.").arg(found_works.size()));
}

//==================================================================================================================================
//==================================================================================================================================

void MainWindow::on_worksFilterLineEdit_textEdited(const QString& text) {
	qDebug() << text;
}

//==================================================================================================================================

void MainWindow::on_worksFilterStatusComboBox_currentIndexChanged(int index) {
	QString status = ui->worksFilterStatusComboBox->itemData(index).toString();
	qDebug() << status;
}

//==================================================================================================================================

void MainWindow::on_worksFilterTypeComboBox_currentIndexChanged(int index) {
	QString type = ui->worksFilterTypeComboBox->itemData(index).toString();
	qDebug() << type;
}

//==================================================================================================================================

void MainWindow::on_worksFilterByComboBox_currentIndexChanged(int index) {
	QString by = ui->worksFilterByComboBox->itemData(index).toString();
	qDebug() << by;
}

//==================================================================================================================================
//==================================================================================================================================

void MainWindow::on_worksListWidget_itemSelectionChanged() {
	qDebug("me");
}

//==================================================================================================================================

void MainWindow::on_worksListWidget_itemDoubleClicked(QListWidgetItem* item) {
	int result = QMessageBox::warning(this,
									  "Deleting Entry.",
									  QString("Are you sure you want to delete \"%1\"?").arg(item->text()),
									  QMessageBox::Yes,
									  QMessageBox::No);

	if (result == QMessageBox::Yes) {
		delete item;
	}
}

//==================================================================================================================================
//==================================================================================================================================

void MainWindow::on_worksAddButton_clicked() {

}

//==================================================================================================================================

void MainWindow::on_worksNameLineEdit_textEdited(const QString& text) {

}

//==================================================================================================================================

void MainWindow::on_statusComboBox_currentIndexChanged(int index) {
	QString status = ui->statusComboBox->itemData(index).toString();
	qDebug() << status;
}

//==================================================================================================================================

void MainWindow::on_typeComboBox_currentIndexChanged(int index) {
	QString type = ui->typeComboBox->itemData(index).toString();
	qDebug() << type;
}

//==================================================================================================================================

void MainWindow::on_worksGroupingLineEdit_textEdited(const QString& text) {

}

//==================================================================================================================================

void MainWindow::on_worksChapterLineEdit_textEdited(const QString& text) {

}

//==================================================================================================================================

void MainWindow::on_worksAuthorEditButton_clicked() {

}

//==================================================================================================================================

void MainWindow::on_worksArtistEditButton_clicked() {

}

//==================================================================================================================================

void MainWindow::on_worksAuthorListWidget_itemClicked(QListWidgetItem* item) {

}

//==================================================================================================================================

void MainWindow::on_worksArtistListWidget_itemClicked(QListWidgetItem* item) {

}

//==================================================================================================================================
//==================================================================================================================================
//==================================================================================================================================

void MainWindow::on_creatorsFilterLineEdit_textEdited(const QString& text) {

}

//==================================================================================================================================

void MainWindow::on_creatorsListWidget_itemSelectionChanged() {

}

//==================================================================================================================================

void MainWindow::on_creatorsListWidget_itemDoubleClicked(QListWidgetItem* item) {

}

//==================================================================================================================================

void MainWindow::on_creatorsAddButton_clicked() {

}

//==================================================================================================================================

void MainWindow::on_creatorsNameLineEdit_textEdited(const QString& text) {

}

//==================================================================================================================================

void MainWindow::on_creatorsWorksListWidget_itemClicked(QListWidgetItem* item) {

}

//==================================================================================================================================
//==================================================================================================================================
//==================================================================================================================================
