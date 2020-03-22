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
        bool event(QEvent*) override;
        ~NativeWindow();
 
    private:
        app::Painter picasso;
    };
};