#pragma once
#include <atlbase.h>
#include <atlwin.h>
#include <atltypes.h>


/** Window that draws an ellipse on transparent background.
    Uses WS_EX_TRANSPARENT and no background clearing to achieve a transparent background.
    Uses WM_NCHITTEST for custom hit testing to forward mouse messages in background areas to the underlying window. */
class TransparentWindow : public CWindowImpl<TransparentWindow> {
public:
    TransparentWindow(HWND parent) {
        Create(parent);
    }

    ~TransparentWindow() override {
    }

    static DWORD GetWndExStyle(DWORD dwExStyle) {
        return dwExStyle | WS_EX_TRANSPARENT;
    }

    static const wchar_t* GetWndCaption() {
        return L"TransparentWindow";
    }

private:
    BEGIN_MSG_MAP(TransparentWindow)
        MESSAGE_HANDLER(WM_PAINT, OnPaint)
        MESSAGE_HANDLER(WM_NCHITTEST, OnHitTest)
        MESSAGE_HANDLER(WM_ERASEBKGND, OnEraseBackground)
        MESSAGE_HANDLER(WM_LBUTTONDOWN, OnLButtonDown)
    END_MSG_MAP()

    // Message Handlers
    LRESULT OnPaint(UINT nMsg, WPARAM wparam, LPARAM lparam, BOOL& bHandled) {
        PAINTSTRUCT ps = {};
        HDC hdc = BeginPaint(&ps);

        // change to blue color
        HGDIOBJ prev = SelectObject(hdc, GetStockObject(DC_BRUSH));
        SetDCBrushColor(hdc, RGB(0, 0, 255));

        // draw filled ellipse
        Ellipse(hdc, ps.rcPaint.left, ps.rcPaint.top, ps.rcPaint.right, ps.rcPaint.bottom);
        // annotate with window style
        DrawTextW(hdc, L"native WS_EX_TRANSPARENT", -1, &ps.rcPaint, DT_CENTER | DT_VCENTER | DT_SINGLELINE);

        EndPaint(&ps);
        SelectObject(hdc, prev); // restore to prev. state

        return 0; // paint completed
    }

    LRESULT OnHitTest(UINT nMsg, WPARAM wparam, LPARAM lparam, BOOL& bHandled) {
        POINT pt{ GET_X_LPARAM(lparam), GET_Y_LPARAM(lparam) };
        ScreenToClient(&pt); // convert to client coordinates

        RECT client = {}; // ellipse covers client rect
        GetClientRect(&client);

        if (IsInsideEllipse(pt, client))
            return HTCLIENT; // absorb message
        else
            return HTTRANSPARENT; // forward message to underlying window
    }

    LRESULT OnEraseBackground(UINT nMsg, WPARAM wparam, LPARAM lparam, BOOL& bHandled) {
        return 0; // background not cleared
    }

    LRESULT OnLButtonDown(UINT, WPARAM, LPARAM, BOOL& bHandled) {
        wprintf(L"TransparentWindow WM_LBUTTONDOWN.\n");
        SetFocus();
        bHandled = FALSE; // don't absorb message
        return 0;
    }

    static bool IsInsideEllipse(POINT pt, RECT ellipse) {
        POINT center{ (ellipse.right + ellipse.left) / 2, (ellipse.bottom + ellipse.top) / 2 }; // ellipse center
        POINT ext{ (ellipse.right - ellipse.left) / 2, (ellipse.bottom - ellipse.top) / 2 }; // ellipse extends

        POINT rel{ pt.x - center.x, pt.y - center.y }; // point relative to center

        // ellipse formula
        return (rel.x * rel.x) * 1.0f / (ext.x * ext.x) + (rel.y * rel.y) * 1.0f / (ext.y * ext.y) <= 1.0f;
    }
};
