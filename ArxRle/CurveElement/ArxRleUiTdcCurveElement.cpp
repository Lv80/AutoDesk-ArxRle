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
#include "ArxRle.h"

#if defined(_DEBUG) && !defined(AC_FULL_DEBUG)
#error _DEBUG should not be defined except in internal Adesk debug builds
#endif

#include "ArxRleUiTdcCurveElement.h"
#include "ArxRleUiTdmCurveElement.h"
#include "DbEntity\ArxRleEntityClass.h"



// ArxRleUiTdcCurveElement �Ի���

IMPLEMENT_DYNAMIC(ArxRleUiTdcCurveElement, CAcUiTabChildDialog)

ArxRleUiTdcCurveElement::ArxRleUiTdcCurveElement()
	: CAcUiTabChildDialog(NULL, ArxRleApp::m_hInstance)
	, m_openPathName(_T(""))
	, m_savePathName(_T(""))
{
}

ArxRleUiTdcCurveElement::~ArxRleUiTdcCurveElement()
{
	if(m_pGridCtrl != NULL) delete m_pGridCtrl;
}

void ArxRleUiTdcCurveElement::DoDataExchange(CDataExchange* pDX)
{
	CAcUiTabChildDialog::DoDataExchange(pDX);
	DDX_Control(pDX, ARXRLE_CURVE_ELEMENT_BN_OPENFILE, m_bnOpenFile);
	DDX_Control(pDX, ARXRLE_CURVE_ELEMENT_BN_SAVEFILE, m_bnSaveFile);
	DDX_Control(pDX, ARXRLE_CURVE_ELEMENT_SC_GRIDCTRL, m_gpGridCtrl);
	DDX_Text(pDX, ARXRLE_CURVE_ELEMENT_EB_OPENFILE, m_openPathName);
	DDX_Text(pDX, ARXRLE_CURVE_ELEMENT_EB_SAVEFILE, m_savePathName);
}


BEGIN_MESSAGE_MAP(ArxRleUiTdcCurveElement, CAcUiTabChildDialog)
	ON_WM_CREATE()
	ON_BN_CLICKED(ARXRLE_CURVE_ELEMENT_BN_OPENFILE, OnBnClickedOpenfile)
	ON_BN_CLICKED(ARXRLE_CURVE_ELEMENT_BN_CALCELEMENT, OnBnClickedCalcelement)
	ON_BN_CLICKED(ARXRLE_CURVE_ELEMENT_BN_SAVEFILE, OnBnClickedSavefile)
	ON_BN_CLICKED(ARXRLE_CURVE_ELEMENT_BN_DRAWCURVE, OnBnClickedDrawcurve)
	ON_BN_CLICKED(ARXRLE_CURVE_ELEMENT_BN_SHOWDATA, OnBnClickedShowdata)
	ON_BN_CLICKED(ARXRLE_CURVE_ELEMENT_BN_ADDROW, OnBnClickedAddrow)
	ON_BN_CLICKED(ARXRLE_CURVE_ELEMENT_BN_DELROW, OnBnClickedDelrow)
	ON_BN_CLICKED(ARXRLE_CURVE_ELEMENT_BN_SAVECHANGE, OnBnClickedSavechange)
END_MESSAGE_MAP()


// ArxRleUiTdcCurveElement ��Ϣ�������

BOOL ArxRleUiTdcCurveElement::OnInitDialog()
{
	CAcUiTabChildDialog::OnInitDialog();

	// ���ô˶Ի����ͼ�ꡣ��Ӧ�ó��������ڲ��ǶԻ���ʱ����ܽ��Զ�
	//  ִ�д˲���

	// TODO: �ڴ���Ӷ���ĳ�ʼ������
	HICON hIcon= AfxGetApp()->LoadIcon(ARXRLE_ICON_SELECT);
	m_bnOpenFile.SetIcon(hIcon);
	m_bnSaveFile.SetIcon(hIcon);

	CRect rc;
	m_gpGridCtrl.GetWindowRect(&rc);
	ScreenToClient(&rc);
	rc.DeflateRect(8,15,8,8);
	m_pGridCtrl->MoveWindow(rc);

	InitGridCtrl();

	return TRUE;  // ���ǽ��������õ��ؼ������򷵻� TRUE
}

