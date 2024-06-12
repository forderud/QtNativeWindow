Sample code for embedding non-Qt UI in a Qt application. Emphasis is put on native windows with transparent background.

![image](https://github.com/forderud/QtNativeWindow/assets/2671400/e926b0d8-2f8e-4d1e-9c05-801908646619)

### Known issues
* Child window flickering problem. Currently worked around with [`WS_EX_COMPOSITED`](https://learn.microsoft.com/en-us/windows/win32/winmsg/extended-window-styles).
* OLE window not yet semi-transparent.

### Documentation
* Qt [Window Embedding](https://doc.qt.io/qt-6/platform-integration.html#window-embedding)
* [QWindow::fromWinId](https://doc.qt.io/qt-6/qwindow.html#fromWinId)
* [QWidget::createWindowContainer](https://doc.qt.io/qt-6/qwidget.html#createWindowContainer)
