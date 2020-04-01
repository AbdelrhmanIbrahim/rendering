#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

#include "app/Painter.h"

#include "Palette.h"

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
        app::Painter picasso;
        gui::Palette palette;
    };
};

#endif // MAINWINDOW_H
