#include <QWindow>

namespace gui
{
    class NativeWindow : public QWindow
    {
        Q_OBJECT

    public:
        explicit NativeWindow(unsigned int width, unsigned int height, QWindow* parent = nullptr);
    };
}