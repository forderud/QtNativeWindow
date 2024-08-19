#pragma once
#include <atlbase.h>
#include <atlwin.h>
#include <atltypes.h>
#include <cassert>

/** Convert red, green, blue & alpha values into a RGBQUAD with premultipled alpha channel. */
RGBQUAD RGBAPremult(BYTE r, BYTE g, BYTE b, BYTE a) {
    float alphaScale = a*1.0f/255; // in [0,1] range
    RGBQUAD res{};
    res.rgbRed = (BYTE)(r*alphaScale);
    res.rgbGreen = (BYTE)(g*alphaScale);
    res.rgbBlue = (BYTE)(b*alphaScale);
    res.rgbReserved = a;
    return res;
}

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
            HDC hdcBmp = CreateCompatibleDC(hdc);
            int width = ps.rcPaint.right - ps.rcPaint.left;
            int height = ps.rcPaint.bottom - ps.rcPaint.top;

            HBITMAP bmpObj = nullptr;
            HGDIOBJ prevObj = nullptr; // previous hdcBmp object (before bmpObj)
            {
                BITMAPINFO bmi = {};
                bmi.bmiHeader.biSize = sizeof(BITMAPINFOHEADER);
                bmi.bmiHeader.biWidth = width;
                bmi.bmiHeader.biHeight = height;
                bmi.bmiHeader.biPlanes = 1;
                bmi.bmiHeader.biBitCount = 32;
                bmi.bmiHeader.biCompression = BI_RGB;
                bmi.bmiHeader.biSizeImage = width * height * 4;

                // create offscreen bitmap with premultiplied alpha channel
                RGBQUAD* pixels = nullptr; // pixel buffer in 0xAARRGGBB format
                bmpObj = CreateDIBSection(hdcBmp, &bmi, DIB_RGB_COLORS, (void**)&pixels, NULL, 0);
                prevObj = SelectObject(hdcBmp, bmpObj);

                // draw filled ellipse into bitmap
                // cannot use GDI Ellipse function here since it sets alpha=0 (see https://devblogs.microsoft.com/oldnewthing/20210915-00/?p=105687)
                RGBQUAD color = RGBAPremult(0, 0, 255, 64); // semi-transparent blue
                for (int y = 0; y < height; y++) {
                    for (int x = 0; x < width; x++) {
                        POINT pt = { x + ps.rcPaint.left, y + ps.rcPaint.top}; // add x&y offset back so that pt matches ps.rcPaint
                        if (IsInsideEllipse({ x, y }, ps.rcPaint))
                            pixels[x + y*width] = color;
                    }
                }
            }

            {
                BLENDFUNCTION blend = {};
                blend.BlendOp = AC_SRC_OVER;
                blend.BlendFlags = 0;
                blend.SourceConstantAlpha = 0xFF;
                blend.AlphaFormat = AC_SRC_ALPHA; // per-pixel alpha

                // copy bitmat to window buffer with per-pixel alpha blending
                BOOL ok = GdiAlphaBlend(hdc, ps.rcPaint.left, ps.rcPaint.top, width, height, hdcBmp, 0, 0, width, height, blend);
                assert(ok);
            }

            // clean up
            SelectObject(hdcBmp, prevObj);
            DeleteObject(bmpObj);
            DeleteDC(hdcBmp);
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

    static bool IsInsideEllipse(POINT pt, RECT ellipse) {
        POINT center{ (ellipse.right + ellipse.left) / 2, (ellipse.bottom + ellipse.top) / 2 }; // ellipse center
        POINT ext{ (ellipse.right - ellipse.left) / 2, (ellipse.bottom - ellipse.top) / 2 }; // ellipse extends

        POINT rel{ pt.x - center.x, pt.y - center.y }; // point relative to center

        // ellipse formula
        return (rel.x * rel.x) * 1.0f / (ext.x * ext.x) + (rel.y * rel.y) * 1.0f / (ext.y * ext.y) <= 1.0f;
    }
};
