#pragma once
#include <QWindow>

#include "app/Painter.h"

namespace gui
{
    class Palette : public QWindow
    {
        Q_OBJECT

    public:
        explicit Palette(app::Painter painter, QWindow* parent = nullptr);
        void painter_replace(app::Painter p);
        ~Palette();

    protected:
        bool event(QEvent* e) override;
        void mouseMoveEvent(QMouseEvent* e) override;
        void mousePressEvent(QMouseEvent* e) override;
        void mouseReleaseEvent(QMouseEvent* e) override;
        void keyPressEvent(QKeyEvent* e) override;
        void keyReleaseEvent(QKeyEvent* e) override;
        void resizeEvent(QResizeEvent* e) override;
        void wheelEvent(QWheelEvent* e) override;

    private:
        app::Painter painter;
    };
};