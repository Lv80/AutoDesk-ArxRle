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

#ifndef ARXRLEDOCLOCKWRITE_H
#define ARXRLEDOCLOCKWRITE_H

/****************************************************************************
**
**  CLASS ArxRleDocLockWrite:
**
**  **jma
**
*************************************/

class ArxRleDocLockWrite {

public:
					ArxRleDocLockWrite();
					ArxRleDocLockWrite(AcDbDatabase* db);
    virtual			~ArxRleDocLockWrite();

    virtual Acad::ErrorStatus   lock();
    virtual Acad::ErrorStatus   lockAndSetCurrent();

    virtual Acad::ErrorStatus   lock(AcDbDatabase* nextDb);
    virtual Acad::ErrorStatus   lockAndSetCurrent(AcDbDatabase* nextDb);

    virtual Acad::ErrorStatus   unlock();
    virtual Acad::ErrorStatus   unlockAndRestoreCurrent();

private:
        // member data
    AcDbDatabase*       m_db;
    AcApDocument*       m_doc;
    bool                m_didLock;
	bool				m_didSwitch;
    AcApDocument*       m_curDocWhenStarted;
};


#endif    // ARXRLEDOCLOCKWRITE_H
