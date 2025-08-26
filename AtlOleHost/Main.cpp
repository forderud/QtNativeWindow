#define _ATL_APARTMENT_THREADED
#define NOMINMAX

#include <atlstr.h>
#include "resource.h"
#include "CMainWindow.h"


class AtlOleHostModule : public CAtlExeModuleT<AtlOleHostModule> {
public:
    HRESULT PreMessageLoop(int showCmd);
    HRESULT PostMessageLoop();

    CMainWindow m_mainwindow;
};

AtlOleHostModule _AtlModule;


// Starts up via AtlExeModule
int WINAPI wmain(int argc, wchar_t * argv[]) {
    CLSID clsid = {};
    if (argc >= 2) {
        // check for CLSID passed as argument
        HRESULT hr = CLSIDFromString(argv[1], &clsid);
        if (FAILED(hr)) {
            // check for ProgID passed as argument
            HRESULT hr = CLSIDFromProgID(argv[1], &clsid);
            if (FAILED(hr))
                return -1;
        }
    } else {
        // TODO: Replace hardcoded app selection with selection UI
        std::wstring clsid_str = L"{45C33494-127D-4AEA-B7EB-63A203D69E8A}"; // MfcOleControl
        HRESULT hr = CLSIDFromString(clsid_str.c_str(), &clsid);
        if (FAILED(hr))
            return -1;
    }

    _AtlModule.m_mainwindow.m_ole_clsid = clsid;

    int retval = _AtlModule.WinMain(SW_SHOWDEFAULT);
    return retval;
}

HRESULT AtlOleHostModule::PreMessageLoop(int showCmd) {
    CString app_title;
    if (!app_title.LoadStringW(IDS_APP_TITLE))
        return E_FAIL;

    HMENU hMenu = LoadMenuW(_pModule->GetResourceInstance(), MAKEINTRESOURCEW(IDR_MENU));
    if (hMenu == 0)
        return E_FAIL;

    if (!m_mainwindow.Create(0, CWindow::rcDefault, app_title, 0, 0, hMenu))
        return E_FAIL;

    m_mainwindow.ShowWindow(showCmd);
    m_mainwindow.CenterWindow();

    return S_OK;
}

HRESULT AtlOleHostModule::PostMessageLoop() {
    return S_OK;
}
