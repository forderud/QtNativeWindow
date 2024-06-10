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

        QWidget* mainWidget = new QWidget;
        mainWidget->setLayout(layout);
        win.setCentralWidget(mainWidget);

        // gradient background to test native window transparency
        mainWidget->setStyleSheet("background-color: qlineargradient(spread:pad, x1:0, y1:0, x2:1, y2:0, stop:0 rgba(0, 0, 0, 255), stop:1 rgba(255, 255, 255, 255))");
    }

    win.show();
    return a.exec();
}
