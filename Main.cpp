#include <QApplication>
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

    // create native window
    HWND parent = reinterpret_cast<HWND>(win.winId());
    LayeredWindow nw;
    //TransparentWindow nw;
    nw.Create(parent);

    // embed native widow in Qt window
    win.setCentralWidget(EmbedNativeWindow(nw.m_hWnd));
    
    win.show();
    return a.exec();
}
