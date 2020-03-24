#include "NativeWindow.h"

#include <qevent.h>
#include <qnamespace.h>

#include "window/Window.h"
#include "io/Event.h"

namespace gui
{

    inline static io::KEYBOARD
    _map_keyboard_key(Qt::Key k)
    {
        switch (k)
        {
            case Qt::Key_W:
                return io::KEYBOARD::W;
            case Qt::Key_A:
                return io::KEYBOARD::A;
            case Qt::Key_S:
                return io::KEYBOARD::S;
            case Qt::Key_D:
                return io::KEYBOARD::D;
            default:
                return io::KEYBOARD::COUNT;
        }
    }

    io::Event
    _transform_qt_event(QEvent* event)
    {
        io::Event e{};
        e.kind = io::Event::KIND::KIND_DUMMY;

        switch (event->type())
        {
            case QEvent::MouseMove:
            {
                e.kind = io::Event::KIND::KIND_MOUSE_MOVE;
                e.mouse_move.x = ((QMouseEvent*)event)->pos().x();
                e.mouse_move.y = ((QMouseEvent*)event)->pos().y();
                break;
            }
            case QEvent::KeyPress:
            {
                e.kind = io::Event::KIND::KIND_KEYBOARD_KEY;
                e.keyboard_key.s = io::KEY_STATE::DOWN;
                e.keyboard_key.k = _map_keyboard_key((Qt::Key)((QKeyEvent*)event)->key());
                break;
            }
            case QEvent::KeyRelease:
            {
                e.kind = io::Event::KIND::KIND_KEYBOARD_KEY;
                e.keyboard_key.s = io::KEY_STATE::UP;
                e.keyboard_key.k = _map_keyboard_key((Qt::Key)((QKeyEvent*)event)->key());
                break;
            }
            default:
                break;
        }
        return e;
    }

    NativeWindow::NativeWindow(QWindow* parent) :
    picasso(nullptr)
    {
        //prepare window pixel format and vsync
        setSurfaceType(QWindow::OpenGLSurface);
        win::window_pixel_format_set((void*)winId());
        QSurfaceFormat fmt;
        fmt.setSwapInterval(0);
        setFormat(fmt);

        //painter
        picasso = app::painter_new();
    }

    bool
    NativeWindow::event(QEvent* event)
    {
        if(picasso)
        {
            io::Event e = _transform_qt_event(event);
            if (e.kind != io::Event::KIND_WINDOW_CLOSE &&
                e.kind != io::Event::KIND_DUMMY)
            {
                app::painter_input(picasso, e);
                app::painter_update(picasso, width(), height());
                app::painter_paint(picasso, (void*)winId(), width(), height());
                return true;
            }
        }
        else
            return QWindow::event(event);
    }

    NativeWindow::~NativeWindow()
    {
        app::painter_free(picasso);
    }
};