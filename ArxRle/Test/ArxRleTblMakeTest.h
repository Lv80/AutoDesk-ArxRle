//
//////////////////////////////////////////////////////////////////////////////
//
//  Copyright 2015 Autodesk, Inc.  All rights reserved.
//
//  Use of this software is subject to the terms of the Autodesk license 
//  agreement provided at the time of installation or download, or which 
//  otherwise accompanies this software in either electronic or hard copy form.   
//
//////////////////////////////////////////////////////////////////////////////
//

#ifndef ARXRLE_TBLMAKETEST_H
#define ARXRLE_TBLMAKETEST_H

#include "Resource.h"

/****************************************************************************
**
**    CLASS ArxRleTblMakeDbox:
**
**    **jma
**
*************************************/

class ArxRleTblMakeDbox : public CDialog {

public:
                ArxRleTblMakeDbox(CWnd* pParent);

    virtual INT_PTR    DoModal();

    // Dialog Data
    //{{AFX_DATA(ArxRleTblMakeDbox)
        // NOTE: the ClassWizard will add data members here
    //}}AFX_DATA


    // Overrides
    // ClassWizard generated virtual function overrides
    //{{AFX_VIRTUAL(ArxRleTblMakeDbox)
    protected:
    virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
    //}}AFX_VIRTUAL

    virtual BOOL OnCmdMsg(UINT nID, int nCode, void* pExtra,
                AFX_CMDHANDLERINFO* pHandlerInfo);

protected:

    // Generated message map functions
    //{{AFX_MSG(ArxRleTblMakeDbox)
    afx_msg void OnButtonClicked();
    //}}AFX_MSG
    DECLARE_MESSAGE_MAP()

private:
        // data members
    int    m_buttonId;

        // callback functions
    void    makeNamedBlock();
    void    makeAnonymousBlock();
    void    makeDimstyle();
    void    makeLayer();
    void    makeLinetype();
    void    makeTextstyle();
    void    makeAppid();
    void    makeUcs();
    void    makeView();
    void    makeViewport();

    BOOL    setSymbolName(AcDbSymbolTableRecord* newRec, LPCTSTR newName);
    BOOL    addToSymbolTableAndClose(AcDbSymbolTableRecord* symRec);
};

#endif    // ARXRLE_TBLMAKETEST_H
