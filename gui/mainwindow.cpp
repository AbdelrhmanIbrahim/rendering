#include "MainWindow.h"
#include "ui_MainWindow.h"

namespace gui
{
	MainWindow::MainWindow(QWidget* parent) :
		QMainWindow(parent),
		ui(new Ui::MainWindow),
		picasso(app::painter_new()),
		palette(picasso)
	{
		ui->setupUi(this);
		ui->horizontalLayout->addWidget(QWidget::createWindowContainer(&palette, this));
	}

	MainWindow::~MainWindow()
	{
		delete ui;
	}
};
