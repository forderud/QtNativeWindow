#pragma once
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
static bool IsInsideEllipse(POINT pt, RECT ellipse) {
    POINT center{ (ellipse.right + ellipse.left) / 2, (ellipse.bottom + ellipse.top) / 2 }; // ellipse center
    POINT ext{ (ellipse.right - ellipse.left) / 2, (ellipse.bottom - ellipse.top) / 2 }; // ellipse extends

    POINT rel{ pt.x - center.x, pt.y - center.y }; // point relative to center

    // ellipse formula
    return (rel.x * rel.x) * 1.0f / (ext.x * ext.x) + (rel.y * rel.y) * 1.0f / (ext.y * ext.y) <= 1.0f;
}