int ArxRleUiTdcCurveElement::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CAcUiTabChildDialog::OnCreate(lpCreateStruct) == -1)
		return -1;

	// TODO:  �ڴ������ר�õĴ�������
	m_pGridCtrl = new CXTPGridControl;
	m_pGridCtrl->Create(CRect(),this,ARXRLE_CURVE_ELEMENT_GRIDCTRL);

	m_imgList.Create(ARXRLE_BMP_POKER, 16, 1, ILC_COLOR8 | ILC_MASK);
	m_pGridCtrl->SetImageList(&m_imgList);

	return 0;
}

void ArxRleUiTdcCurveElement::InitGridCtrl()
{
	m_pGridCtrl->SetEditable(TRUE);
	m_pGridCtrl->SetTextBkColor(RGB(0xFF, 0xFF, 0xE0));
	m_pGridCtrl->SetRowCount(1); //��ʼΪ1��
	m_pGridCtrl->SetColumnCount(15); //��ʼ��Ϊ5��
	m_pGridCtrl->SetFixedRowCount(1); //��ͷΪһ��
	m_pGridCtrl->SetFixedColumnCount(1); //��ͷΪһ�� 

	for (int row = 0; row < m_pGridCtrl->GetRowCount(); row++)
	{
		for (int col = 0; col < m_pGridCtrl->GetColumnCount(); col++)
		{ 
			//���ñ����ʾ����
			m_gvItem.mask = GVIF_TEXT|GVIF_FORMAT;
			m_gvItem.row = row;
			m_gvItem.col = col;
			if ( row == 0)//��(0��0)��
			{
				m_pGridCtrl->SetColumnWidth(col,60);//�п�
				if(col == 0)
				{
					m_gvItem.nFormat = DT_CENTER|DT_WORDBREAK;
					m_gvItem.strText.Format(_T("���JD"),col);
					m_pGridCtrl->SetColumnWidth(col,40);//�п�
				}
				else if(col == 1)
					m_gvItem.strText.Format(_T("���S"),col);
				else if(col == 2)
					m_gvItem.strText.Format(_T("����E/X"),col);
				else if(col == 3)
					m_gvItem.strText.Format(_T("����N/Y"),col);
				else if(col == 4)
					m_gvItem.strText.Format(_T("�뾶R"),col);
				else if(col == 5)
					m_gvItem.strText.Format(_T("����lo"),col);
				else if(col == 6)
					m_gvItem.strText.Format(_T("ת��W"),col);
				else if(col == 7)
					m_gvItem.strText.Format(_T("���߳�L"),col);
				else if(col == 8)
					m_gvItem.strText.Format(_T("���߳�T"),col);
				else if(col == 9)
					m_gvItem.strText.Format(_T("���ƾ�E"),col);
				else if(col == 10)
					m_gvItem.strText.Format(_T("ֱ��ZH"),col);
				else if(col == 11)
					m_gvItem.strText.Format(_T("��ԲHY"),col);
				else if(col == 12)
					m_gvItem.strText.Format(_T("����QZ"),col);
				else if(col == 13)
					m_gvItem.strText.Format(_T("Բ��YH"),col);
				else if(col == 14)
					m_gvItem.strText.Format(_T("��ֱHZ"),col);
			}
			else
			{
				m_gvItem.nFormat = DT_CENTER|DT_VCENTER|DT_SINGLELINE|DT_END_ELLIPSIS;

				if(col == 0)
					m_gvItem.strText.Format(_T("JD%d"),row);
				else
					m_gvItem.strText.Format(_T(""),2);
			}

			m_pGridCtrl->SetItem(&m_gvItem); 
			m_pGridCtrl->SetRowHeight(row, 25);//�и�
		}
	}
}

