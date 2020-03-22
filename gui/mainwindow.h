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
        explicit MainWindow(QWidget* parent = nullptr);
        ~MainWindow();

    private:
        Ui::MainWindow* ui;
        NativeWindow qnative_win;
    };
};

#endif // MAINWINDOW_H
