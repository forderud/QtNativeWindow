#include <QApplication>
#include <QLayout>
#include <QMainWindow>
#include <QWindow>
#include <QWidget>
#include "LayeredWindow.hpp"
#include "TransparentWindow.hpp"


class MyWindow : public QMainWindow {
public:
};


static QWidget* EmbedNativeWindow(HWND nativeHandle) {
    // wrap native window handle
    QWindow* window = QWindow::fromWinId((WId)nativeHandle);

    // re-parent native window
    QWidget* container = QWidget::createWindowContainer(window);
    return container;
}

int main(int argc, char *argv[]) {
    QApplication a(argc, argv);

    MyWindow win;

    // create native windows
    LayeredWindow lw;
    lw.Create(reinterpret_cast<HWND>(win.winId())); // parent window handle from Qt

    TransparentWindow tw;
    tw.Create(reinterpret_cast<HWND>(win.winId())); // parent window handle from Qt

    {
        // embed native widows in Qt window
        QHBoxLayout* layout = new QHBoxLayout;
        layout->addWidget(EmbedNativeWindow(lw.m_hWnd));
        layout->addWidget(EmbedNativeWindow(tw.m_hWnd));

        // Set layout in QWidget
        QWidget* widget = new QWidget;
        widget->setLayout(layout);

        win.setCentralWidget(widget);
    }
    
    win.show();
    return a.exec();
}
