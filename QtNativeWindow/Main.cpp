#include <QApplication>
#include <QLayout>
#include <QMainWindow>
#include <QWindow>
#include <QWidget>
#include <QAxWidget>
#include "LayeredWindow.hpp"
#include "TransparentWindow.hpp"


/** Sample Qt window with native window UI. */
class QtWindowWithNativeContent : public QMainWindow {
public:
    QtWindowWithNativeContent() {
        auto* mainWidget = new QWidget;
        setCentralWidget(mainWidget);

        // gradient background to test native window transparency
        mainWidget->setStyleSheet("background-color: qlineargradient(spread:pad, x1:0, y1:0, x2:1, y2:0, stop:0 rgba(0, 0, 0, 255), stop:1 rgba(255, 255, 255, 255))");

        m_layout = new QHBoxLayout;
        mainWidget->setLayout(m_layout);
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
        widget->setControl(cls);

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

    // add OLE control
    win.AddOleControl("{45C33494-127D-4AEA-B7EB-63A203D69E8A}"); // MfcOleControl

    // add native window with WS_EX_LAYERED-based transparency
    LayeredWindow lw;
    lw.Create(reinterpret_cast<HWND>(win.winId())); // parent window handle from Qt
    win.AddNativeWindow(lw.m_hWnd); // add to Qt window

    // add native window with WS_EX_TRANSPARENT-based transparency
    TransparentWindow tw;
    tw.Create(reinterpret_cast<HWND>(win.winId())); // parent window handle from Qt
    win.AddNativeWindow(tw.m_hWnd); // add to Qt window

    win.resize(800, 400);

    win.show();
    return a.exec();
}
