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
#include "StdAfx.h"

#if defined(_DEBUG) && !defined(AC_FULL_DEBUG)
#error _DEBUG should not be defined except in internal Adesk debug builds
#endif

#include "ArxRleUiPrDistDef.h"



/****************************************************************************
**
**  ArxRleUiPrDistDef::ArxRleUiPrDistDef
**
**  **jma
**
*************************************/

ArxRleUiPrDistDef::ArxRleUiPrDistDef(LPCTSTR msg, LPCTSTR keyWordList, DistType type, double def)
:   ArxRleUiPrDist(msg, keyWordList, type)
{
    m_default = def;
}

/****************************************************************************
**
**  ArxRleUiPrDistDef::ArxRleUiPrDistDef
**
**  **jma
**
*************************************/

ArxRleUiPrDistDef::ArxRleUiPrDistDef(LPCTSTR msg, LPCTSTR keyWordList, DistType type,
            const AcGePoint3d& basePt, double def)
:   ArxRleUiPrDist(msg, keyWordList, type, basePt)
{
    m_default = def;
}

/****************************************************************************
**
**  ArxRleUiPrDistDef::~ArxRleUiPrDistDef
**
**  **jma
**
*************************************/

ArxRleUiPrDistDef::~ArxRleUiPrDistDef()
{
}

/****************************************************************************
**
**  ArxRleUiPrDistDef::go
**
**  **jma
**
*************************************/

ArxRleUiPrBase::Status
ArxRleUiPrDistDef::go()
{
    CString prompt;
    TCHAR defStr[512];
    int initFlag;

        // set up prompt
    acdbRToS(m_default, m_unit, m_precision, defStr);
    prompt.Format(_T("\n%s<%s>: "), message(), defStr);

        // set up init flag
    if (m_distType == kNoZero)
        initFlag = RSG_NOZERO;
    else if (m_distType == kNoNeg)
        initFlag = RSG_NONEG;
    else if (m_distType == kNoNegNoZero)
        initFlag = RSG_NOZERO+RSG_NONEG;
    else
        initFlag = 0;

    int result;
    while (1) {
        acedInitGet(initFlag, keyWords());
        if (m_useBasePt)
            result = acedGetDist(asDblArray(m_basePt), prompt, &m_value);
        else
            result = acedGetDist(NULL, prompt, &m_value);

        if (result == RTNORM){
            if (inRange())
                return ArxRleUiPrBase::kOk;
        }
        else if (result == RTKWORD) {
            const size_t kBufSize = 512;
            acedGetInput(m_keyWordPicked.GetBuffer(kBufSize), kBufSize);
            m_keyWordPicked.ReleaseBuffer();
            return ArxRleUiPrBase::kKeyWord;
        }
        else if (result == RTNONE) {
            if (m_distType == ArxRleUiPrDist::kRange) {
                ASSERT(m_minVal != m_maxVal);    // make sure they set ranges!
                ASSERT((m_default >= m_minVal) && (m_default <= m_maxVal));
            }
            m_value = m_default;
            return ArxRleUiPrBase::kOk;
        }
        else
            return ArxRleUiPrBase::kCancel;
    }
}





