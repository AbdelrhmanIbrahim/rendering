#pragma once
#include <QWindow>

#include "app/Painter.h"

namespace gui
{
    class Palette : public QWindow
    {
        Q_OBJECT

    public:
        explicit Palette(QWindow* parent = nullptr);
        ~Palette();
        void mouseMoveEvent(QMouseEvent* event) override;
        void keyPressEvent(QKeyEvent* event) override;
        void keyReleaseEvent(QKeyEvent* event) override;
        void resizeEvent(QResizeEvent* event) override;
 
    private:
        app::Painter picasso;
    };
};