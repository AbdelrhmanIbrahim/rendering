#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

#include "NativeWindow.h"

namespace Ui {
    class MainWindow;
}

namespace gui
{
    class MainWindow : public QMainWindow
    {
        Q_OBJECT

    public:
        MainWindow(QWidget* parent = nullptr);
        ~MainWindow();

    private:
        Ui::MainWindow* ui;
        gui::NativeWindow scene;
    };
};

#endif // MAINWINDOW_H
