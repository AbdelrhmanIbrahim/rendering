#pragma once
#include <QWindow>
#include <QOpenGLContext>

#include "app/Painter.h"

namespace gui
{
    class OpenglPalette : public QWindow
    {
        Q_OBJECT

    public:
        explicit OpenglPalette(int glmajor_version, int glminor_version, QWindow* parent = nullptr);
        bool event(QEvent*) override;
        ~OpenglPalette();

    private:
        app::Painter picasso;
        QOpenGLContext* ctx;
    };
};