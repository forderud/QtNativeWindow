#include <QApplication>
#include <QLayout>
#include <QMainWindow>
#include <QWindow>
#include <QWidget>
#include <QAxWidget>
#include "LayeredWindow.hpp"
#include "TransparentWindow.hpp"


#define TEST_OLE_CONTROL
#define AVOID_FLICKERING_WITH_COMPOSITED_WINDOW

/** Sample Qt application that contain native child windows. */
class QtWindowWithNativeContent : public QMainWindow {
public:
    QtWindowWithNativeContent() {
        auto* mainWidget = new QWidget;
        setCentralWidget(mainWidget);

        // gradient background to test native window transparency
        mainWidget->setStyleSheet("background-color: qlineargradient(spread:pad, x1:0, y1:0, x2:1, y2:0, stop:0 rgba(0, 0, 0, 255), stop:1 rgba(255, 255, 255, 255))");

        m_layout = new QHBoxLayout;
        mainWidget->setLayout(m_layout);

#ifdef AVOID_FLICKERING_WITH_COMPOSITED_WINDOW
        // enable double-buffering with WS_EX_COMPOSITED to avoid flickering
        // TODO: Figure out how to do this through Qt
        auto wnd = (HWND)mainWidget->winId();
        SetWindowLongW(wnd, GWL_EXSTYLE, GetWindowLongW(wnd, GWL_EXSTYLE) | WS_EX_COMPOSITED);
#endif
    }

    void AddNativeWindow(HWND nativeHandle) {
        // wrap native window handle
        auto* window = QWindow::fromWinId((WId)nativeHandle);

        // re-parent native window
        auto* widget = QWidget::createWindowContainer(window);

        m_layout->addWidget(widget);
    }

    void AddOleControl(QString cls) {
        auto* widget = new QAxWidget();
        bool ok = widget->setControl(cls);

        if (ok)
            m_layout->addWidget(widget);
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

#ifdef TEST_OLE_CONTROL
    // add OLE control
    win.AddOleControl("{45C33494-127D-4AEA-B7EB-63A203D69E8A}"); // MfcOleControl
#endif

    // add native window with WS_EX_LAYERED-based transparency
    LayeredWindow lw(reinterpret_cast<HWND>(win.winId())); // parent window handle from Qt
    win.AddNativeWindow(lw.m_hWnd); // add to Qt window

    // add native window with WS_EX_TRANSPARENT-based transparency
    TransparentWindow tw(reinterpret_cast<HWND>(win.winId())); // parent window handle from Qt
    win.AddNativeWindow(tw.m_hWnd); // add to Qt window

    win.resize(800, 400);
    win.show();

    return a.exec();
}
