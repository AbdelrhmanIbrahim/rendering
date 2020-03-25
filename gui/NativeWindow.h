#pragma once
#include <QWindow>

#include "app/Painter.h"

namespace gui
{
    class NativeWindow : public QWindow
    {
        Q_OBJECT

    public:
        explicit NativeWindow(QWindow* parent = nullptr);
        ~NativeWindow();
        void mouseMoveEvent(QMouseEvent* event) override;
        void keyPressEvent(QKeyEvent* event) override;
        void keyReleaseEvent(QKeyEvent* event) override;
        void resizeEvent(QResizeEvent* event) override;
 
    private:
        app::Painter picasso;
    };
};