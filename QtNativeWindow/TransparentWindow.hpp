#pragma once
#include <atlbase.h>
#include <atlwin.h>
#include <atltypes.h>
#include <cassert>
#include "Ellipse.hpp"


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
        // WS_EX_TRANSPARENT ensure that non-WS_EX_TRANSPARENT siblings windows are painted first (see https://devblogs.microsoft.com/oldnewthing/20121217-00/?p=5823)
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
    LRESULT OnPaint(UINT nMsg, WPARAM wparam, LPARAM lparam, BOOL& handled) {
        PAINTSTRUCT ps = {};
        HDC hdc = BeginPaint(&ps);

        {
            EllipseBmp ellipse(hdc, ps.rcPaint);
            RGBQUAD color = {255, 0, 0, 64}; // semi-transparent blue (BGRA format)
            ellipse.Draw(color);
            ellipse.BlendInto(ps.rcPaint.left, ps.rcPaint.top);
        }

        // annotate with window style
        DrawTextW(hdc, L"native WS_EX_TRANSPARENT", -1, &ps.rcPaint, DT_CENTER | DT_VCENTER | DT_SINGLELINE);

        EndPaint(&ps);

        return 0; // paint completed
    }

    LRESULT OnHitTest(UINT nMsg, WPARAM wparam, LPARAM lparam, BOOL& handled) {
        POINT pt{ GET_X_LPARAM(lparam), GET_Y_LPARAM(lparam) };
        ScreenToClient(&pt); // convert to client coordinates

        RECT client = {}; // ellipse covers client rect
        GetClientRect(&client);

        if (IsInsideEllipse(pt, client))
            return HTCLIENT; // absorb message
        else
            return HTTRANSPARENT; // forward message to underlying window
    }

    LRESULT OnEraseBackground(UINT nMsg, WPARAM wparam, LPARAM lparam, BOOL& handled) {
        return 0; // background not cleared
    }

    LRESULT OnLButtonDown(UINT, WPARAM, LPARAM, BOOL& handled) {
        wprintf(L"TransparentWindow WM_LBUTTONDOWN.\n");
        SetFocus();
        handled = FALSE; // don't absorb message
        return 0;
    }
};
