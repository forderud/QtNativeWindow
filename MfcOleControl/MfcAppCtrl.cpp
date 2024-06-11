#include "targetver.h"
#include "resource.h"   // main symbols
#include <afxctl.h>     // MFC support for ActiveX Controls
#include "MfcOleControl_h.h"
#include "MfcAppCtrl.h"
#include "afxdialogex.h"


IMPLEMENT_DYNCREATE(CMfcAppCtrl, COleControl)

BEGIN_MESSAGE_MAP(CMfcAppCtrl, COleControl)
    ON_WM_ERASEBKGND()
    ON_OLEVERB(AFX_IDS_VERB_PROPERTIES, OnProperties)
END_MESSAGE_MAP()

BEGIN_DISPATCH_MAP(CMfcAppCtrl, COleControl)
END_DISPATCH_MAP()

BEGIN_EVENT_MAP(CMfcAppCtrl, COleControl)
END_EVENT_MAP()

// Initialize class factory and guid
IMPLEMENT_OLECREATE_NOREGNAME(CMfcAppCtrl, __uuidof(MfcApp).Data1, __uuidof(MfcApp).Data2, __uuidof(MfcApp).Data3,
    __uuidof(MfcApp).Data4[0], __uuidof(MfcApp).Data4[1], __uuidof(MfcApp).Data4[2], __uuidof(MfcApp).Data4[3], __uuidof(MfcApp).Data4[4], __uuidof(MfcApp).Data4[5], __uuidof(MfcApp).Data4[6], __uuidof(MfcApp).Data4[7])

// Type library ID and version
IMPLEMENT_OLETYPELIB(CMfcAppCtrl, LIBID_MfcOleLib, 1, 0)

// Control type information
static const DWORD s_MfcAppOleMisc =
    OLEMISC_ACTIVATEWHENVISIBLE |
    OLEMISC_SETCLIENTSITEFIRST |
    OLEMISC_INSIDEOUT |
    OLEMISC_CANTLINKINSIDE |
    OLEMISC_RECOMPOSEONRESIZE;

IMPLEMENT_OLECTLTYPE(CMfcAppCtrl, IDS_MFCAPP, s_MfcAppOleMisc)

// Adds or removes system registry entries for CMfcAppCtrl
BOOL CMfcAppCtrl::CMfcAppCtrlFactory::UpdateRegistry(BOOL bRegister) {
    // TODO: Verify that your control follows apartment-model threading rules.
    // Refer to MFC TechNote 64 for more information.
    // If your control does not conform to the apartment-model rules, then
    // you must modify the code below, changing the 6th parameter from
    // afxRegApartmentThreading to 0.

    if (bRegister) {
        return AfxOleRegisterControlClass(
            AfxGetInstanceHandle(),
            m_clsid,
            m_lpszProgID,
            IDS_MFCAPP,
            0, // bitmap
            afxRegApartmentThreading,
            s_MfcAppOleMisc,
            LIBID_MfcOleLib,
            1, 0);
    } else {
        return AfxOleUnregisterClass(m_clsid, m_lpszProgID);
    }
}


CMfcAppCtrl::CMfcAppCtrl() {
    InitializeIIDs(&__uuidof(DMfcAppPrimary), &__uuidof(DMfcAppEvents));
    // TODO: Initialize your control's instance data here.

    SetText(L"CMfcAppCtrl"); // name window handle
}

CMfcAppCtrl::~CMfcAppCtrl() {
    // TODO: Cleanup your control's instance data here.
}

BOOL CMfcAppCtrl::OnEraseBkgnd(CDC* dc) {
#ifdef TRANSPARENT_OLE_CONTROL
    dc; // mute warning
    return 0; // background not erased
#else
    CRect area;
    GetClientRect(&area);

    // draw background
    CBrush brush(RGB(255, 255, 255));
    dc->FillRect(&area, &brush);

    return 1; // background erased
#endif
}

// Drawing function
void CMfcAppCtrl::OnDraw(CDC* dc, const CRect& bounds, const CRect& /*invalid*/) {
    if (!dc)
        return;

    // draw filled ellipse
    dc->Ellipse(bounds);

    // draw app name
    DrawTextW(dc->m_hDC, L"MFC OLE control", -1, &const_cast<CRect&>(bounds), DT_CENTER | DT_VCENTER | DT_SINGLELINE);
}

BEGIN_INTERFACE_MAP(CMfcAppCtrl, COleControl)
END_INTERFACE_MAP()
