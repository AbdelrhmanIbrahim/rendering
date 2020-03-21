#include "MainWindow.h"
#include "ui_MainWindow.h"

#include "NativeWindow.h"
#include "window/Window.h"
#include "gl/gl_context.h"

namespace gui
{
    MainWindow::MainWindow(QWidget* parent) :
        QMainWindow(parent),
        ui(new Ui::MainWindow)
    {
        ui->setupUi(this);
        //gui::NativeWindow* scene_win = new gui::NativeWindow(300, 500);
        QWidget* lool = new QWidget;
        lool->create()
        QWidget::create = createWindowContainer(scene_win, this);
        win::Window dis = win::window_new_test((void*)wid->winId(), wid->width(), wid->height(), "");
        auto ctx = glgpu::context_create(4, 0, dis);
        //glgpu::context_attach(ctx, dis);
        //ui->horizontalLayout->addWidget(wid);
    }

    MainWindow::~MainWindow()
    {
        delete ui;
    }
};