void ArxRleUiTdcCurveElement::OnBnClickedOpenfile()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	TCHAR szFilter[] = _T("�����ļ�|*.jd|�ı��ļ�|*.txt|�����ļ�|*.*||");
	CFileDialog dlg(TRUE,_T("jd"), NULL, 0, szFilter, this);
	if (dlg.DoModal() == IDOK)
	{
		CString pathName = dlg.GetPathName();
		POSITION po;
		po=dlg.GetStartPosition();//��ȡ��һ���ļ�λ��
		while(po)
		{
			pathName=dlg.GetNextPathName(po);//��ȡѡ���ļ��б��е���һ���ļ���
			m_openPathName=pathName;
			UpdateData(FALSE);
		}
		m_curveElement.InputJDData(m_openPathName);

		msgInf(L"�ļ��򿪳ɹ���");
	}
}

void ArxRleUiTdcCurveElement::OnBnClickedCalcelement()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	if(m_curveElement.m_vecJDElement.size() > 0)
	{
		m_curveElement.Calc_fwj_jdz();              //���㷽λ�ǡ�����
		m_curveElement.Calc_pj_L_A_p_q();            //����ת�ǡ����ƾ�
		m_curveElement.Calc_T_E();                   //�������߳�
		m_curveElement.Calc_ml();                    //�����������
		m_curveElement.Calc_ZH_HZ_xy();
		m_curveElement.Calc_HY_YH_xy();
		m_curveElement.Calc_QZ_xy();
		msgInf(L"������ϣ�");
	}
}

void ArxRleUiTdcCurveElement::OnBnClickedSavefile()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	TCHAR szFilter[] = _T("�����ļ�|*.jd|�ı��ļ�|*.txt|�����ļ�|*.*||");
	CFileDialog dlg(FALSE,_T("jd"), NULL, 0, szFilter, this);
	if (dlg.DoModal() == IDOK)
	{
		CString pathName = dlg.GetPathName();
		POSITION po;
		po=dlg.GetStartPosition();//��ȡ��һ���ļ�λ��
		while(po)
		{
			pathName=dlg.GetNextPathName(po);//��ȡѡ���ļ��б��е���һ���ļ���
			m_savePathName=pathName;
			UpdateData(FALSE);
		}
		m_curveElement.OutputJDResult(m_savePathName);

		msgInf(L"�ļ�����ɹ���");
	}
}

