#pragma once
#include <functional>
#include <iostream>
#include <AppAPI/AppAPI.h> // IMeasurementSeries (generated from IDL)
#include <AppAPI/ComSupport.hpp>
#include "../../host/HostSupport/CoutLogger.hpp"


class ATL_NO_VTABLE PluginHostBasic :
    public CComObjectRootEx<CComMultiThreadModel>,
    public CComCoClass<PluginHostBasic>, // no registry entries
    public IPluginHostBasic {
public:
    PluginHostBasic () {
        m_logger = CreateLocalInstance<CoutLogger>();
    }

    /*NOT virtual*/ ~PluginHostBasic () {
    }

    void Initialize (std::function<void(DisplayMode)> window_resize) {
        m_window_resize = window_resize;
    }

    HRESULT GetHostInfo(BSTR* family_name, BSTR* model_name, unsigned short host_version[4]) override {
        if (!family_name || !model_name)
            return E_INVALIDARG;

        *family_name = CComBSTR(L"AtlMiniHost").Detach();
        *model_name = CComBSTR(L"E99").Detach();

        host_version[0] = 0; // major
        host_version[1] = 99; // minor
        host_version[2] = 0; // patch
        host_version[3] = 0; // build

        return S_OK;
    }

    HRESULT GetUserConfigPath(/*out*/BSTR * /*path*/) override {
        return E_NOTIMPL;
    }

    HRESULT GetGlobalConfigPath(/*out*/BSTR * path) override {
        return E_NOTIMPL;
    }

    HRESULT GetLogReceiver(IPluginLogger ** log) override {
        if (!log)
            return E_INVALIDARG;

        *log = CComPtr<IPluginLogger>(m_logger).Detach();
        return S_OK;
    }

    HRESULT GetCurrentUser(BSTR * /*username*/) override {
        return E_NOTIMPL;
    }

    HRESULT GetHostMessageReceiver(IHostMessage ** receiver) override {
        return E_NOTIMPL;
    }

    HRESULT SetSupportedDisplayModes(unsigned int display_modes) override {
        return S_OK; // pretend to support all modes to avoid signaling an error to the app
    }

    HRESULT RequestDisplayMode(DisplayMode mode) override {
        m_window_resize(mode);
        return S_OK;
    }

    HRESULT GetUserControls(IUserControls **) override {
        return E_NOTIMPL;
    }

    HRESULT SetSecondaryWindow(ULONGLONG /*wnd*/, DisplayMode /*display_mode*/) override {
        return E_NOTIMPL;
    }

    BEGIN_COM_MAP(PluginHostBasic)
        COM_INTERFACE_ENTRY(IPluginHostBasic)
    END_COM_MAP()

private:
    CComPtr<IPluginLogger>           m_logger;
    std::function<void(DisplayMode)> m_window_resize; // parent object callback
};
