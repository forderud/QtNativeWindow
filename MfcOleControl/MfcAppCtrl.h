#pragma once



class CMfcAppCtrl : public COleControl {
    DECLARE_DYNCREATE(CMfcAppCtrl)
public:
    CMfcAppCtrl();
    ~CMfcAppCtrl();

    BOOL PreCreateWindow(CREATESTRUCT& cs) override {
        cs.dwExStyle |= WS_EX_TRANSPARENT;
        return COleControl::PreCreateWindow(cs);
    }

    afx_msg BOOL OnEraseBkgnd(CDC* pdc);
    void OnDraw(CDC* dc, const CRect& bounds, const CRect& invalid) override;

protected:
    DECLARE_INTERFACE_MAP()

    DECLARE_OLECREATE_EX(CMfcAppCtrl)    // Class factory and guid
    DECLARE_OLETYPELIB(CMfcAppCtrl)      // GetTypeInfo
    DECLARE_OLECTLTYPE(CMfcAppCtrl)		// Type name and misc status

    DECLARE_MESSAGE_MAP()
    DECLARE_DISPATCH_MAP()
    DECLARE_EVENT_MAP()
};
