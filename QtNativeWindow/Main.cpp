#include <QApplication>
#include <QLayout>
#include <QMainWindow>
#include <QWindow>
#include <QWidget>
#include "LayeredWindow.hpp"
#include "TransparentWindow.hpp"


/** Sample Qt window with native window UI. */
class QtWindowWithNativeContent : public QMainWindow {
public:
    QtWindowWithNativeContent() {
        QWidget* mainWidget = new QWidget;
        setCentralWidget(mainWidget);

        // gradient background to test native window transparency
        mainWidget->setStyleSheet("background-color: qlineargradient(spread:pad, x1:0, y1:0, x2:1, y2:0, stop:0 rgba(0, 0, 0, 255), stop:1 rgba(255, 255, 255, 255))");

        m_layout = new QHBoxLayout;
        mainWidget->setLayout(m_layout);
    }

    void AddNativeWindow(HWND nativeHandle) {
        // wrap native window handle
        QWindow* window = QWindow::fromWinId((WId)nativeHandle);

        // re-parent native window
        QWidget* container = QWidget::createWindowContainer(window);

        m_layout->addWidget(container);
    }

    void mousePressEvent(QMouseEvent* evt) override {
        wprintf(L"QtNativeWindow mousePressEvent\n");
    }

private:
    QHBoxLayout* m_layout = nullptr;
};


int main(int argc, char *argv[]) {
    QApplication a(argc, argv);

    QtWindowWithNativeContent win;

    // add native window with WS_EX_LAYERED-based transparency
    LayeredWindow lw;
    lw.Create(reinterpret_cast<HWND>(win.winId())); // parent window handle from Qt
    win.AddNativeWindow(lw.m_hWnd); // add to Qt window

    // add native window with WS_EX_TRANSPARENT-based transparency
    TransparentWindow tw;
    tw.Create(reinterpret_cast<HWND>(win.winId())); // parent window handle from Qt
    win.AddNativeWindow(tw.m_hWnd); // add to Qt window

    win.show();
    return a.exec();
}
