#include "targetver.h"
#include <afxctl.h>     // MFC support for ActiveX Controls
#include "MfcOleControl_i.c"

class CMfcAppApp : public COleControlModule {
};

CMfcAppApp theApp;

// Adds entries to the system registry
STDAPI DllRegisterServer() {
    AFX_MANAGE_STATE(_afxModuleAddrThis);

    if (!AfxOleRegisterTypeLib(AfxGetInstanceHandle(), LIBID_MfcOleLib))
        return ResultFromScode(SELFREG_E_TYPELIB);

    if (!COleObjectFactoryEx::UpdateRegistryAll(TRUE))
        return ResultFromScode(SELFREG_E_CLASS);

    return NOERROR;
}



// Removes entries from the system registry
STDAPI DllUnregisterServer() {
    AFX_MANAGE_STATE(_afxModuleAddrThis);

    if (!AfxOleUnregisterTypeLib(LIBID_MfcOleLib, 1, 0))
        return ResultFromScode(SELFREG_E_TYPELIB);

    if (!COleObjectFactoryEx::UpdateRegistryAll(FALSE))
        return ResultFromScode(SELFREG_E_CLASS);

    return NOERROR;
}
