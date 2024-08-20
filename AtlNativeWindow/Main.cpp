#include <atlbase.h>
#include <atlwin.h>
#include <atltypes.h>
#include <iostream>
#include "../QtNativeWindow/LayeredWindow.hpp"
#include "../QtNativeWindow/TransparentWindow.hpp"


class AtlNativeWindowModule :
    public ATL::CAtlExeModuleT<AtlNativeWindowModule> {
};
AtlNativeWindowModule _AtlModule; // required for CAxWindow


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
        MESSAGE_HANDLER(WM_DESTROY, OnDestroy)
    END_MSG_MAP()

    LRESULT OnPaint(UINT nMsg, WPARAM wparam, LPARAM lparam, BOOL& handled) {
        PAINTSTRUCT ps = {};
        HDC hdc = BeginPaint(&ps);

        HGDIOBJ oldBrush = SelectObject(hdc, GetStockObject(DC_BRUSH)); // enable brush color change
        SetDCBrushColor(hdc, RGB(255, 255, 0)); // yellow fill
        SelectObject(hdc, GetStockObject(DC_PEN)); // enable pen color change
        SetDCPenColor(hdc, RGB(63, 63, 63)); // dark gray outline

        // draw filled ellipse
        Ellipse(hdc, ps.rcPaint.left, ps.rcPaint.top, ps.rcPaint.right, ps.rcPaint.bottom);

        // annotate with window style
        DrawTextW(hdc, L"Opaque parent", -1, &ps.rcPaint, DT_CENTER | DT_VCENTER | DT_SINGLELINE);

        EndPaint(&ps);
        SelectObject(hdc, oldBrush); // restore to prev. state

        return 0; // paint completed
    }

    LRESULT OnDestroy(UINT nMsg, WPARAM wparam, LPARAM lparam, BOOL& handled) {
        PostQuitMessage(0);
        return 0;
    }
};

int main(int argc, char* argv[]) {
    CoInitialize(NULL);

    ParentWindow mainWin;
    mainWin.Create(nullptr, 0, 0, WS_OVERLAPPEDWINDOW);

    CAxWindow ole;
    {
        RECT olePos = { 10, 10, 200, 210 };
        ole.Create(mainWin, olePos, nullptr, WS_CHILD | WS_VISIBLE);
        ole.SetWindowTextW(L"OLE host"); // set window name _after_ construction to avoid "Invalid class string" (0x800401f3 ) error from CreateWindowEx
        GUID clsid{};
        CLSIDFromString(L"{45C33494-127D-4AEA-B7EB-63A203D69E8A}", &clsid); // MfcOleControl
        CComPtr<IUnknown> ctrl;
        HRESULT hr = ctrl.CoCreateInstance(clsid, NULL, CLSCTX_ALL);
        if (FAILED(hr))
            abort();
        ole.AttachControl(ctrl.p, /*out container IUnknown*/nullptr);
    }

    TransparentWindow tw(mainWin);
    tw.MoveWindow(210, 10, 190, 200);

    LayeredWindow lw(mainWin);
    lw.MoveWindow(410, 10, 190, 200);

    mainWin.ShowWindow(SW_NORMAL);

    MSG msg{};
    while (BOOL ret = GetMessageW(&msg, 0, 0, 0)) {
        if (ret == -1) // error occured
            break;

        TranslateMessage(&msg);
        DispatchMessageW(&msg);
    }

    return 0;
}
