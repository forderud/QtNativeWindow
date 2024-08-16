#include <atlbase.h>
#include <atlwin.h>
#include <atltypes.h>
#include <iostream>
#include "../QtNativeWindow/LayeredWindow.hpp"
#include "../QtNativeWindow/TransparentWindow.hpp"


class ParentWindow : public CWindowImpl<ParentWindow> {
public:
    static const wchar_t* GetWndCaption() {
        return L"ParentWindow";
    }

    static DWORD GetWndExStyle(DWORD dwExStyle) {
        // Enable double-buffering with WS_EX_COMPOSITED to avoid flickering when child window is WS_EX_TRANSPARENT
        // DOC: https://learn.microsoft.com/en-us/windows/win32/winmsg/extended-window-styles
        return dwExStyle | WS_EX_COMPOSITED;
    }

private:
    BEGIN_MSG_MAP(ParentWindow)
        MESSAGE_HANDLER(WM_PAINT, OnPaint)
    END_MSG_MAP()

    LRESULT OnPaint(UINT nMsg, WPARAM wparam, LPARAM lparam, BOOL& handled) {
        PAINTSTRUCT ps = {};
        HDC hdc = BeginPaint(&ps);

        // change to green color
        HGDIOBJ oldBrush = SelectObject(hdc, GetStockObject(DC_BRUSH));
        SetDCBrushColor(hdc, RGB(255, 255, 0));

        // draw filled ellipse
        Ellipse(hdc, ps.rcPaint.left, ps.rcPaint.top, ps.rcPaint.right, ps.rcPaint.bottom);

        // annotate with window style
        DrawTextW(hdc, L"Opaque parent", -1, &ps.rcPaint, DT_CENTER | DT_VCENTER | DT_SINGLELINE);

        EndPaint(&ps);
        SelectObject(hdc, oldBrush); // restore to prev. state

        return 0; // paint completed
    }
};

int main(int argc, char* argv[]) {
    ParentWindow mainWin;
    mainWin.Create(nullptr, 0, 0, WS_OVERLAPPEDWINDOW);

    TransparentWindow tw(mainWin);
    tw.MoveWindow(10, 10, 400, 400);

    LayeredWindow lw(mainWin);
    lw.MoveWindow(420, 10, 400, 400);

    mainWin.ShowWindow(SW_NORMAL);

    MSG msg{};
    while (GetMessageW(&msg, 0, 0, 0)) {
        if (msg.message == WM_QUIT)
            break;

        TranslateMessage(&msg);
        DispatchMessageW(&msg);
    }

    return 0;
}