void ArxRleUiTdcCurveElement::OnBnClickedDrawcurve()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	if(m_curveElement.m_vecJDElement.size() == 0) return;

	//������
	AddLayer("����",3);
	SetCurLayer("����");
	for (int i= 0;i < m_curveElement.m_nJDNum-1; i++)
	{
		CPt3d pt1(m_curveElement.m_vecJDElement[i].m_x, m_curveElement.m_vecJDElement[i].m_y);
		CPt3d pt2(m_curveElement.m_vecJDElement[i+1].m_x, m_curveElement.m_vecJDElement[i+1].m_y);

		AddLine(pt1,pt2);
	}

	//��Բ����
	AddLayer("������",1);
	SetCurLayer("������");
	for (int i= 1;i < m_curveElement.m_nJDNum-1; i++)
	{
		double x1,y1,fwj1,qxj1,x2,y2,fwj2,qxj2;
		m_curveElement.ml_to_xy(m_curveElement.m_vecJDElement[i].m_HY,x1,y1,fwj1,qxj1);
		m_curveElement.ml_to_xy(m_curveElement.m_vecJDElement[i].m_QZ,x2,y2,fwj2,qxj2);

		// ���㷨����Բ��(��Բ����)

		CPt3d ptHY(x1,y1);
		CPt3d ptQZ(x2,y2);
		CPt3d ptYH(m_curveElement.m_vecJDElement[i].m_YH_x,m_curveElement.m_vecJDElement[i].m_YH_y);

		if (m_curveElement.m_vecJDElement[i].m_pj>0.0)
			AddArc(ptYH, ptQZ, ptHY);
		else
			AddArc(ptHY, ptQZ, ptYH);
	}

	//����������
	AddLayer("��������",4);
	SetCurLayer("��������");
	for (int i= 1;i < m_curveElement.m_nJDNum-1; i++)
	{
		//ǰ��
		double mile;
		mile=m_curveElement.m_vecJDElement[i].m_ZH;
		do 
		{
			double x1,y1,fwj1,qxj1,x2,y2,fwj2,qxj2;
			m_curveElement.ml_to_xy(mile,x1,y1,fwj1,qxj1);
			m_curveElement.ml_to_xy(mile+0.5,x2,y2,fwj2,qxj2);

			CPt3d pt1(x1,y1);
			CPt3d pt2(x2,y2);

			AddLine(pt1,pt2);

			mile+=0.5;
		} while (mile<m_curveElement.m_vecJDElement[i].m_HY);

		//��
		mile=m_curveElement.m_vecJDElement[i].m_YH+0.5;
		double x,y,fwj,qxj;
		m_curveElement.ml_to_xy(mile,x,y,fwj,qxj);
		CPt3d pt1(m_curveElement.m_vecJDElement[i].m_YH_x,m_curveElement.m_vecJDElement[i].m_YH_y);
		CPt3d pt2(x, y);
		AddLine(pt1,pt2);

		do 
		{
			double x1,y1,fwj1,qxj1,x2,y2,fwj2,qxj2;
			m_curveElement.ml_to_xy(mile,x1,y1,fwj1,qxj1);
			m_curveElement.ml_to_xy(mile+0.5,x2,y2,fwj2,qxj2);

			CPt3d pt1(x1,y1);
			CPt3d pt2(x2,y2);

			AddLine(pt1,pt2);

			mile+=0.5;
		} while (mile<m_curveElement.m_vecJDElement[i].m_HZ);	
	}

	//�������
	AddLayer("��ע",3);
	SetCurLayer("��ע");
	for (int i= 0;i < m_curveElement.m_nJDNum; i++)
	{
		double x,y,fwj,qxj;
		CString stat;
		if(i == 0)
		{
			m_curveElement.ml_to_xy(m_curveElement.m_vecJDElement[i].m_JD,x,y,fwj,qxj);
			stat = FormatStat(m_curveElement.m_vecJDElement[i].m_JD,1,L"QD  D");
		}
		else
		{
			m_curveElement.ml_to_xy(m_curveElement.m_vecJDElement[i].m_ZH,x,y,fwj,qxj);
			stat = FormatStat(m_curveElement.m_vecJDElement[i].m_ZH,1,L"ZH  D");
		}
		CPt3d pt1(x,y);
		CPt3d pt2 = CPt3d::GetLineEnd(pt1,10,qxj+PI/2);
		if(i < m_curveElement.m_nJDNum-1 )
		{
			AddLine(pt1,pt2);
			AddText(stat,pt2,2.5,qxj+PI/2,ArxLeftMiddle);
		}
		if(i == 0) continue;

		if(i == m_curveElement.m_nJDNum-1 )
		{
			m_curveElement.ml_to_xy(m_curveElement.m_vecJDElement[i].m_JD,x,y,fwj,qxj);
			stat = FormatStat(m_curveElement.m_vecJDElement[i].m_JD,1,L"ZD  D");
		}
		else
		{
			m_curveElement.ml_to_xy(m_curveElement.m_vecJDElement[i].m_HZ,x,y,fwj,qxj);
			stat = FormatStat(m_curveElement.m_vecJDElement[i].m_HZ,1,L"HZ  D");
		}
		pt1 = CPt3d(x,y);
		pt2 = CPt3d::GetLineEnd(pt1,10,qxj+PI/2);
		AddLine(pt1,pt2);
		AddText(stat,pt2,2.5,qxj+PI/2,ArxLeftMiddle);
		if(i == m_curveElement.m_nJDNum-1 ) continue;

		m_curveElement.ml_to_xy(m_curveElement.m_vecJDElement[i].m_HY-m_curveElement.m_vecJDElement[i].m_ls/2,x,y,fwj,qxj);
		stat = FormatStat(m_curveElement.m_vecJDElement[i].m_HY-m_curveElement.m_vecJDElement[i].m_ls/2);
		pt1 = CPt3d(x,y);
		pt2 = CPt3d::GetLineEnd(pt1,10,qxj+PI/2);
		AddLine(pt1,pt2);
		AddText(stat,pt2,2.5,qxj+PI/2,ArxLeftMiddle);
		if(i == m_curveElement.m_nJDNum-1 ) continue;

		m_curveElement.ml_to_xy(m_curveElement.m_vecJDElement[i].m_HY,x,y,fwj,qxj);
		stat = FormatStat(m_curveElement.m_vecJDElement[i].m_HY,1,L"HY  D");
		pt1 = CPt3d(x,y);
		pt2 = CPt3d::GetLineEnd(pt1,10,qxj+PI/2);
		AddLine(pt1,pt2);
		AddText(stat,pt2,2.5,qxj+PI/2,ArxLeftMiddle);
		if(i == m_curveElement.m_nJDNum-1 ) continue;

		m_curveElement.ml_to_xy(m_curveElement.m_vecJDElement[i].m_QZ,x,y,fwj,qxj);
		stat = FormatStat(m_curveElement.m_vecJDElement[i].m_QZ,1,L"QZ  D");
		pt1 = CPt3d(x,y);
		pt2 = CPt3d::GetLineEnd(pt1,10,qxj+PI/2);
		AddLine(pt1,pt2);
		AddText(stat,pt2,2.5,qxj+PI/2,ArxLeftMiddle);
		if(i == m_curveElement.m_nJDNum-1 ) continue;

		m_curveElement.ml_to_xy(m_curveElement.m_vecJDElement[i].m_YH,x,y,fwj,qxj);
		stat = FormatStat(m_curveElement.m_vecJDElement[i].m_YH,1,L"YH  D");
		pt1 = CPt3d(x,y);
		pt2 = CPt3d::GetLineEnd(pt1,10,qxj+PI/2);
		AddLine(pt1,pt2);
		AddText(stat, pt2,2.5,qxj+PI/2,ArxLeftMiddle);
		if(i == m_curveElement.m_nJDNum-1 ) continue;

		m_curveElement.ml_to_xy(m_curveElement.m_vecJDElement[i].m_YH+m_curveElement.m_vecJDElement[i].m_ls/2,x,y,fwj,qxj);
		stat = FormatStat(m_curveElement.m_vecJDElement[i].m_YH+m_curveElement.m_vecJDElement[i].m_ls/2);
		pt1 = CPt3d(x,y);
		pt2 = CPt3d::GetLineEnd(pt1,10,qxj+PI/2);
		AddLine(pt1,pt2);
		AddText(stat,pt2,2.5,qxj+PI/2,ArxLeftMiddle);
		if(i == m_curveElement.m_nJDNum-1 ) continue;
	}

	//д����Ҫ��
	for (int i= 1;i < m_curveElement.m_nJDNum-1; i++)
	{
		CPt3d pt1(m_curveElement.m_vecJDElement[i].m_x+10*cos(m_curveElement.m_vecJDElement[i].m_fwj-m_curveElement.m_vecJDElement[i+1].m_pj/2), m_curveElement.m_vecJDElement[i].m_y-10*sin(m_curveElement.m_vecJDElement[i].m_fwj-m_curveElement.m_vecJDElement[i+1].m_pj/2));
		CPt3d pt2(m_curveElement.m_vecJDElement[i].m_x+10*cos(m_curveElement.m_vecJDElement[i].m_fwj-m_curveElement.m_vecJDElement[i+1].m_pj/2), m_curveElement.m_vecJDElement[i].m_y-10*sin(m_curveElement.m_vecJDElement[i].m_fwj-m_curveElement.m_vecJDElement[i+1].m_pj/2)-5);
		CPt3d pt3(m_curveElement.m_vecJDElement[i].m_x+10*cos(m_curveElement.m_vecJDElement[i].m_fwj-m_curveElement.m_vecJDElement[i+1].m_pj/2), m_curveElement.m_vecJDElement[i].m_y-10*sin(m_curveElement.m_vecJDElement[i].m_fwj-m_curveElement.m_vecJDElement[i+1].m_pj/2)-10);
		CPt3d pt4(m_curveElement.m_vecJDElement[i].m_x+10*cos(m_curveElement.m_vecJDElement[i].m_fwj-m_curveElement.m_vecJDElement[i+1].m_pj/2), m_curveElement.m_vecJDElement[i].m_y-10*sin(m_curveElement.m_vecJDElement[i].m_fwj-m_curveElement.m_vecJDElement[i+1].m_pj/2)-15);
		CPt3d pt5(m_curveElement.m_vecJDElement[i].m_x+10*cos(m_curveElement.m_vecJDElement[i].m_fwj-m_curveElement.m_vecJDElement[i+1].m_pj/2), m_curveElement.m_vecJDElement[i].m_y-10*sin(m_curveElement.m_vecJDElement[i].m_fwj-m_curveElement.m_vecJDElement[i+1].m_pj/2)-20);
		CPt3d pt6(m_curveElement.m_vecJDElement[i].m_x+10*cos(m_curveElement.m_vecJDElement[i].m_fwj-m_curveElement.m_vecJDElement[i+1].m_pj/2), m_curveElement.m_vecJDElement[i].m_y-10*sin(m_curveElement.m_vecJDElement[i].m_fwj-m_curveElement.m_vecJDElement[i+1].m_pj/2)-25);
		CPt3d pt7(m_curveElement.m_vecJDElement[i].m_x+10*cos(m_curveElement.m_vecJDElement[i].m_fwj-m_curveElement.m_vecJDElement[i+1].m_pj/2), m_curveElement.m_vecJDElement[i].m_y-10*sin(m_curveElement.m_vecJDElement[i].m_fwj-m_curveElement.m_vecJDElement[i+1].m_pj/2)-30);
		CPt3d pt8(m_curveElement.m_vecJDElement[i].m_x+10*cos(m_curveElement.m_vecJDElement[i].m_fwj-m_curveElement.m_vecJDElement[i+1].m_pj/2), m_curveElement.m_vecJDElement[i].m_y-10*sin(m_curveElement.m_vecJDElement[i].m_fwj-m_curveElement.m_vecJDElement[i+1].m_pj/2)-35);

		CString str1;
		str1.Format(_T("%s"),m_curveElement.m_vecJDElement[i].m_JDNr);
		CString str2;
		str2.Format(_T("%.6f"),m_curveElement.m_vecJDElement[i].m_R);
		CString str3;
		str3.Format(_T("%.6f"),m_curveElement.m_vecJDElement[i].m_pj);
		CString str4;
		str4.Format(_T("%.6f"),m_curveElement.m_vecJDElement[i].m_L);
		CString str5;
		str5.Format(_T("%.6f"),m_curveElement.m_vecJDElement[i].m_ls);
		CString str6;
		str6.Format(_T("%.6f"),m_curveElement.m_vecJDElement[i].m_A);
		CString str7;
		str7.Format(_T("%.6f"),m_curveElement.m_vecJDElement[i].m_T);
		CString str8;
		str8.Format(_T("%.6f"),m_curveElement.m_vecJDElement[i].m_E);

		AddText(str1, pt1,2.5,0,ArxLeftMiddle);
		AddText(L"R="+str2, pt2,2.5,0,ArxLeftMiddle);
		AddText( L"Alfa="+str3, pt3,2.5,0,ArxLeftMiddle);
		AddText( L"L="+str4, pt4,2.5,0,ArxLeftMiddle);
		AddText( L"lo="+str5, pt5,2.5,0,ArxLeftMiddle);
		AddText( L"A="+str6, pt6,2.5,0,ArxLeftMiddle);
		AddText( L"T="+str7, pt7,2.5,0,ArxLeftMiddle);
		AddText( L"E="+str8, pt8,2.5,0,ArxLeftMiddle);
	}

	msgInf(L"���߻�����ϣ�");
}

