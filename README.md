Sample code for **embedding non-Qt UI in a Qt application**. Emphasis is put on native windows with transparent background.

Screenshot:  
![image](https://github.com/forderud/QtNativeWindow/assets/2671400/e926b0d8-2f8e-4d1e-9c05-801908646619)

Window hierarchy with extended styles:  
![image](https://github.com/forderud/QtNativeWindow/assets/2671400/7faa2666-647a-4a1f-bfdb-4a64ddd78491)


## Discovered issues

### Qt foreign window flickering

[QTBUG-126280](https://bugreports.qt.io/browse/QTBUG-126280): The semi-transparent foreign windows showing green & blue circles will *flicker when the main window is resized*. This is currently worked around by manually setting [`WS_EX_COMPOSITED`](https://learn.microsoft.com/en-us/windows/win32/winmsg/extended-window-styles) on the parent Qt window. However, this feels like a hack that leads to other drawing artifacts.


### Qt OLE window transparency

Transparent background has not yet been enabled for the OLE window due to [Active Qt](https://doc.qt.io/qt-6/activeqt-index.html) drawing artifacts. Define `TRANSPARENT_OLE_CONTROL` to reproduce this problem.

This problem have not yet been submitted to the Qt company.


## Documentation
* Qt [Window Embedding](https://doc.qt.io/qt-6/platform-integration.html#window-embedding)
* [QWindow::fromWinId](https://doc.qt.io/qt-6/qwindow.html#fromWinId)
* [QWidget::createWindowContainer](https://doc.qt.io/qt-6/qwidget.html#createWindowContainer)
