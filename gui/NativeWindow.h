#pragma once
#include <QWindow>

#include "window/Window.h"

#include "app/Painter.h"

namespace gui
{
    class NativeWindow : public QWindow
    {
        Q_OBJECT

    public:
        explicit NativeWindow(win::Window win, QWindow* parent = nullptr);
        bool event(QEvent*) override;
        ~NativeWindow();
 
    private:
        win::Window palette;
        app::Painter picasso;
    };
};