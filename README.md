Sample code for embedding non-Qt UI in a Qt application

![image](https://github.com/forderud/QtNativeWindow/assets/2671400/e926b0d8-2f8e-4d1e-9c05-801908646619)

### Known issues
* Child window flickering problem worked around with [`WS_EX_COMPOSITED`](https://learn.microsoft.com/en-us/windows/win32/winmsg/extended-window-styles).
* Admin privileges required to build & test: https://github.com/forderud/QtNativeWindow/issues/1.
* Qt SDK path hardcoded in `QtNativeWindow.bat`.

### Documentation
* Qt [Window Embedding](https://doc.qt.io/qt-6/platform-integration.html#window-embedding)
* [QWindow::fromWinId](https://doc.qt.io/qt-6/qwindow.html#fromWinId)
* [QWidget::createWindowContainer](https://doc.qt.io/qt-6/qwidget.html#createWindowContainer)
