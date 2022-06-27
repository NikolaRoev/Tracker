#include "pch.h"
#include "BrowseTab.h"
#include "ui_BrowseTab.h"
#include "DatabaseManager/DatabaseManager.h"
#include "DatabaseManager/Work.h"
#include "DatabaseManager/Creator.h"
#include "Page/Page.h"
#include "Page/HomePage.h"
#include "Page/WorkPage.h"
#include "Page/CreatorPage.h"
#include "Dialog/AddWorkDialog.h"
#include "Dialog/AddCreatorDialog.h"

BrowseTab::BrowseTab(QWidget* parent) : QWidget(parent), ui(new Ui::BrowseTab) {
	ui->setupUi(this);

	HomePage* home_page = dynamic_cast<HomePage*>(ui->stackedWidget->widget(0));
	connect(home_page, &HomePage::workClicked, this, &BrowseTab::on_workClicked);
	connect(home_page, &HomePage::creatorClicked, this, &BrowseTab::on_creatorClicked);
	connect(home_page, &HomePage::workRemoved, this, &BrowseTab::on_workRemoved);
	connect(home_page, &HomePage::creatorRemoved, this, &BrowseTab::on_creatorRemoved);
}

BrowseTab::~BrowseTab() {
	delete ui;
}

void BrowseTab::populate() {
	on_stackedWidget_currentChanged(ui->stackedWidget->currentIndex());
}

void BrowseTab::on_backToolButton_clicked() {
	int previous = ui->stackedWidget->currentIndex() - 1;

	if (previous >= 0) {
		ui->stackedWidget->setCurrentIndex(previous);
	}
}

void BrowseTab::on_forwardToolButton_clicked() {
	int current = ui->stackedWidget->currentIndex();
	int last = ui->stackedWidget->count() - 1;

	if (current < last) {
		ui->stackedWidget->setCurrentIndex(current + 1);
	}
}

void BrowseTab::on_homeToolButton_clicked() {
	ui->stackedWidget->setCurrentIndex(0);
}

void BrowseTab::on_addWorkToolButton_clicked() {
	AddWorkDialog* dialog = new AddWorkDialog(this);
	connect(dialog, &AddWorkDialog::workAdded, this, &BrowseTab::on_workAdded);
	dialog->exec();
}

void BrowseTab::on_addCreatorToolButton_clicked() {
	AddCreatorDialog* dialog = new AddCreatorDialog(this);
	connect(dialog, &AddCreatorDialog::creatorAdded, this, &BrowseTab::on_creatorAdded);
	dialog->exec();
}

void BrowseTab::on_stackedWidget_currentChanged(int index) {
	//Enable the Home and Back buttons if we are not on the Home Page.
	ui->backToolButton->setEnabled(index);
	ui->homeToolButton->setEnabled(index);

	//Enable the forward button if we are not on the last Page.
	int last = ui->stackedWidget->count() - 1;
	ui->forwardToolButton->setEnabled(index != last);

	dynamic_cast<Page*>(ui->stackedWidget->widget(index))->populate();
}

void BrowseTab::on_workAdded(const Work& work) {
	if (DatabaseManager::add_work(work)) {
		populate();
	}
	else {
		QMessageBox::warning(this, "Database Error", "Failed to add Work.");
	}
}

void BrowseTab::on_creatorAdded(const Creator& creator) {
	if (DatabaseManager::add_creator(creator)) {
		populate();
	}
	else {
		QMessageBox::warning(this, "Database Error", "Failed to add Creator.");
	}
}

void BrowseTab::on_workRemoved(const int id) {
	if (DatabaseManager::remove_work(id)) {
		while (ui->stackedWidget->count() > 1) {
			QWidget* widget = ui->stackedWidget->widget(ui->stackedWidget->count() - 1);
			ui->stackedWidget->removeWidget(widget);
			delete widget;
		}

		populate();
	}
	else {
		QMessageBox::warning(this, "Database Error", "Failed to remove Work.");
	}
}

void BrowseTab::on_creatorRemoved(const int id) {
	if (DatabaseManager::remove_creator(id)) {
		while (ui->stackedWidget->count() > 1) {
			QWidget* widget = ui->stackedWidget->widget(ui->stackedWidget->count() - 1);
			ui->stackedWidget->removeWidget(widget);
			delete widget;
		}

		populate();
	}
	else {
		QMessageBox::warning(this, "Database Error", "Failed to remove Creator.");
	}
}

void BrowseTab::on_workClicked(const int id) {
	int last = ui->stackedWidget->count() - 1;
	int current = ui->stackedWidget->currentIndex();

	if (current < last) {
		for (int i = last; i > current; --i) {
			QWidget* widget = ui->stackedWidget->widget(i);
			ui->stackedWidget->removeWidget(widget);
			delete widget;
		}
	}


	WorkPage* work_page = new WorkPage(id);
	connect(work_page, &WorkPage::creatorClicked, this, &BrowseTab::on_creatorClicked);
	connect(work_page, &WorkPage::workRemoved, this, &BrowseTab::on_workRemoved);

	ui->stackedWidget->addWidget(work_page);
	ui->stackedWidget->setCurrentIndex(ui->stackedWidget->count() - 1);
}

void BrowseTab::on_creatorClicked(const int id) {
	int last = ui->stackedWidget->count() - 1;
	int current = ui->stackedWidget->currentIndex();

	if (current < last) {
		for (int i = last; i > current; --i) {
			QWidget* widget = ui->stackedWidget->widget(i);
			ui->stackedWidget->removeWidget(widget);
			delete widget;
		}
	}


	CreatorPage* creator_page = new CreatorPage(id);
	connect(creator_page, &CreatorPage::workClicked, this, &BrowseTab::on_workClicked);
	connect(creator_page, &CreatorPage::creatorRemoved, this, &BrowseTab::on_creatorRemoved);

	ui->stackedWidget->addWidget(creator_page);
	ui->stackedWidget->setCurrentIndex(ui->stackedWidget->count() - 1);
}
