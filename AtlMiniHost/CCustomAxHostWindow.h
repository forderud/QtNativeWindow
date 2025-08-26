#pragma once
#include <iostream>

/// Host window class similar how CAxHostWindow is derived and used in EchoPluginHost
class ATL_NO_VTABLE CCustomAxHostWindow : public CAxHostWindow {
public:
    CCustomAxHostWindow() {
    }

    ~CCustomAxHostWindow() override {
    }

    DECLARE_WND_CLASS(_T("CCustomAxHostWindow"))
    DECLARE_PROTECT_FINAL_CONSTRUCT()
    BEGIN_COM_MAP(CCustomAxHostWindow)
        COM_INTERFACE_ENTRY(IOleClientSite) // First entry of the map should be a simple entry. See atlcom.h: ATLASSERT(pEntries->pFunc == _ATL_SIMPLEMAPENTRY);
        COM_INTERFACE_ENTRY_CHAIN(CAxHostWindow)
    END_COM_MAP()

    BEGIN_MSG_MAP(CCustomAxHostWindow)
        CHAIN_MSG_MAP(CAxHostWindow)
    END_MSG_MAP()

    /// Override OnFocus and TranslateAccelerator for testing focus and keyboard handling
    HRESULT OnFocus(BOOL fGotFocus) override {
        std::wcout << L"Host: OnFocus " << (fGotFocus ? L"true" : L"false") << std::endl;
        return S_FALSE;
    }

    /// https://learn.microsoft.com/en-us/windows/win32/api/ocidl/nf-ocidl-iolecontrolsite-translateaccelerator
    /// Is called by ActiveQt controls for out-of-process key events, that is not handled in app.
    /// Return
    ///     S_OK: The container processed the message
    ///     S_FALSE: The container did not process the message.
    HRESULT TranslateAccelerator(LPMSG lpMsg, DWORD grfModifiers) override {
        if (lpMsg->message != WM_KEYDOWN)
            return S_FALSE;

        DWORD dwKeyMod = 0;
        if (::GetKeyState(VK_SHIFT) < 0)
            dwKeyMod |= 1;  // KEYMOD_SHIFT
        if (::GetKeyState(VK_CONTROL) < 0)
            dwKeyMod |= 2;  // KEYMOD_CONTROL
        if (::GetKeyState(VK_MENU) < 0)
            dwKeyMod |= 4;  // KEYMOD_ALT

        switch (LOWORD(lpMsg->wParam)) {
        case VK_F1:
            std::wcout << L"Container got keycode F1, 'handled it'" << std::endl;
            return S_OK;
            break;
        default:
            wchar_t name[1024] = {};
            UINT scanCode = MapVirtualKeyW(LOWORD(lpMsg->wParam), MAPVK_VK_TO_VSC);
            LONG lParamValue = (scanCode << 16);
            int result = GetKeyNameTextW(lParamValue, name, static_cast<int>(std::size(name)));

            if (result > 0)
                std::wcout << L"Container got keycode " << name << L" but chose to not handle it" << std::endl;

            break;
        }

        return S_FALSE;
    }
};
