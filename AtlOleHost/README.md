# AtlOleHost - ATL-based host window for OLE controls

Enables testing of:
- Qt ActiveX control examples in ATL container and reproducing bugs in Qt or ATL.
- ATL host-control communication.
- Display, mouse and keyboard event communication.
- Main application window is divided in two, where left sub window hosts the OLE control. This allows testing mouse focus when clicking outside ATL host but within host application.
- Two alternative ATL host window implementations, selected at compile time.

## References
- [ATL Project Wizard](https://learn.microsoft.com/en-us/cpp/atl/reference/atl-project-wizard)
- [Using the ATL Windowing Classes](https://www.codeguru.com/cpp/com-tech/atl/article.php/c3605/Using-the-ATL-Windowing-Classes.htm)
- [ATL Internals book](http://www.369o.com/data/books/atl/index.html)
  - Ch 12 Control Containment and more
- [ATL 3.0 Window Classes](https://learn.microsoft.com/en-us/previous-versions/visualstudio/visual-studio-6.0/aa260759(v=vs.60))
