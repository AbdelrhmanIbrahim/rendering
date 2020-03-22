#include "NativeWindow.h"

#include "app/Painter.h"

namespace gui
{
    NativeWindow::NativeWindow(win::Window win, QWindow* parent)
    {
        //setSurfaceType(QWindow::OpenGLSurface);
        palette = win;
        math::vec2f size = win::window_size(palette);
        resize(size[0], size[1]);
        picasso = app::painter_new();
    }

    bool
    NativeWindow::event(QEvent* event)
    {
        //update and render
        if(event->type() != QEvent::Close)
        {
            app::painter_update(picasso, width(), height());
            app::painter_paint(picasso, win::window_test_new((void*)winId(), width(), height(), ""));
        }

        return true;
    }

    NativeWindow::~NativeWindow()
    {
        win::window_free(palette);
        app::painter_free(picasso);
    }
};