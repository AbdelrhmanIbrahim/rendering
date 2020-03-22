#include "MainWindow.h"
#include "ui_MainWindow.h"

#include "window/Window.h"
#include "gl/gl_context.h"

//using namespace win;

namespace gui
{
	MainWindow::MainWindow(QWidget* parent) :
		QMainWindow(parent),
		ui(new Ui::MainWindow),
		qnative_win(50, 50)
	{
		ui->setupUi(this);
		/*win::Window win = win::window_new(50, 50, "scene");
		QWindow* qwin = QWindow::fromWinId((WId)win::window_handle(win));*/
		//QWidget* qwid =  QWidget::createWindowContainer(&mqwin, this);
		//ui->horizontalLayout->addWidget(qwid);
		//glgpu::context_attach(ctx, dis);
		//gui::NativeWindow* scene_win = new gui::NativeWindow(300, 500);
		/*QWidget* lool = new QWidget;
		lool->create()
		QWidget::create = createWindowContainer(scene_win, this);
		win::Window dis = win::window_new_test((void*)wid->winId(), wid->width(), wid->height(), "");
		auto ctx = glgpu::context_create(4, 0, dis);*/
	}

	MainWindow::~MainWindow()
	{
		delete ui;
	}
};
