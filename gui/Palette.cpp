#include "Palette.h"

#include <qevent.h>
#include <qnamespace.h>

#include "window/Window.h"
#include "io/Event.h"

#include <fmt/printf.h>

namespace gui
{
    inline static io::KEYBOARD
    _map_keyboard_key(Qt::Key k)
    {
        switch (k)
        {
            case Qt::Key::Key_W:
            case Qt::UpArrow:
                return io::KEYBOARD::W;
            case Qt::Key::Key_A:
            case Qt::LeftArrow:
                return io::KEYBOARD::A;
            case Qt::Key::Key_S:
            case Qt::DownArrow:
                return io::KEYBOARD::S;
            case Qt::Key::Key_D:
            case Qt::RightArrow:
                return io::KEYBOARD::D;
            default:
                return io::KEYBOARD::COUNT;
        }
    }

    inline static io::MOUSE
     _map_mouse_button(Qt::MouseButton b)
    {
        switch (b)
        {
            case Qt::MouseButton::LeftButton:
                return io::MOUSE::LEFT;
            case Qt::MouseButton::RightButton:
                return io::MOUSE::RIGHT;
            case Qt::MouseButton::MiddleButton:
                return io::MOUSE::MIDDLE;
            default:
                return io::MOUSE::COUNT;
        }
    }

    Palette::Palette(app::Painter p, QWindow* parent) :
    painter(p)
    {
        //frameless and glsurface
        Qt::WindowFlags flag;
        flag.setFlag(Qt::WindowType::FramelessWindowHint);
        setFlags(flag);
        setSurfaceType(QWindow::OpenGLSurface);

        //compatible pixel format -revisit-
        win::window_pixel_format_set((void*)winId());
    }

    void
    Palette::painter_replace(app::Painter p)
    {
        painter = p;
    }

    Palette::~Palette()
    {

    }
 
    bool
    Palette::event(QEvent* e)
    {
        switch (e->type() )
        {
            case QEvent::UpdateRequest:
            {
                app::painter_update(painter, width(), height());
                app::painter_paint(painter, (void*)winId(), width(), height());
                return true;
            }
            default:
                break;
        }

        return QWindow::event(e);
    }

    void
    Palette::mouseMoveEvent(QMouseEvent* event)
    {
        if (geometry().contains(event->pos()))
        {
            //recieve keyboard focus
            requestActivate();

            io::Event e{};
            e.kind = io::Event::KIND::KIND_MOUSE_MOVE;
            e.mouse_move.x = event->pos().x();
            e.mouse_move.y = event->pos().y();

            app::painter_input(painter, e);
            app::painter_update(painter, width(), height());
            app::painter_paint(painter, (void*)winId(), width(), height());
        }

        QWindow::mouseMoveEvent(event);
    }

    void 
    Palette::mousePressEvent(QMouseEvent* event)
    {
        if (geometry().contains(event->pos()))
        {
            io::Event e{};
            e.kind = io::Event::KIND::KIND_MOUSE_BUTTON;
            e.mouse_button.s = io::KEY_STATE::DOWN;
            e.mouse_button.b = _map_mouse_button(event->button());

            app::painter_input(painter, e);
            app::painter_update(painter, width(), height());
            app::painter_paint(painter, (void*)winId(), width(), height());
        }

        QWindow::mousePressEvent(event);
    }

    void
    Palette::mouseReleaseEvent(QMouseEvent* event)
    {
        if (geometry().contains(event->pos()))
        {
            io::Event e{};
            e.kind = io::Event::KIND::KIND_MOUSE_BUTTON;
            e.mouse_button.s = io::KEY_STATE::UP;
            e.mouse_button.b = _map_mouse_button(event->button());

            app::painter_input(painter, e);
            app::painter_update(painter, width(), height());
            app::painter_paint(painter, (void*)winId(), width(), height());
        }
        QWindow::mouseReleaseEvent(event);
    }

    void
    Palette::keyPressEvent(QKeyEvent* event)
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

            app::painter_input(painter, e);
            app::painter_update(painter, width(), height());
            app::painter_paint(painter, (void*)winId(), width(), height());

            return;
        }

        QWindow::keyPressEvent(event);
    }

    void
    Palette::keyReleaseEvent(QKeyEvent* event)
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

            app::painter_input(painter, e);
            app::painter_update(painter, width(), height());
            app::painter_paint(painter, (void*)winId(), width(), height());

            return;
        }

        QWindow::keyReleaseEvent(event);
    }

    void
    Palette::resizeEvent(QResizeEvent* event)
    {
        if (painter && isVisible())
        {
            io::Event e{};
            e.kind = io::Event::KIND::KIND_WINDOW_RESIZE;
            e.window_resize.width = event->size().width();
            e.window_resize.height = event->size().height();

            app::painter_input(painter, e);
            app::painter_update(painter, width(), height());
            app::painter_paint(painter, (void*)winId(), width(), height());
            return;
        }

        QWindow::resizeEvent(event);
    }

    void
    Palette::wheelEvent(QWheelEvent* event)
    {
        io::Event e{};
        e.kind = io::Event::KIND::KIND_MOUSE_WHEEL;
        e.mouse_wheel.dir = event->delta();

        app::painter_input(painter, e);
        app::painter_update(painter, width(), height());
        app::painter_paint(painter, (void*)winId(), width(), height());

        QWindow::wheelEvent(event);
    }
};