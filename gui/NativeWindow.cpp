#include "NativeWindow.h"

#include "gl/gl_context.h"

#include "window/Window.h"

namespace gui
{
    NativeWindow::NativeWindow(unsigned int width, unsigned int height, QWindow* parent) :
        QWindow(parent)
    {
        resize(width, height);
        setSurfaceType(QWindow::OpenGLSurface);
        //leak
        win::Window win = win::window_new_test((void*)winId(), width, height, "qwindow_test");
        //glgpu::context_create(4, 0, win);
    }
};