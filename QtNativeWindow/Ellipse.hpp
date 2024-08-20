#pragma once
#include <cassert>
/* Ellipse drawing support functions. */


/** Convert red, green, blue & alpha values into a RGBQUAD with premultipled alpha channel. */
RGBQUAD RGBAPremult(BYTE r, BYTE g, BYTE b, BYTE a) {
    float alphaScale = a * 1.0f / 255; // in [0,1] range
    RGBQUAD res{};
    res.rgbRed = (BYTE)(r * alphaScale);
    res.rgbGreen = (BYTE)(g * alphaScale);
    res.rgbBlue = (BYTE)(b * alphaScale);
    res.rgbReserved = a;
    return res;
}

/** Check if a coordinate is inside an ellipse. */
static bool IsInsideEllipse(CPoint pt, CRect ellipse) {
    CPoint ext{ ellipse.Width()/2, ellipse.Height()/2}; // ellipse extends
    CPoint rel = pt - ellipse.CenterPoint(); // point relative to center

    // ellipse formula
    return (rel.x * rel.x)*1.0f/(ext.x * ext.x) + (rel.y * rel.y)*1.0f/(ext.y * ext.y) <= 1.0f;
}

/** Draw semi-transparent ellipse. */
class EllipseBmp {
public:
    EllipseBmp(HDC hdc, int width, int height) : m_hdcParent(hdc), m_width(width), m_height(height) {
        m_hdcBmp = CreateCompatibleDC(hdc);
        CreateBitmap();
    }

    ~EllipseBmp() {
        SelectObject(m_hdcBmp, m_prevObj);
        DeleteObject(m_bmpObj);
        DeleteDC(m_hdcBmp);
    }

    void Draw(RGBQUAD color) {
        // remove rect left/top offset before passing it to DrawEllipse
        RECT nonOffsetRect{};
        nonOffsetRect.left = 0;
        nonOffsetRect.top = 0;
        nonOffsetRect.right = m_width;
        nonOffsetRect.bottom = m_height;

        DrawEllipse(nonOffsetRect, RGBAPremult(color.rgbRed, color.rgbGreen, color.rgbBlue, color.rgbReserved));
    }

    void BlendInto(int xOrigin, int yOrigin) {
        BLENDFUNCTION blend = {};
        blend.BlendOp = AC_SRC_OVER;
        blend.BlendFlags = 0;
        blend.SourceConstantAlpha = 0xFF;
        blend.AlphaFormat = AC_SRC_ALPHA; // per-pixel alpha

        // copy bitmat to window buffer with per-pixel alpha blending
        BOOL ok = GdiAlphaBlend(m_hdcParent, xOrigin, yOrigin, m_width, m_height, m_hdcBmp, 0, 0, m_width, m_height, blend);
        assert(ok);
    }

private:
    void CreateBitmap() {
        BITMAPINFO bmi = {};
        bmi.bmiHeader.biSize = sizeof(BITMAPINFOHEADER);
        bmi.bmiHeader.biWidth = m_width;
        bmi.bmiHeader.biHeight = m_height;
        bmi.bmiHeader.biPlanes = 1;
        bmi.bmiHeader.biBitCount = 32;
        bmi.bmiHeader.biCompression = BI_RGB;
        bmi.bmiHeader.biSizeImage = m_width * m_height * 4;

        // create offscreen bitmap with premultiplied alpha channel
        m_bmpObj = CreateDIBSection(m_hdcBmp, &bmi, DIB_RGB_COLORS, (void**)&m_pixels, NULL, 0);
        m_prevObj = SelectObject(m_hdcBmp, m_bmpObj);
    }

    void DrawEllipse(RECT rect, RGBQUAD color) {
        // draw filled ellipse into bitmap
        // cannot use GDI Ellipse function here since it sets alpha=0 (see https://devblogs.microsoft.com/oldnewthing/20210915-00/?p=105687)
        for (int y = 0; y < m_height; y++) {
            for (int x = 0; x < m_width; x++) {
                POINT pt = { x, y };
                if (IsInsideEllipse({ x, y }, rect))
                    m_pixels[x + y * m_width] = color;
            }
        }
    }

    const HDC m_hdcParent = nullptr;
    const int m_width = 0;
    const int m_height = 0;
    HDC m_hdcBmp = nullptr;
    HBITMAP m_bmpObj = nullptr;
    HGDIOBJ m_prevObj = nullptr; // previous hdcBmp object (before bmpObj)
    RGBQUAD* m_pixels = nullptr; // pixel buffer in 0xAARRGGBB format
};
