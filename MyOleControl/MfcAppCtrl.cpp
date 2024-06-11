#include "targetver.h"
#include "resource.h"   // main symbols
#include <afxctl.h>     // MFC support for ActiveX Controls
#include "MyOleControl.h"
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
IMPLEMENT_OLETYPELIB(CMfcAppCtrl, LIBID_MfcAppLib, 1, 0)

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
            IDB_MFCAPP,
            afxRegApartmentThreading,
            s_MfcAppOleMisc,
            LIBID_MfcAppLib,
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

BOOL CMfcAppCtrl::OnEraseBkgnd(CDC* pdc) {
    CRect area;
    GetClientRect(&area);

    // draw background
    CBrush brush(RGB(255, 255, 255));
    pdc->FillRect(&area, &brush);

    return 1; // background erased
}

// Drawing function
void CMfcAppCtrl::OnDraw(CDC* pdc, const CRect& rcBounds, const CRect& /* rcInvalid */) {
    if (!pdc)
        return;

    // draw filled ellipse
    pdc->Ellipse(rcBounds);

    // draw app name
    DrawTextW(pdc->m_hDC, L"MfcApp", -1, &const_cast<CRect&>(rcBounds), DT_CENTER | DT_VCENTER | DT_SINGLELINE);
}

BEGIN_INTERFACE_MAP(CMfcAppCtrl, COleControl)
END_INTERFACE_MAP()
