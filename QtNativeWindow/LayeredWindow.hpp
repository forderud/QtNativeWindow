#pragma once
#include <atlbase.h>
#include <atlwin.h>
#include <atltypes.h>


/** Window that draws an ellipse on transparent background.
    Uses WS_EX_LAYERED to achieve a transparent background through a magic RGB value.
    Custom hit testing is not required, since Windows already knows which pixels are transparent. */
class LayeredWindow : public CWindowImpl<LayeredWindow> {
public:
    LayeredWindow() {
    }

    ~LayeredWindow() override {
    }

    static const wchar_t* GetWndCaption() {
        return L"LayeredWindow";
    }

private:
    BEGIN_MSG_MAP(LayeredWindow)
        MESSAGE_HANDLER(WM_PAINT, OnPaint)
        MESSAGE_HANDLER(WM_ERASEBKGND, OnEraseBackground)
        MESSAGE_HANDLER(WM_LBUTTONDOWN, OnLButtonDown)
    END_MSG_MAP()

    // Message Handlers
    LRESULT OnPaint(UINT nMsg, WPARAM wparam, LPARAM lparam, BOOL& bHandled) {
        PAINTSTRUCT ps = {};
        HDC hdc = BeginPaint(&ps);

        // change to green color
        HGDIOBJ prev = SelectObject(hdc, GetStockObject(DC_BRUSH));
        SetDCBrushColor(hdc, RGB(0, 255, 0));

        // draw filled ellipse
        Ellipse(hdc, ps.rcPaint.left, ps.rcPaint.top, ps.rcPaint.right, ps.rcPaint.bottom);
        // annotate with window style
        DrawTextW(hdc, L"WS_EX_LAYERED", -1, &ps.rcPaint, DT_CENTER | DT_VCENTER | DT_SINGLELINE);

        EndPaint(&ps);
        SelectObject(hdc, prev); // restore to prev. state

        return 0; // paint completed
    }

    LRESULT OnEraseBackground(UINT nMsg, WPARAM wparam, LPARAM lparam, BOOL& bHandled) {
        if (!m_initialized) {
            m_initialized = true;

            // configure transparent background
            // work-around for UI not being shown initially if doing this from WM_CREATE 
            ModifyStyleEx(0, WS_EX_LAYERED);
            SetLayeredWindowAttributes(m_hWnd, s_background, 0, LWA_COLORKEY);
        }

        CRect area;
        GetClientRect(&area);

        // clear background
        HBRUSH brush = CreateSolidBrush(s_background);
        FillRect(GetDC(), area, brush);
        DeleteObject(brush);

        return 1; // background erased
    }

    LRESULT OnLButtonDown(UINT, WPARAM, LPARAM, BOOL& bHandled) {
        wprintf(L"LayeredWindow WM_LBUTTONDOWN.\n");
        SetFocus();
        bHandled = FALSE; // don't absorb message
        return 0;
    }

    bool                  m_initialized = false;
    static const COLORREF s_background = RGB(0, 0, 0); // magic background color that's treated as transparent
};
