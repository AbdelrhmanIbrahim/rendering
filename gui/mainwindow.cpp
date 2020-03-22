#include "MainWindow.h"
#include "ui_MainWindow.h"

namespace gui
{
	MainWindow::MainWindow(QWidget* parent) :
		QMainWindow(parent),
		ui(new Ui::MainWindow)
	{
		ui->setupUi(this);
		scene = new NativeWindow(win::window_fake_new(200, 200, "scene_1"));
		ui->horizontalLayout->addWidget(QWidget::createWindowContainer(scene, this));
	}

	MainWindow::~MainWindow()
	{
		delete scene;
		delete ui;
	}
};
