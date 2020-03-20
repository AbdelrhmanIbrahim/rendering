#include "MainWindow.h"
#include "ui_MainWindow.h"

namespace gui
{
    MainWindow::MainWindow(QWidget* parent) :
        QMainWindow(parent),
        ui(new Ui::MainWindow)
    {
        ui->setupUi(this);
        void* win = (void*)ui->scene->winId();
    }

    MainWindow::~MainWindow()
    {
        delete ui;
    }
};
