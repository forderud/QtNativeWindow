Sample code for embedding non-Qt UI in a Qt application

![image](https://github.com/forderud/QtForeignWindow/assets/2671400/d88c550f-8141-4cc6-bb95-70050e304235)

Known issues:
* Child window flickering problem worked around manually with [`WS_EX_COMPOSITED`](https://learn.microsoft.com/en-us/windows/win32/winmsg/extended-window-styles).
* Admin privileges required to build & test: #1.

Documentation:
* Qt [Window Embedding](https://doc.qt.io/qt-6/platform-integration.html#window-embedding)
* [QWindow::fromWinId](https://doc.qt.io/qt-6/qwindow.html#fromWinId)
* [QWidget::createWindowContainer](https://doc.qt.io/qt-6/qwidget.html#createWindowContainer)
