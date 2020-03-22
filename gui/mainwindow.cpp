#include "MainWindow.h"
#include "ui_MainWindow.h"

namespace gui
{
	MainWindow::MainWindow(QWidget* parent) :
		QMainWindow(parent),
		ui(new Ui::MainWindow)
	{
		ui->setupUi(this);
		ui->horizontalLayout->addWidget(QWidget::createWindowContainer(&scene, this));
	}

	MainWindow::~MainWindow()
	{
		delete ui;
	}
};
