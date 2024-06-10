Sample code for embedding non-Qt UI in a Qt application 


Sample code:
* https://github.com/qt/qtbase/blob/dev/tests/manual/embeddedwindows/main.cpp#L80
* https://github.com/qt/qtbase/blob/dev/tests/manual/foreignwindows/main.cpp#L253
* https://github.com/qt/qtbase/blob/dev/tests/manual/embeddedintoforeignwindow/main.cpp#L237

Maybe relevant links:
* https://stackoverflow.com/questions/45061803/cannot-get-qwindowfromwinid-to-work-properly

Documentation:
* Qt [Window Embedding](https://doc.qt.io/qt-6/platform-integration.html#window-embedding) mentions `QWindow::fromWinId` for native window handles and `QWidget::createWindowContainer` for re-parenting
* [QWindow::fromWinId](https://doc.qt.io/qt-6/qwindow.html#fromWinId)
