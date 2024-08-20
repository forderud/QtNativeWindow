Sample code for **embedding non-Qt UI in a Qt application**. Emphasis is put on native windows and OLE controls with transparent background.

Screenshot:  
![image](https://github.com/user-attachments/assets/120e6645-db4b-4eae-93ce-809f9cad4ad5)

Window hierarchy with [extended window styles](https://learn.microsoft.com/en-us/windows/win32/winmsg/extended-window-styles):  
![image](https://github.com/forderud/QtNativeWindow/assets/2671400/bf7e0231-3e0a-43cc-929a-7949e1cf8866)

## Discovered issues

### Qt foreign window flickering

[QTBUG-126280](https://bugreports.qt.io/browse/QTBUG-126280): The semi-transparent foreign windows showing green & blue circles will *flicker when the main window is resized*. This is currently worked around by manually setting `WS_EX_COMPOSITED` on the parent Qt window. However, this feels like a hack that leads to other drawing artifacts.

Stop defining `AVOID_FLICKERING_WITH_COMPOSITED_WINDOW` to reproduce this issue.

### Qt OLE control transparency problem

[QAxWidget](https://doc.qt.io/qt-6/qaxwidget.html) doesn't seem to handle OLE controls with transparent background properly. This leads to drawing artifacts where the background is not cleared.

This problem have not yet been submitted to the Qt company.

## Prerequisites
* [Visual Studio 2022](https://visualstudio.microsoft.com/)
  - Select "Desktop development with C++" workload.
  - Select "C++ MFC for latest v143 build tools (x86 & x64)" component.
* [Qt SDK](https://www.qt.io/product/development-tools) (any recent version)
  - Select "Active Qt" component.
* [Qt VS Tools](https://doc.qt.io/qtvstools/)

## Qt documentation
* Qt [Window Embedding](https://doc.qt.io/qt-6/platform-integration.html#window-embedding)
* [QWindow::fromWinId](https://doc.qt.io/qt-6/qwindow.html#fromWinId)
* [QWidget::createWindowContainer](https://doc.qt.io/qt-6/qwidget.html#createWindowContainer)
* [Using ActiveX controls and COM in Qt](https://doc.qt.io/qt-6/activeqt-container.html)

## Microsoft resources
* Raymond Chen: [I used WS_EX_COMPOSITED to get rid of my redrawing flicker, but it resulted in sluggish response](https://devblogs.microsoft.com/oldnewthing/20171018-00/?p=97245) - explains that `WS_EX_COMPOSITED` relies on the release the device contexts (DC) _or_ `End­Paint` calls to detect that rendering have completed.
