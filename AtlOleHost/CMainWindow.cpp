#include "CMainWindow.h"


CMainWindow::CMainWindow() {
}

CMainWindow::~CMainWindow() {
}


LRESULT CMainWindow::OnDestroy(UINT nMsg, WPARAM wparam, LPARAM lparam, BOOL& handled) {
    PostQuitMessage(0);

    m_control.Release(); // close app (must be done before the destructor)
    return 0;
}

LRESULT CMainWindow::OnClose(UINT nMsg, WPARAM wparam, LPARAM lparam, BOOL &handled) {
    DefWindowProcW(); // call OnDestroy
    PostQuitMessage(0);
    return 0;
}

LRESULT CMainWindow::OnExit(WORD, UINT, HWND, BOOL&) {
    DestroyWindow(); // call OnDestroy
    return 0;
}


LRESULT CMainWindow::OnCreate(UINT nMsg, WPARAM wparam, LPARAM lparam, BOOL &handled) {
    ModifyStyleEx(0, WS_EX_COMPOSITED); // enable double-buffering

    {
        CComPtr<IGlobalOptions> globalOptions;
        CHECK(globalOptions.CoCreateInstance(CLSID_GlobalOptions, NULL, CLSCTX_INPROC_SERVER));
        // disable COM catching of exceptions
        CHECK(globalOptions->Set(COMGLB_EXCEPTION_HANDLING, COMGLB_EXCEPTION_DONOT_HANDLE_ANY));
    }

    // create OLE control
    // Security sandboxing can be implemented by impersonating low IL or AppContainer here and passing ClsContext=CLSCTX_LOCAL_SERVER|CLSCTX_ENABLE_CLOAKING
    HRESULT hr = m_control.CoCreateInstance(m_ole_clsid, nullptr, CLSCTX_INPROC_SERVER | CLSCTX_LOCAL_SERVER);
    if (FAILED(hr))
        abort();

    // create OLE child window & attach control
#ifdef USE_ALTERNATIVE_HOSTWINDOW
    CComObject<CCustomAxHostWindow> * tmp = nullptr;
    if (FAILED(CComObject<CCustomAxHostWindow>::CreateInstance(&tmp)))
        return E_FAIL;

    m_ole_window = tmp;
    HWND host_hwnd = m_ole_window->Create(m_hWnd, GetChildRect(Child::OLE), L"OLE control", WS_CHILD | WS_VISIBLE);
    m_ole_window->AttachControl(m_control.p, host_hwnd);
#else
    m_ole_window.Create(m_hWnd, _U_RECT(GetChildRect(Child::OLE)), nullptr, WS_CHILD | WS_VISIBLE);
    m_ole_window.SetWindowTextW(L"OLE control"); // set window name _after_ construction to avoid "Invalid class string" (0x800401f3 ) error from CreateWindowEx
    m_ole_window.AttachControl(m_control.p, /*out container IUnknown*/nullptr);
#endif

    // create sibling child window for focus testing
    m_trans_window.Create(m_hWnd, _U_RECT(GetChildRect(Child::Transparent)), L"Transparent window");
    m_layer_window.Create(m_hWnd, _U_RECT(GetChildRect(Child::Layered)), L"Layered window");

    return 0;
}

LRESULT CMainWindow::OnPaint(UINT nMsg, WPARAM wparam, LPARAM lparam, BOOL &handled) {
    static uint8_t counter = 0;
    counter = (counter + 4) % 128;
    HBRUSH brush = CreateSolidBrush(RGB(64+counter, 64+counter, 64+counter));

    // draw background in dynamic color
    PAINTSTRUCT ps = {};
    HDC hdc = BeginPaint(&ps);
    FillRect(hdc, &ps.rcPaint, brush);
    EndPaint(&ps);

    DeleteObject(brush);

    return 0;
}

LRESULT CMainWindow::OnSize(UINT nMsg, WPARAM wparam, LPARAM lparam, BOOL &handled) {
#ifdef USE_ALTERNATIVE_HOSTWINDOW
    m_ole_window->MoveWindow(GetChildRect(Child::OLE));
#else
    m_ole_window.MoveWindow(GetChildRect(Child::OLE));
#endif

    m_trans_window.MoveWindow(GetChildRect(Child::Transparent));
    m_layer_window.MoveWindow(GetChildRect(Child::Layered));

    return 0;
}


// Utility functions

CRect CMainWindow::GetChildRect(Child c) const {
    CRect parent;
    GetClientRect(&parent);

    if (c == Child::OLE) {
        // left three-quarters of window
        CRect rect = { parent.left, parent.top, 3*parent.Width()/4, parent.bottom };
        rect.DeflateRect({2, 2}); // shrink to add border
        return rect;
    } else if (c == Child::Transparent) {
        // top right quarter of window
        CRect rect = { 3 * parent.Width() / 4, parent.top, parent.right, (parent.top + parent.bottom)/2 };

        rect.DeflateRect({ 2, 2 }); // shrink to add border
        return rect;
    } else if (c == Child::Layered) {
        // bottom right quarter of window
        CRect rect = { 3 * parent.Width() / 4, (parent.top + parent.bottom) / 2, parent.right, parent.bottom };
        rect.DeflateRect({ 2, 2 }); // shrink to add border
        return rect;
    }

    abort();
}
