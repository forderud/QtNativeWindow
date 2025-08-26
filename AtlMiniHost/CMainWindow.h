#pragma once
#include "resource.h"
#include "LayeredWindow.hpp"
#include "TransparentWindow.hpp"
#include "CCustomAxHostWindow.h"

#if 0
#define USE_ALTERNATIVE_HOSTWINDOW
#endif

class CMainWindow : public CWindowImpl<CMainWindow, CWindow, CFrameWinTraits> {
public:
    CMainWindow();
    ~CMainWindow() override;

private:
    BEGIN_MSG_MAP(CMainWindow)
        MESSAGE_HANDLER(WM_CREATE, OnCreate)
        MESSAGE_HANDLER(WM_PAINT, OnPaint)
        MESSAGE_HANDLER(WM_SIZE, OnSize)
        MESSAGE_HANDLER(WM_DESTROY, OnDestroy)
        MESSAGE_HANDLER(WM_CLOSE, OnClose)
        COMMAND_HANDLER(IDM_EXIT, 0, OnExit)
        MESSAGE_HANDLER(WM_LBUTTONDOWN, OnLButtonDown)
    END_MSG_MAP()

    // Message Handlers
    LRESULT OnCreate(UINT nMsg, WPARAM wparam, LPARAM lparam, BOOL &handled);
    LRESULT OnPaint(UINT nMsg, WPARAM wparam, LPARAM lparam, BOOL &handled);
    LRESULT OnSize(UINT nMsg, WPARAM wparam, LPARAM lparam, BOOL &handled);
    LRESULT OnDestroy(UINT nMsg, WPARAM wparam, LPARAM lparam, BOOL &handled);
    LRESULT OnClose(UINT nMsg, WPARAM wparam, LPARAM lparam, BOOL &handled);
    // Command handlers
    LRESULT OnExit(WORD, UINT, HWND, BOOL&);

    LRESULT OnLButtonDown(UINT, WPARAM, LPARAM, BOOL& handled) {
        wprintf(L"CMainWindow WM_LBUTTONDOWN.\n");
        handled = FALSE; // don't absorb message
        return 0;
    }

    // Utility functions
    enum class Child {
        OLE, ///< main OLE control
        Transparent, ///< transparent sibling window
        Layered, ///< transparend sibling window
    };

    CRect GetChildRect(Child c) const;

    CLSID             m_ole_clsid = {};

private:
    CComPtr<IUnknown> m_control;                  ///< OLE control
#ifdef USE_ALTERNATIVE_HOSTWINDOW
    CComPtr<CCustomAxHostWindow> m_ole_window;   ///< parent window for OLE control
#else
    CAxWindow         m_ole_window;
#endif
    TransparentWindow m_trans_window; ///< Non-OLE child window for testing transparency and mouse/keyboard focus.
    LayeredWindow     m_layer_window; ///< Non-OLE child window for testing transparency and mouse/keyboard focus.
};
