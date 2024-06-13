Sample code for **embedding non-Qt UI in a Qt application**. Emphasis is put on native windows with transparent background.

Screenshot:  
![image](https://github.com/forderud/QtNativeWindow/assets/2671400/f5777ce5-ec10-4763-a9a1-25d2bc00f70b)

Window hierarchy with extended styles:  
![image](https://github.com/forderud/QtNativeWindow/assets/2671400/1d665924-05d1-4b11-a812-734991d7bd4c)


## Discovered issues

### Qt foreign window flickering

[QTBUG-126280](https://bugreports.qt.io/browse/QTBUG-126280): The semi-transparent foreign windows showing green & blue circles will *flicker when the main window is resized*. This is currently worked around by manually setting [`WS_EX_COMPOSITED`](https://learn.microsoft.com/en-us/windows/win32/winmsg/extended-window-styles) on the parent Qt window. However, this feels like a hack that leads to other drawing artifacts.


### Qt OLE window transparency

[QAxWidget](https://doc.qt.io/qt-6/qaxwidget.html) doesn't seem to handle OLE windows with transparent background properly. This leads to drawing artifacts where the background is not cleared.

This problem have not yet been submitted to the Qt company.


## Qt documentation
* Qt [Window Embedding](https://doc.qt.io/qt-6/platform-integration.html#window-embedding)
* [QWindow::fromWinId](https://doc.qt.io/qt-6/qwindow.html#fromWinId)
* [QWidget::createWindowContainer](https://doc.qt.io/qt-6/qwidget.html#createWindowContainer)
* [Using ActiveX controls and COM in Qt](https://doc.qt.io/qt-6/activeqt-container.html)
