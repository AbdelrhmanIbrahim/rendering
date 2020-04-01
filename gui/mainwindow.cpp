#include "MainWindow.h"
#include "ui_MainWindow.h"

#include <QResource>

#include "defs/Defs.h"

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
		//QResource::registerResource("ui.rcc");
	}

	void
	MainWindow::phong_render_mode()
	{
		app::painter_style(picasso, Rendering::PHONG);
	}

	void
	MainWindow::pbr_render_mode()
	{
		app::painter_style(picasso, Rendering::PBR);
	}

	MainWindow::~MainWindow()
	{
		delete ui;
	}
};
