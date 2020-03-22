#include "NativeWindow.h"

#include "app/Painter.h"

namespace gui
{
    NativeWindow::NativeWindow(QWindow* parent)
    {
        setSurfaceType(QWindow::OpenGLSurface);
        picasso = app::painter_new();
    }

    bool
    NativeWindow::event(QEvent* event)
    {
        app::painter_update(picasso, width(), height());
        //leak
        app::painter_paint(picasso, win::window_test_new((void*)winId(), width(), height(), ""));
        return true;
    }

    NativeWindow::~NativeWindow()
    {
        app::painter_free(picasso);
    }
};