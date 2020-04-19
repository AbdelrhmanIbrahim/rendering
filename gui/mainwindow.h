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

    private slots:
        bool stl_load();
        void phong_render_mode();
        void pbr_render_mode();
        void colored_render_mode();

    private:
        Ui::MainWindow* ui;
        app::Painter picasso;
        gui::Palette palette;
    };
};

#endif // MAINWINDOW_H
