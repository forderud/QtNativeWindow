#define _ATL_APARTMENT_THREADED
#define NOMINMAX

#include <atlstr.h>
#include "resource.h"
#include "CMainWindow.h"
#include <AppAPI/PluginQuery.hpp>


static std::vector<appapi::PluginAppControl> FilterCompatibleApps(std::vector<appapi::PluginAppControl> list, bool include_hidden) {
    std::vector<appapi::PluginAppControl> result;
    for (const auto & app : list) {
        if ((app.version_major != APPAPI_VERSION_MAJOR) || (app.version_minor != APPAPI_VERSION_MINOR))
            continue; // filter out incompatible apps

        if (!include_hidden && app.caps.hidden)
            continue; // filter out hidden apps

        result.push_back(app);
    }

    return result;
}


class AtlMiniHostModule : public CAtlExeModuleT<AtlMiniHostModule> {
public:
    HRESULT PreMessageLoop(int showCmd);
    HRESULT PostMessageLoop();

    CMainWindow m_mainwindow;
};

AtlMiniHostModule _AtlModule;


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
        // search for installed apps
        std::vector<appapi::PluginAppControl> installed_apps = FilterCompatibleApps(appapi::GetPluginAppControlList(false), false);

        // TODO: Replace hardcoded app selection with selection UI
        std::wstring clsid_str = installed_apps.back().clsid;
        clsid_str = L"{68B8E4BF-39BA-4C2B-B95A-DBAEF60C399A}"; // MfcSimpleViewer

        HRESULT hr = CLSIDFromString(clsid_str.c_str(), &clsid);
        if (FAILED(hr))
            return -1;
    }

    _AtlModule.m_mainwindow.m_ole_clsid = clsid;

    // Register TypeLib for out-of-process marshaling of AppAPI interfaces. Only need to be done once.
    // assumes AtlMiniHost.tlb in same folder as this EXE 
    HRESULT hr = _AtlModule.RegisterServer(TRUE);
    if (FAILED(hr)) {
        std::wcout << L"TypeLib registration error (must run as admin)." << std::endl;
        return -2;
    }

    int retval = _AtlModule.WinMain(SW_SHOWDEFAULT);
    return retval;
}

HRESULT AtlMiniHostModule::PreMessageLoop(int showCmd) {
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

HRESULT AtlMiniHostModule::PostMessageLoop() {
    return S_OK;
}
