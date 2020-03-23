#include "OpenglPalette.h"

#include "gl/glgpu.h"

namespace gui
{
    OpenglPalette::OpenglPalette(int glmajor_version, int glminor_version, QWindow* parent):
    picasso(nullptr),
    ctx(nullptr)
    {
        //setSurfaceType(QWindow::OpenGLSurface);

        //init glcontext
        //ctx = new QOpenGLContext(this);
        //QSurfaceFormat fmt;
        //fmt.setVersion(glmajor_version, glminor_version);
        //ctx->setFormat(fmt);
        //ctx->create();
        //ctx->makeCurrent(this);
        
        glgpu::error();
        glgpu::color_clear(1,1,1);
        glgpu::error();
        //auto buf = glgpu::buffer_uniform_create(sizeof(math::vec4f));
        //picasso = app::painter_new();
    }

    bool
    OpenglPalette::event(QEvent* event)
    {
        switch (event->type())
        {
            case QEvent::MouseButtonPress:
            {
                ctx->makeCurrent(this);
                app::painter_update(picasso, width(), height());
                app::painter_paint(picasso, (void*)winId(), width(), height());
                ctx->swapBuffers(this);
                return true;
            }
            default:
                return QWindow::event(event);
        }
    }

    OpenglPalette::~OpenglPalette()
    {
        app::painter_free(picasso);
        delete ctx;
    }
};