void ArxRleUiTdcCurveElement::OnBnClickedShowdata()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	CString strGrid0,strGrid1,strGrid2,strGrid3,strGrid4;
	CString strGrid5,strGrid6,strGrid7,strGrid8,strGrid9;
	CString strGrid10,strGrid11,strGrid12,strGrid13,strGrid14;

	m_pGridCtrl->SetRowCount(m_curveElement.m_nJDNum+1);
	for (int row = 0; row < m_curveElement.m_nJDNum; row++)
	{
		strGrid0.Format(_T("%s"),m_curveElement.m_vecJDElement[row].m_JDNr);
		strGrid1.Format(_T("%.6f"),m_curveElement.m_vecJDElement[row].m_JD);
		strGrid2.Format(_T("%.6f"),m_curveElement.m_vecJDElement[row].m_x);
		strGrid3.Format(_T("%.6f"),m_curveElement.m_vecJDElement[row].m_y);
		strGrid4.Format(_T("%.6f"),m_curveElement.m_vecJDElement[row].m_R);
		strGrid5.Format(_T("%.6f"),m_curveElement.m_vecJDElement[row].m_ls);
		strGrid6.Format(_T("%s"),FormatAngle(m_curveElement.m_vecJDElement[row].m_pj*180/PI));
		strGrid7.Format(_T("%.6f"),m_curveElement.m_vecJDElement[row].m_L);
		strGrid8.Format(_T("%.6f"),m_curveElement.m_vecJDElement[row].m_T);
		strGrid9.Format(_T("%.6f"),m_curveElement.m_vecJDElement[row].m_E);
		strGrid10.Format(_T("%.6f"),m_curveElement.m_vecJDElement[row].m_ZH);
		strGrid11.Format(_T("%.6f"),m_curveElement.m_vecJDElement[row].m_HY);
		strGrid12.Format(_T("%.6f"),m_curveElement.m_vecJDElement[row].m_QZ);
		strGrid13.Format(_T("%.6f"),m_curveElement.m_vecJDElement[row].m_YH);
		strGrid14.Format(_T("%.6f"),m_curveElement.m_vecJDElement[row].m_HZ);

		m_pGridCtrl->SetItemText(row+1,0,strGrid0);
		m_pGridCtrl->SetItemText(row+1,1,strGrid1);
		m_pGridCtrl->SetItemText(row+1,2,strGrid2);
		m_pGridCtrl->SetItemText(row+1,3,strGrid3);
		m_pGridCtrl->SetItemText(row+1,4,strGrid4);
		m_pGridCtrl->SetItemText(row+1,5,strGrid5);
		m_pGridCtrl->SetItemText(row+1,6,strGrid6);
		m_pGridCtrl->SetItemText(row+1,7,strGrid7);
		m_pGridCtrl->SetItemText(row+1,8,strGrid8);
		m_pGridCtrl->SetItemText(row+1,9,strGrid9);
		m_pGridCtrl->SetItemText(row+1,10,strGrid10);
		m_pGridCtrl->SetItemText(row+1,11,strGrid11);
		m_pGridCtrl->SetItemText(row+1,12,strGrid12);
		m_pGridCtrl->SetItemText(row+1,13,strGrid13);
		m_pGridCtrl->SetItemText(row+1,14,strGrid14);

		m_pGridCtrl->SetItem(&m_gvItem); 
		m_pGridCtrl->SetRowHeight(row+1, 25);//�и�
		m_pGridCtrl->SetItemFormat(row+1,0,DT_CENTER);
		m_pGridCtrl->SetItemFormat(row+1,1,DT_CENTER);
		m_pGridCtrl->SetItemFormat(row+1,2,DT_CENTER);
		m_pGridCtrl->SetItemFormat(row+1,3,DT_CENTER);
		m_pGridCtrl->SetItemFormat(row+1,4,DT_CENTER);
		m_pGridCtrl->SetItemFormat(row+1,5,DT_CENTER);
		m_pGridCtrl->SetItemFormat(row+1,6,DT_CENTER);
		m_pGridCtrl->SetItemFormat(row+1,7,DT_CENTER);
		m_pGridCtrl->SetItemFormat(row+1,8,DT_CENTER);
		m_pGridCtrl->SetItemFormat(row+1,9,DT_CENTER);
		m_pGridCtrl->SetItemFormat(row+1,10,DT_CENTER);
		m_pGridCtrl->SetItemFormat(row+1,11,DT_CENTER);
		m_pGridCtrl->SetItemFormat(row+1,12,DT_CENTER);
		m_pGridCtrl->SetItemFormat(row+1,13,DT_CENTER);
		m_pGridCtrl->SetItemFormat(row+1,14,DT_CENTER);
	}
	m_pGridCtrl->Invalidate(FALSE);
}

