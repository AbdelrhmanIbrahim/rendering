#include "MainWindow.h"
#include "ui_MainWindow.h"

#include <QFileDialog>
#include <QMessageBox>

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
	}

	bool
	MainWindow::stl_load()
	{
		QString path = QFileDialog::getOpenFileName(this, "Select STL file", QDir::homePath());
		if (path.isEmpty())
			return false;

		bool result = app::painter_stl_load(picasso, path.toStdString().c_str());
		if (result == false)
			QMessageBox::warning(nullptr, QString("Can't load STL"), QString("file is corrupted."), QMessageBox::StandardButton::Ok);
		else
			palette.requestUpdate();
		
		return result;
	}

	void
	MainWindow::phong_render_mode()
	{
		app::painter_style(picasso, Rendering::PHONG);
		palette.requestUpdate();
	}

	void
	MainWindow::pbr_render_mode()
	{
		app::painter_style(picasso, Rendering::PBR);
		palette.requestUpdate();
	}

	void
	MainWindow::colored_render_mode()
	{
		app::painter_style(picasso, Rendering::COLORED);
		palette.requestUpdate();
	}

	MainWindow::~MainWindow()
	{
		delete ui;
	}
};
