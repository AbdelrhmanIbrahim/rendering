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
            case Qt::UpArrow:
                return io::KEYBOARD::W;
            case Qt::Key_A:
            case Qt::LeftArrow:
                return io::KEYBOARD::A;
            case Qt::Key_S:
            case Qt::DownArrow:
                return io::KEYBOARD::S;
            case Qt::Key_D:
            case Qt::RightArrow:
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
            //app will crash on exit as there's no closeevent virtual fn in QWindow yet, find a workaround
            case QEvent::Resize:
            {
                e.kind = io::Event::KIND::KIND_WINDOW_RESIZE;
                e.window_resize.width = ((QResizeEvent*)event)->size().width();
                e.window_resize.height = ((QResizeEvent*)event)->size().height();
                break;
            }
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

        //frameless
        Qt::WindowFlags flag;
        flag.setFlag(Qt::WindowType::FramelessWindowHint);
        setFlags(flag);

        //painter
        picasso = app::painter_new();
    }

    void
    NativeWindow::mouseMoveEvent(QMouseEvent* event)
    {
        if (geometry().contains(event->pos()))
        {
            io::Event e{};
            e.kind = io::Event::KIND::KIND_MOUSE_MOVE;
            e.mouse_move.x = event->pos().x();
            e.mouse_move.y = event->pos().y();
            app::painter_input(picasso, e);
            app::painter_update(picasso, width(), height());
            app::painter_paint(picasso, (void*)winId(), width(), height());
        }

        QWindow::mouseMoveEvent(event);
    }

    void
    NativeWindow::keyPressEvent(QKeyEvent* event)
    {
        if ((event)->key() == Qt::Key::Key_W ||
            (event)->key() == Qt::Key::Key_A ||
            (event)->key() == Qt::Key::Key_S ||
            (event)->key() == Qt::Key::Key_D)
        {
            io::Event e{};
            e.kind = io::Event::KIND::KIND_KEYBOARD_KEY;
            e.keyboard_key.s = io::KEY_STATE::DOWN;
            e.keyboard_key.k = _map_keyboard_key((Qt::Key)(event)->key());

            app::painter_input(picasso, e);
            app::painter_update(picasso, width(), height());
            app::painter_paint(picasso, (void*)winId(), width(), height());

            return;
        }

        QWindow::keyPressEvent(event);
    }

    void
    NativeWindow::keyReleaseEvent(QKeyEvent* event)
    {
        if ((event)->key() == Qt::Key::Key_W ||
            (event)->key() == Qt::Key::Key_A ||
            (event)->key() == Qt::Key::Key_S ||
            (event)->key() == Qt::Key::Key_D)
        {
            io::Event e{};
            e.kind = io::Event::KIND::KIND_KEYBOARD_KEY;
            e.keyboard_key.s = io::KEY_STATE::UP;
            e.keyboard_key.k = _map_keyboard_key((Qt::Key)(event)->key());

            app::painter_input(picasso, e);
            app::painter_update(picasso, width(), height());
            app::painter_paint(picasso, (void*)winId(), width(), height());

            return;
        }

        QWindow::keyReleaseEvent(event);
    }

    void
    NativeWindow::resizeEvent(QResizeEvent* event)
    {
        if (picasso)
        {
            io::Event e{};
            e.kind = io::Event::KIND::KIND_WINDOW_RESIZE;
            e.window_resize.width = event->size().width();
            e.window_resize.height = event->size().height();

            app::painter_input(picasso, e);
            app::painter_update(picasso, width(), height());
            app::painter_paint(picasso, (void*)winId(), width(), height());
            return;
        }

        QWindow::resizeEvent(event);
    }

    NativeWindow::~NativeWindow()
    {
        app::painter_free(picasso);
    }
};