void ArxRleUiTdcCurveElement::OnBnClickedAddrow()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	int nRow = m_pGridCtrl->GetFocusCell().row;
	CString tmp;
	if (nRow >= 0)
	{
		m_gvItem.row = nRow;
		tmp.Format(_T("JD%d"),nRow);
		m_pGridCtrl->InsertRow(tmp,nRow);
		for (int i = nRow; i<m_pGridCtrl->GetRowCount()+1;i++)
		{
			tmp.Format(_T("JD%d"),i + 1);
			m_pGridCtrl->SetItemText(i + 1,0,tmp);
		}
		m_pGridCtrl->SetRowHeight(nRow,30);
	}
	m_pGridCtrl->SetFocusCell(nRow,1);
	m_pGridCtrl->Invalidate();
}

void ArxRleUiTdcCurveElement::OnBnClickedDelrow()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	int nRow = m_pGridCtrl->GetFocusCell().row;
	if  (nRow == m_pGridCtrl->GetRowCount()-1) return;
	CString tmp;
	if (nRow >= 0)
	{
		m_pGridCtrl->DeleteRow(nRow);

		for (int i = nRow; i<m_pGridCtrl->GetRowCount()+1;i++)
		{
			tmp.Format(_T("JD%d"),i);
			m_pGridCtrl->SetItemText(i,0,tmp);
		}
		m_pGridCtrl->SetRowHeight(nRow,30);

	}
	m_pGridCtrl->SetFocusCell(nRow,1);
	m_pGridCtrl->Invalidate();
}

