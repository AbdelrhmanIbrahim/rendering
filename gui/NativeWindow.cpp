#include "NativeWindow.h"

#include "window/Window.h"
#include "gl/gl_context.h"

namespace gui
{
    NativeWindow::NativeWindow(unsigned int width, unsigned int height, QWindow* parent) :
        QWindow(parent)
    {
        resize(width, height);
        setSurfaceType(QWindow::OpenGLSurface);
        glgpu::Context ctx = glgpu::context_create(4, 0);
    }

    NativeWindow::~NativeWindow()
    {
    }
};