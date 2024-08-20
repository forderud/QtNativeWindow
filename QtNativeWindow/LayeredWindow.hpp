#pragma once
#include <atlbase.h>
#include <atlwin.h>
#include <atltypes.h>
#include <cassert>


#define LAYERED_STYLE_WORK_ARUND

/** Window that draws an ellipse on transparent background.
    Uses WS_EX_LAYERED to achieve a transparent background through a magic RGB value.
    Custom hit testing is not required, since Windows already knows which pixels are transparent.
    NOTE: WS_EX_LAYERED is only supported for child windows on Win8 and newer, so a MANIFEST FILE IS REQURED to enable the feature. */
class LayeredWindow : public CWindowImpl<LayeredWindow> {
public:
    LayeredWindow(HWND parent) {
        Create(parent);
    }

    ~LayeredWindow() override {
    }

#ifndef LAYERED_STYLE_WORK_ARUND
    static DWORD GetWndExStyle(DWORD dwExStyle) {
        return dwExStyle | WS_EX_LAYERED;
    }
#endif

    static const wchar_t* GetWndCaption() {
        return L"LayeredWindow";
    }

private:
    BEGIN_MSG_MAP(LayeredWindow)
        MESSAGE_HANDLER(WM_CREATE, OnCreate)
        MESSAGE_HANDLER(WM_PAINT, OnPaint)
        MESSAGE_HANDLER(WM_ERASEBKGND, OnEraseBackground)
        MESSAGE_HANDLER(WM_LBUTTONDOWN, OnLButtonDown)
    END_MSG_MAP()

    // Message Handlers
    LRESULT OnCreate(UINT nMsg, WPARAM wparam, LPARAM lparam, BOOL& handled) {
#ifndef LAYERED_STYLE_WORK_ARUND
        // configure transparent background
        // WARNING: Doesn't work
        assert(GetExStyle() & WS_EX_LAYERED);
        BOOL ok = SetLayeredWindowAttributes(m_hWnd, s_background, 0, LWA_COLORKEY);
        assert(ok);
#endif

        return 0;
    }

    LRESULT OnPaint(UINT nMsg, WPARAM wparam, LPARAM lparam, BOOL& handled) {
        PAINTSTRUCT ps = {};
        HDC hdc = BeginPaint(&ps);

        HGDIOBJ prev = SelectObject(hdc, GetStockObject(DC_BRUSH)); // enable brush color change
        SetDCBrushColor(hdc, RGB(0, 255, 0)); // green fill
        SelectObject(hdc, GetStockObject(DC_PEN)); // enable pen color change
        SetDCPenColor(hdc, RGB(63, 63, 63)); // dark gray outline


        // draw filled ellipse
        BOOL ok = Ellipse(hdc, ps.rcPaint.left, ps.rcPaint.top, ps.rcPaint.right, ps.rcPaint.bottom);
        assert(ok);
        // annotate with window style
        DrawTextW(hdc, L"native WS_EX_LAYERED", -1, &ps.rcPaint, DT_CENTER | DT_VCENTER | DT_SINGLELINE);

        EndPaint(&ps);
        SelectObject(hdc, prev); // restore to prev. state

        return 0; // paint completed
    }

    LRESULT OnEraseBackground(UINT nMsg, WPARAM wparam, LPARAM lparam, BOOL& handled) {
#ifdef LAYERED_STYLE_WORK_ARUND
        if (!m_initialized) {
            m_initialized = true;

            // configure transparent background
            // work-around for UI not being shown initially if doing this from WM_CREATE
            BOOL ok = ModifyStyleEx(0, WS_EX_LAYERED);
            assert(ok);
            ok = SetLayeredWindowAttributes(m_hWnd, s_background, 0, LWA_COLORKEY);
            assert(ok);
        }
#endif

        CRect area;
        GetClientRect(&area);

        // clear background
        HBRUSH brush = CreateSolidBrush(s_background);
        FillRect(GetDC(), area, brush);
        DeleteObject(brush);

        return 1; // background erased
    }

    LRESULT OnLButtonDown(UINT, WPARAM, LPARAM, BOOL& handled) {
        wprintf(L"LayeredWindow WM_LBUTTONDOWN.\n");
        SetFocus();
        handled = FALSE; // don't absorb message
        return 0;
    }

#ifdef LAYERED_STYLE_WORK_ARUND
    bool                  m_initialized = false;
#endif
    static const COLORREF s_background = RGB(1, 1, 1); // magic background color that's treated as transparent
};