void ArxRleUiTdcCurveElement::OnBnClickedSavechange()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	int row = m_pGridCtrl->GetRowCount()-1;
	int col = m_pGridCtrl->GetColumnCount();
	m_curveElement.m_nJDNum=row;
	for (int i = 0; i < m_curveElement.m_nJDNum; i++)
	{
		m_curveElement.m_vecJDElement[i].m_JD = _tstof(m_pGridCtrl->GetItemText(i+1,1));
		m_curveElement.m_vecJDElement[i].m_x = _tstof(m_pGridCtrl->GetItemText(i+1,2));
		m_curveElement.m_vecJDElement[i].m_y = _tstof(m_pGridCtrl->GetItemText(i+1,3));
		m_curveElement.m_vecJDElement[i].m_R = _tstof(m_pGridCtrl->GetItemText(i+1,4));
		m_curveElement.m_vecJDElement[i].m_ls = _tstof(m_pGridCtrl->GetItemText(i+1,5));
	}
	m_curveElement.m_nJDNum = row;

	setlocale(LC_ALL,"");

	CStdioFile file;
	if(file.Open(m_openPathName, CFile::modeCreate | CFile::modeReadWrite))
	{
		file.Seek(CFile::begin,0);
		CString strTemp;
		for(int i = 0; i<m_curveElement.m_nJDNum;i++)
		{
			strTemp.Format(L"%5s;%10.3f;%10.4f;%10.4f;%10.3f;%10.3f\n",m_curveElement.m_vecJDElement[i].m_JD,m_curveElement.m_vecJDElement[i].m_JD,m_curveElement.m_vecJDElement[i].m_x,m_curveElement.m_vecJDElement[i].m_y,m_curveElement.m_vecJDElement[i].m_R,m_curveElement.m_vecJDElement[i].m_ls);
			file.WriteString(strTemp);
		}

		file.Close();
	}

	msgInf(L"�ļ�����ɹ���");
}