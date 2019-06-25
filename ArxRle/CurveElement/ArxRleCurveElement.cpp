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

#include "ArxRleCurveElement.h"
#include "DbEntity/ArxRleEntityClass.h"


ArxRleCurveElement::ArxRleCurveElement()
{
	m_nJDNum = 0;
	m_vecJDElement.clear();

	m_nSQXNum = 0;
	m_vecSQXElement.clear();
}

ArxRleCurveElement::~ArxRleCurveElement(void)
{
}

void ArxRleCurveElement::InputJDData(CString inputfilename)
{
	if(PathFileExists(inputfilename))
	{
		m_vecJDElement.clear();
		m_nJDNum = 0;

		setlocale(LC_ALL,"");

		CStdioFile file;
		if(file.Open(inputfilename, CFile::modeRead))
		{
			int nPos = 0;
			CString strLine;
			while(file.ReadString(strLine))
			{
				strLine.Trim();
				strLine.Replace(_T("；"), _T(";"));
				strLine.Replace(_T(","), _T(";"));
				strLine.Replace(_T("，"), _T(";"));
				if(strLine.GetLength() == 0)
					continue;

				CStringArray arrSubStr;
				int cnt = ParseSubStrByDelimiter(strLine, arrSubStr, ';');

				if(cnt == 6)
				{
					JDELEMENT jd;
					jd.m_JDNr = arrSubStr[0];
					jd.m_JD = _tstof(arrSubStr[1]);
					jd.m_x = _tstof(arrSubStr[2]);
					jd.m_y = _tstof(arrSubStr[3]);
					jd.m_R = _tstof(arrSubStr[4]);
					jd.m_ls = _tstof(arrSubStr[5]);

					m_vecJDElement.push_back(jd);
					m_nJDNum++;
				}
			}

			file.Close();
		}
	}
}

void ArxRleCurveElement::InputSQXData(CString filePath)
{
	if(PathFileExists(filePath))
	{
		m_nSQXNum = 0;
		m_vecSQXElement.clear();

		setlocale(LC_ALL,"");
		CStdioFile file;
		if(file.Open(filePath, CFile::modeRead))
		{
			CString str;
			int nPos = 0;
			while(file.ReadString(str))
			{
				str.Trim();
				SQXELEMENT sqx;
				if((nPos = str.Find(_T("S"))) >= 0) sqx.m_S = _tstof(str.Mid(nPos+2));
				if((nPos = str.Find(_T("H"))) >= 0) sqx.m_H = _tstof(str.Mid(nPos+2));
				if((nPos = str.Find(_T("R"))) >= 0) sqx.m_R = _tstof(str.Mid(nPos+2));
				if((nPos = str.Find(_T("I1"))) >= 0) sqx.m_I1 = _tstof(str.Mid(nPos+3))/1000;
				if((nPos = str.Find(_T("I2"))) >= 0) sqx.m_I2 = _tstof(str.Mid(nPos+3))/1000;
				sqx.m_L = fabs(sqx.m_R * (sqx.m_I1 - sqx.m_I2));
				sqx.m_T = sqx.m_L / 2;
				if(sqx.m_R != 0) sqx.m_E = sqx.m_T * sqx.m_T / 2 / sqx.m_R;

				m_vecSQXElement.push_back(sqx);
				m_nSQXNum++;
			}

			file.Close();
		}
	}
}

void ArxRleCurveElement::ReadMDBTable(CString mdbFname,CString TabFname)
{
	CoInitialize(NULL);//VS2005用这个函数

	m_pConnection.CreateInstance(__uuidof(XTPREPORTMSADODB::Connection));
	m_pRecordset.CreateInstance(__uuidof(XTPREPORTMSADODB::Recordset));

	m_strConnect.Format(L"Provider=Microsoft.Jet.OLEDB.4.0;Data Source=%s",mdbFname);
	//m_strSQL ="select * from 交点数据";
	m_strSQL.Format(L"select * from %s",TabFname);

	try
	{	
		m_pConnection->Open(_bstr_t(m_strConnect),"","",XTPREPORTMSADODB::adModeUnknown);
	}
	catch (_com_error e)
	{
		AfxMessageBox(_T("数据库连接失败!"));
	}

	try
	{
		m_pRecordset->Open(_bstr_t(m_strSQL),
			m_pConnection.GetInterfacePtr(),
			XTPREPORTMSADODB::adOpenStatic,
			XTPREPORTMSADODB::adLockOptimistic,
			XTPREPORTMSADODB::adCmdText);
	}
	catch (_com_error e)
	{
		AfxMessageBox(e.ErrorMessage());                     
	}

	try
	{
		m_nJDNum = m_pRecordset->GetRecordCount();
		for (int i=0; i<m_nJDNum; i++)
		{				
			_variant_t var1 = m_pRecordset->GetCollect("交点编号");
			_variant_t var2 = m_pRecordset->GetCollect("交点里程");
			_variant_t var3 = m_pRecordset->GetCollect("交点坐标E/X");
			_variant_t var4 = m_pRecordset->GetCollect("交点坐标N/Y");
			_variant_t var5 = m_pRecordset->GetCollect("圆曲线半径");
			_variant_t var6 = m_pRecordset->GetCollect("缓和曲线长");

			JDELEMENT jd;
			jd.m_JDNr = _bstr_t(var1).GetBSTR();
			jd.m_JD = atof(_bstr_t(var2));
			jd.m_x = atof(_bstr_t(var3));
			jd.m_y = atof(_bstr_t(var4));
			jd.m_R = atof(_bstr_t(var5));
			jd.m_ls = atof(_bstr_t(var6));
	
			m_vecJDElement.push_back(jd);

			m_pRecordset->MoveNext();
		}
	}
	catch (_com_error e)
	{
		AfxMessageBox(e.ErrorMessage());
	}

	m_pRecordset->Close();
	m_pConnection->Close();
}

void ArxRleCurveElement::Calc_fwj_jdz()
{
	int i;
	for (i= 0;i < m_nJDNum-1; i++)
	{
		double dx=m_vecJDElement[i+1].m_x-m_vecJDElement[i].m_x;//两点之间的X,Y坐标的差值
		double dy=m_vecJDElement[i+1].m_y-m_vecJDElement[i].m_y;

		if(dx >= 0.0 && dy > 0.0)
			m_vecJDElement[i].m_fwj = atan(fabs(dx)/fabs(dy));
		else if(dx >= 0.0 && dy < 0.0) 
			m_vecJDElement[i].m_fwj = PI-atan(fabs(dx)/fabs(dy));
		else if(dx < 0.0 && dy <0.0)
			m_vecJDElement[i].m_fwj = PI+atan(fabs(dx)/fabs(dy));
		else if(dx <0.0 && dy >0.0) 
			m_vecJDElement[i].m_fwj = 2.0*PI-atan(fabs(dx)/fabs(dy));
		else if(dx >0.0 && dy==0.0) 
			m_vecJDElement[i].m_fwj = PI/2.0;
		else if(dx <0.0 && dy==0.0)
			m_vecJDElement[i].m_fwj = PI*3.0/2.0;

		m_vecJDElement[i].m_jdz = sqrt(dx*dx+dy*dy);//计算两点距离
	}
	// 边数比交点数少一个，将其初始化为零
	m_vecJDElement[i].m_fwj = 0.0;
	m_vecJDElement[i].m_jdz = 0.0;
}

void ArxRleCurveElement::Calc_pj_L_A_p_q()
{
	int i;
	m_vecJDElement[0].m_pj = 0.0;
	m_vecJDElement[0].m_p = 0.0;
	m_vecJDElement[0].m_L = 0.0;
	m_vecJDElement[0].m_ly = 0.0;
	m_vecJDElement[0].m_q = 0.0;
	m_vecJDElement[0].m_hqj = 0.0;
	for ( i= 0;i < m_nJDNum-1; i++)
	{
		//计算交点偏角pj
		m_vecJDElement[i+1].m_pj=m_vecJDElement[i+1].m_fwj-m_vecJDElement[i].m_fwj;
		if (m_vecJDElement[i+1].m_pj > PI)//转角范围-180到+180
			m_vecJDElement[i+1].m_pj -= 2*PI;
		else if(m_vecJDElement[i+1].m_pj < -PI)
			m_vecJDElement[i+1].m_pj += 2*PI;

		//计算缓和曲线角b
		m_vecJDElement[i+1].m_hqj = m_vecJDElement[i+1].m_ls / (2.0*m_vecJDElement[i+1].m_R);

		//计算圆曲线长度ly
		m_vecJDElement[i+1].m_ly = fabs(m_vecJDElement[i+1].m_R*(m_vecJDElement[i+1].m_pj-2*m_vecJDElement[i+1].m_hqj));
		//计算曲线长度L
		m_vecJDElement[i+1].m_L = m_vecJDElement[i+1].m_ly+2.0*m_vecJDElement[i+1].m_ls;
		//计算内移距p
		m_vecJDElement[i+1].m_p = pow(m_vecJDElement[i+1].m_ls, 2)/(24*m_vecJDElement[i+1].m_R)-pow(m_vecJDElement[i+1].m_ls, 4)/(2384*pow(m_vecJDElement[i+1].m_R, 3));
		//计算切线增长q
		m_vecJDElement[i+1].m_q = m_vecJDElement[i+1].m_ls/2.0-pow(m_vecJDElement[i+1].m_ls, 3)/(240*pow(m_vecJDElement[i+1].m_R, 2));
	}
	m_vecJDElement[i].m_pj = 0.0;
	m_vecJDElement[i].m_p = 0.0;
	m_vecJDElement[i].m_L = 0.0;
	m_vecJDElement[i].m_ly = 0.0;
	m_vecJDElement[i].m_q = 0.0;
	m_vecJDElement[i].m_hqj = 0.0;

	for (i = 0; i < m_nJDNum; i++)	m_vecJDElement[i].m_A = sqrt(m_vecJDElement[i].m_R * m_vecJDElement[i].m_ls);
}

void ArxRleCurveElement::Calc_T_E()
{
	int i;
	m_vecJDElement[0].m_T = 0.0;
	m_vecJDElement[0].m_E = 0.0;
	for ( i= 0;i < m_nJDNum-1; i++)//计算切线长
	{
		m_vecJDElement[i+1].m_T = (m_vecJDElement[i+1].m_R+m_vecJDElement[i+1].m_p)*tan(fabs(m_vecJDElement[i+1].m_pj)/2)+m_vecJDElement[i+1].m_q;
		m_vecJDElement[i+1].m_E = (m_vecJDElement[i+1].m_R+m_vecJDElement[i+1].m_p)/cos(fabs(m_vecJDElement[i+1].m_pj)/2) - m_vecJDElement[i+1].m_R;
	}
	m_vecJDElement[i].m_T = 0.0;
	m_vecJDElement[i].m_E = 0.0;
}

void ArxRleCurveElement::Calc_ml()//计算主点里程
{
	for(int i = 0; i < m_nJDNum; i++ )
	{
		if(i>0 && i<m_nJDNum-1)
		{
			m_vecJDElement[i].m_ZH = m_vecJDElement[i].m_JD - m_vecJDElement[i].m_T;
			m_vecJDElement[i].m_HY = m_vecJDElement[i].m_ZH + m_vecJDElement[i].m_ls;
			m_vecJDElement[i].m_QZ = m_vecJDElement[i].m_ZH + m_vecJDElement[i].m_L/2;
			m_vecJDElement[i].m_HZ = m_vecJDElement[i].m_ZH + m_vecJDElement[i].m_L;
			m_vecJDElement[i].m_YH = m_vecJDElement[i].m_HZ - m_vecJDElement[i].m_ls;
		}
		else
		{
			m_vecJDElement[i].m_ZH = m_vecJDElement[i].m_JD;
			m_vecJDElement[i].m_HY = m_vecJDElement[i].m_JD;
			m_vecJDElement[i].m_QZ = m_vecJDElement[i].m_JD;
			m_vecJDElement[i].m_YH = m_vecJDElement[i].m_JD;
			m_vecJDElement[i].m_HZ = m_vecJDElement[i].m_JD;
		}
	}
}

void ArxRleCurveElement::OutputJDResult(CString outputfilename)
{	
	setlocale(LC_ALL,"");

	CStdioFile file;
	if(file.Open(outputfilename, CFile::modeCreate | CFile::modeReadWrite))
	{
		file.Seek(CFile::begin,0);
		CString strTemp;
		file.WriteString(L"编号     方位角     距离\n");
		for(int i = 0; i<m_nJDNum;i++)
		{
			strTemp.Format(L"%s  %s  %10f\n",m_vecJDElement[i].m_JDNr,FormatAngle(m_vecJDElement[i].m_fwj*180/PI),m_vecJDElement[i].m_jdz);
			file.WriteString(strTemp);
		}

		file.WriteString(L"********************************************************************\n");
		file.WriteString(L"********************************************************************\n");
		file.WriteString(L"编号      转角      曲线长     内移距     切线长\n");
		for(int i = 0; i<m_nJDNum;i++)
		{
			strTemp.Format(L"%s  %s  %10f  %10f  %10f\n",m_vecJDElement[i].m_JDNr,FormatAngle(m_vecJDElement[i].m_pj*180/PI),m_vecJDElement[i].m_L,m_vecJDElement[i].m_E,m_vecJDElement[i].m_T);
			file.WriteString(strTemp);
		}

		file.WriteString(L"********************************************************************\n");
		file.WriteString(L"********************************************************************\n");
		file.WriteString(L"编号   交点里程   直缓里程   缓圆里程   圆缓里程  缓直里程\n");
		for(int i = 0; i<m_nJDNum;i++)
		{
			strTemp.Format(L"%s  %10f  %10f  %10f  %10f  %10f\n",m_vecJDElement[i].m_JDNr,m_vecJDElement[i].m_JD,m_vecJDElement[i].m_ZH,m_vecJDElement[i].m_HY,m_vecJDElement[i].m_YH,m_vecJDElement[i].m_HZ);
			file.WriteString(strTemp);
		}

		file.WriteString(L"********************************************************************\n");
		file.WriteString(L"********************************************************************\n");
		file.WriteString(L"编号   里程   N(X)   E(Y)\n");
		for(int i = 0; i<m_nJDNum;i++)
		{
			strTemp.Format(L"%s  %10f  %10f  %10f\n",m_vecJDElement[i].m_JDNr,m_vecJDElement[i].m_JD,m_vecJDElement[i].m_x,m_vecJDElement[i].m_y);
			file.WriteString(strTemp);
			strTemp.Format(L"%s  %10f  %10f  %10f\n",L"ZH",m_vecJDElement[i].m_ZH,m_vecJDElement[i].m_ZH_x,m_vecJDElement[i].m_ZH_y);
			file.WriteString(strTemp);
			strTemp.Format(L"%s  %10f  %10f  %10f\n",L"HY",m_vecJDElement[i].m_HY,m_vecJDElement[i].m_HY_x,m_vecJDElement[i].m_HY_y);
			file.WriteString(strTemp);
			strTemp.Format(L"%s  %10f  %10f  %10f\n",L"QZ",m_vecJDElement[i].m_QZ,m_vecJDElement[i].m_QZ_x,m_vecJDElement[i].m_QZ_y);
			file.WriteString(strTemp);
			strTemp.Format(L"%s  %10f  %10f  %10f\n",L"YH",m_vecJDElement[i].m_YH,m_vecJDElement[i].m_YH_x,m_vecJDElement[i].m_YH_y);
			file.WriteString(strTemp);
			strTemp.Format(L"%s  %10f  %10f  %10f\n",L"HZ",m_vecJDElement[i].m_HZ,m_vecJDElement[i].m_HZ_x,m_vecJDElement[i].m_HZ_y);
			file.WriteString(strTemp);
		}

		file.Close();
	}
}

void ArxRleCurveElement::WriteMDBTable(CString mdbFname,CString TabFname)
{
	//// 	AfxOleInit();//VS2002和VC6.0用这个函数
	CoInitialize(NULL);//VS2005用这个函数
	m_pConnection.CreateInstance(__uuidof(XTPREPORTMSADODB::Connection));
	m_pRecordset.CreateInstance(__uuidof(XTPREPORTMSADODB::Recordset));
	////m_strConnect.Format(L"Provider=Microsoft.Jet.OLEDB.4.0;Data Source=%s",mdbFname);
	m_strConnect = L"Provider=Microsoft.Jet.OLEDB.4.0;Data Source=" + mdbFname;

	try         
	{	
		m_pConnection->Open(_bstr_t(m_strConnect),"","",XTPREPORTMSADODB::adModeUnknown);
	}
	catch (_com_error e)
	{
		AfxMessageBox(_T("数据库连接失败!"));
	}


	//// 	if(!FindTable(TabFname))//表不存在
	//// 	{
	CString m_strSQL;
	m_strSQL = "create table " ;
	m_strSQL += TabFname;
	m_strSQL += "(";
	m_strSQL += "ID integer identity,转角 VARCHAR ,曲线长 VARCHAR, 切线长 VARCHAR, 直缓点里程 VARCHAR, 缓圆点里程 VARCHAR, 曲中点里程 VARCHAR, 圆缓点里程 VARCHAR, 缓直点里程 VARCHAR "; //建立新表       
	//,缓圆点x坐标 VARCHAR, 缓圆点y坐标 VARCHAR,圆缓点x坐标 VARCHAR, 圆缓点y坐标 VARCHAR,缓直点x坐标 VARCHAR, 缓直点y坐标 VARCHAR
	m_strSQL += ")";
	Execute(m_strSQL);
	////}
	m_strSQL.Format(L"delete from %s",TabFname);//删除全部
	Execute(m_strSQL);

	m_strSQL =L"select * from "+TabFname;
	try
	{
		m_pRecordset->Open(_bstr_t(m_strSQL),
			m_pConnection.GetInterfacePtr(),
			XTPREPORTMSADODB::adOpenStatic,
			XTPREPORTMSADODB::adLockOptimistic,
			XTPREPORTMSADODB::adCmdText);
	}
	catch (_com_error e)
	{
		AfxMessageBox(e.ErrorMessage());                     
	}
	try
	{
		CString strText;
		_variant_t str;
		for(int i=0;i<m_nJDNum-2;i++)
		{
			m_pRecordset->AddNew();
			strText.Format(L"%d",i+1);
			str = strText;
			m_pRecordset->PutCollect(L"ID",str);

			strText.Format(L"%.3lf",m_vecJDElement[i+1].m_pj);
			str = strText;
			m_pRecordset->PutCollect(L"转角",str);

			strText.Format(L"%.3lf",m_vecJDElement[i+1].m_L);
			str = strText;
			m_pRecordset->PutCollect(L"曲线长",str);

			strText.Format(L"%.3lf",m_vecJDElement[i+1].m_T);
			str = strText;
			m_pRecordset->PutCollect(L"切线长",str);

			strText.Format(L"%.3lf",m_vecJDElement[i+1].m_JD);
			str = strText;
			m_pRecordset->PutCollect(L"交点里程",str);

			strText.Format(L"%.3lf",m_vecJDElement[i+1].m_ZH);
			str = strText;
			m_pRecordset->PutCollect(L"直缓点里程",str);

			strText.Format(L"%.3lf",m_vecJDElement[i+1].m_HY);
			str = strText;
			m_pRecordset->PutCollect(L"缓圆点里程",str);

			strText.Format(L"%.3lf",m_vecJDElement[i+1].m_QZ);
			str = strText;
			m_pRecordset->PutCollect(L"曲中点里程",str);

			strText.Format(L"%.3lf",m_vecJDElement[i+1].m_YH);
			str = strText;
			m_pRecordset->PutCollect(L"圆缓点里程",str);

			strText.Format(L"%.3lf",m_vecJDElement[i+1].m_HZ);
			str = strText;
			m_pRecordset->PutCollect(L"缓直点里程",str);

			/*strText.Format(L"%.3lf",m_vecJDElement[i+1].m_HY_x);
			str = strText;
			m_pRecordset->PutCollect(L"缓圆点x坐标",str);

			strText.Format(L"%.3lf",m_vecJDElement[i+1].m_HY_y);
			str = strText;
			m_pRecordset->PutCollect(L"缓圆点y坐标",str);

			strText.Format(L"%.3lf",m_vecJDElement[i+1].m_YH_x);
			str = strText;
			m_pRecordset->PutCollect(L"圆缓点x坐标",str);

			strText.Format(L"%.3lf",m_vecJDElement[i+1].m_YH_y);
			str = strText;
			m_pRecordset->PutCollect(L"圆缓点y坐标",str);

			strText.Format(L"%.3lf",m_vecJDElement[i+1].m_HZ_x);
			str = strText;
			m_pRecordset->PutCollect(L"缓直点x坐标",str);

			strText.Format(L"%.3lf",m_vecJDElement[i+1].m_HZ_y);
			str = strText;
			m_pRecordset->PutCollect(L"缓直点y坐标",str);*/

		}
		m_pRecordset->Update();
	}
	catch(_com_error e)
	{
		AfxMessageBox(e.ErrorMessage());
	}
	m_pRecordset->Close();
	m_pConnection->Close();
}

BOOL ArxRleCurveElement::Execute(CString m_strSQL)
{
	_variant_t RecordsAffected;
	try
	{
		if (m_pConnection != NULL && m_pConnection->State == XTPREPORTMSADODB::adStateOpen)
		{
			m_pConnection->Execute(_bstr_t(m_strSQL),&RecordsAffected,XTPREPORTMSADODB::adCmdText);
		}
	}
	catch(_com_error e)
	{
		// 		AfxMessageBox(e.Description());
	} 
	return FALSE;
}

void ArxRleCurveElement::ml_to_xy(double ml, double& x, double& y, double& fwj, double& qxj)
{
	int ml_type=0;//任一里程点类型
	int i=0;
	double a=0.0,b=0.0;
	double _x=0.0,_y=0.0,dx=0.0,dy=0.0;
	double l=0.0;

	for (i = 0;i < m_nJDNum - 1;i++ )
	{
		if (ml >= m_vecJDElement[i].m_HZ && ml <= m_vecJDElement[i + 1].m_ZH)
		{
			ml_type = 1; //1 直线
			break;
		}
		else if (ml > m_vecJDElement[i].m_ZH && ml < m_vecJDElement[i].m_HY)
		{
			ml_type = 2;  //2 前缓和曲线
			break;
		}
		else if (ml > m_vecJDElement[i].m_YH && ml < m_vecJDElement[i].m_HZ)
		{
			ml_type = 3;  //3 后缓和曲线
			break;
		}
		else if (ml >= m_vecJDElement[i].m_HY && ml <= m_vecJDElement[i].m_YH)
		{
			ml_type = 4;  //4 圆曲线
			break;
		}
	}

	switch (ml_type)
	{
	case 1://直线
		{
			l = ml - m_vecJDElement[i].m_HZ + m_vecJDElement[i].m_T;
			x = m_vecJDElement[i].m_x + l * cos(PI/2 - m_vecJDElement[i].m_fwj);
			y = m_vecJDElement[i].m_y + l * sin(PI/2 - m_vecJDElement[i].m_fwj);
			fwj = m_vecJDElement[i].m_fwj;
			qxj = PI/2-fwj;
			break;
		}
	case 2://前缓和曲线
		{
			l = ml-m_vecJDElement[i].m_ZH;
			_x = l - pow(l,5) / (40.0*pow(m_vecJDElement[i].m_R*m_vecJDElement[i].m_ls,2)) + pow(l,9) / (3456*pow(m_vecJDElement[i].m_R*m_vecJDElement[i].m_ls,4));
			_y = pow(l,3) / (6.0*m_vecJDElement[i].m_R*m_vecJDElement[i].m_ls) - pow(l,7) / (336*pow(m_vecJDElement[i].m_R * m_vecJDElement[i].m_ls,3)) + pow(l,11) / (42240*pow(m_vecJDElement[i].m_R * m_vecJDElement[i].m_ls,5));
			a = PI/2 - m_vecJDElement[i-1].m_fwj;

			if (m_vecJDElement[i].m_pj < 0)
			{
				x = cos(a)*_x + sin(a)*_y + m_vecJDElement[i].m_ZH_x;
				y = sin(a)*_x - cos(a)*_y + m_vecJDElement[i].m_ZH_y;
				fwj = m_vecJDElement[i-1].m_fwj/*m_pj*/ - l*l/2 / m_vecJDElement[i].m_R / m_vecJDElement[i].m_ls;
				qxj = PI/2 - fwj;
			}
			if (m_vecJDElement[i].m_pj > 0)
			{
				x = cos(a)*_x + sin(a)*_y + m_vecJDElement[i].m_ZH_x;
				y = sin(a)*_x - cos(a)*_y + m_vecJDElement[i].m_ZH_y;
				fwj = m_vecJDElement[i - 1].m_fwj/*m_pj*/ + l*l/2 / m_vecJDElement[i].m_R / m_vecJDElement[i].m_ls;
				qxj = PI/2 - fwj;
			}
			break;
		}
	case 3://后缓和曲线
		{
			l = m_vecJDElement[i].m_HZ - ml;
			_x = l - pow(l,5) / (40.0*pow(m_vecJDElement[i].m_R*m_vecJDElement[i].m_ls,2)) + pow(l,9) / (3456*pow(m_vecJDElement[i].m_R*m_vecJDElement[i].m_ls,4));
			_y = pow(l,3) / (6.0*m_vecJDElement[i].m_R*m_vecJDElement[i].m_ls) - pow(l,7) / (336*pow(m_vecJDElement[i].m_R * m_vecJDElement[i].m_ls,3)) + pow(l,11) / (42240*pow(m_vecJDElement[i].m_R * m_vecJDElement[i].m_ls,5));;
			a = 3* PI/2 - m_vecJDElement[i].m_fwj;

			if (m_vecJDElement[i].m_pj < 0)
			{
				x = cos(a)*_x - sin(a)*_y + m_vecJDElement[i].m_HZ_x;
				y = sin(a)*_x + cos(a)*_y + m_vecJDElement[i].m_HZ_y;
				fwj = m_vecJDElement[i].m_fwj/*m_pj*/ + l * l / 2.0 / m_vecJDElement[i].m_R / m_vecJDElement[i].m_ls;
				qxj = PI/2 - fwj;
			}
			if (m_vecJDElement[i].m_pj > 0)
			{
				x = cos(a)*_x - sin(a)*_y + m_vecJDElement[i].m_HZ_x;
				y = sin(a)*_x + cos(a)*_y + m_vecJDElement[i].m_HZ_y;
				fwj = m_vecJDElement[i].m_fwj/*m_pj*/ - l * l / 2.0 / m_vecJDElement[i].m_R / m_vecJDElement[i].m_ls;
				qxj = PI/2 - fwj;
			}
			break;
		}
	case 4://圆曲线
		{
			dx = m_vecJDElement[i].m_YH_x - m_vecJDElement[i].m_HY_x;
			dy = m_vecJDElement[i].m_YH_y - m_vecJDElement[i].m_HY_y;
			a = (PI - fabs(m_vecJDElement[i].m_pj) + m_vecJDElement[i].m_ls / m_vecJDElement[i].m_R) / 2.0;

			//求所在圆圆心的大地坐标
			if (m_vecJDElement[i].m_pj > 0)
			{
				if (dx > 0)
					a = a - atan(dy/dx);
				if (dx < 0)
					a = a - atan(dy/dx) + PI;

				_x = m_vecJDElement[i].m_HY_x + m_vecJDElement[i].m_R * cos(a);
				_y = m_vecJDElement[i].m_HY_y - m_vecJDElement[i].m_R * sin(a);
			}
			if (m_vecJDElement[i].m_pj < 0)
			{
				if (dx > 0)
					a = a + atan(dy/dx);
				if (dx < 0)
					a = a + atan(dy/dx) + PI;

				_x = m_vecJDElement[i].m_HY_x + m_vecJDElement[i].m_R * cos(a);
				_y = m_vecJDElement[i].m_HY_y + m_vecJDElement[i].m_R * sin(a);
			}

			//根据圆心坐标计算所求里程点大地坐标
			dx = m_vecJDElement[i].m_HY_x - _x;
			dy = m_vecJDElement[i].m_HY_y - _y;
			l = ml - m_vecJDElement[i].m_HY;
			a = l / m_vecJDElement[i].m_R;
			b = atan(dy/dx);
			if (m_vecJDElement[i].m_pj > 0.0)
			{
				if (dx > 0) b = b - a;
				if (dx < 0) b = b - a + PI;
			}

			if (m_vecJDElement[i].m_pj < 0.0)
			{
				if (dx > 0) b = b + a;
				if (dx < 0) b = b + a - PI;
			}
			x = _x + m_vecJDElement[i].m_R * cos(b);
			y = _y + m_vecJDElement[i].m_R * sin(b);
		}

		//计算方位角
		dx = x - _x;
		dy = y - _y;
		if(dx >= 0 && dy > 0) fwj = atan(fabs(dy / dx));
		else if(dx >= 0 && dy < 0) fwj = PI-atan(fabs(dy / dx));
		else if(dx < 0 && dy < 0) fwj = PI + atan(fabs(dy / dx));
		else if(dx < 0 && dy > 0) fwj = 2*PI - atan(fabs(dy / dx));
		else if(dx > 0 && dy == 0) fwj = PI/2;
		else if(dx < 0 && dy == 0) fwj = PI/3;
		qxj = PI/2+fwj;
		break;
	}
}

void ArxRleCurveElement::Calc_ZH_HZ_xy()
{
	double l1,l2;
	int i;
	for (i=0;i<m_nJDNum;i++ )
	{
		l1=m_vecJDElement[i].m_T;
		m_vecJDElement[i].m_HZ_x = m_vecJDElement[i].m_x + l1*cos(PI/2.0-m_vecJDElement[i].m_fwj);
		m_vecJDElement[i].m_HZ_y = m_vecJDElement[i].m_y + l1*sin(PI/2.0-m_vecJDElement[i].m_fwj);	
	}

	m_vecJDElement[0].m_ZH_x = m_vecJDElement[0].m_x;
	m_vecJDElement[0].m_ZH_y = m_vecJDElement[0].m_y;
	for (i=0;i<m_nJDNum-1;i++ )
	{
		l2 = m_vecJDElement[i].m_jdz-m_vecJDElement[i + 1].m_T;
		m_vecJDElement[i + 1].m_ZH_x = m_vecJDElement[i].m_x + l2*cos(PI/2.0-m_vecJDElement[i].m_fwj);
		m_vecJDElement[i + 1].m_ZH_y = m_vecJDElement[i].m_y + l2*sin(PI/2.0-m_vecJDElement[i].m_fwj);
	}
}

void ArxRleCurveElement::Calc_HY_YH_xy()
{
	double x,y,a,b;
	int i;

	//起始点初始化
	m_vecJDElement[0].m_HY_x = m_vecJDElement[0].m_x;
	m_vecJDElement[0].m_HY_y = m_vecJDElement[0].m_y;
	m_vecJDElement[0].m_YH_x = m_vecJDElement[0].m_x;
	m_vecJDElement[0].m_YH_y = m_vecJDElement[0].m_y;

	for (i = 1;i < m_nJDNum - 1;i++ )
	{
		x = m_vecJDElement[i].m_ls-pow(m_vecJDElement[i].m_ls,3) / (40*pow(m_vecJDElement[i].m_R,2));
		y = pow(m_vecJDElement[i].m_ls,2) / (6*m_vecJDElement[i].m_R) - pow(m_vecJDElement[i].m_ls,4) / (336*pow(m_vecJDElement[i].m_R,3)) + pow(m_vecJDElement[i].m_ls,11) / (42240*pow(m_vecJDElement[i].m_R,5));

		a = PI/2 - m_vecJDElement[i-1].m_fwj;
		b = 3.0 * PI/2 -m_vecJDElement[i].m_fwj;

		if (m_vecJDElement[i].m_pj < 0)
		{
			m_vecJDElement[i].m_HY_x = cos(a)*x - sin(a) * y + m_vecJDElement[i].m_ZH_x;
			m_vecJDElement[i].m_HY_y = sin(a)*x + cos(a) * y + m_vecJDElement[i].m_ZH_y;
			m_vecJDElement[i].m_YH_x = cos(b)*x + sin(b) * y + m_vecJDElement[i].m_HZ_x;
			m_vecJDElement[i].m_YH_y = sin(b)*x - cos(b) * y + m_vecJDElement[i].m_HZ_y;

		}
		if (m_vecJDElement[i].m_pj>0)
		{
			m_vecJDElement[i].m_HY_x = cos(a) *x + sin(a) * y + m_vecJDElement[i].m_ZH_x;
			m_vecJDElement[i].m_HY_y = sin(a) *x - cos(a) * y + m_vecJDElement[i].m_ZH_y;
			m_vecJDElement[i].m_YH_x = cos(b) *x - sin(b) * y + m_vecJDElement[i].m_HZ_x;
			m_vecJDElement[i].m_YH_y = sin(b) *x + cos(b) * y + m_vecJDElement[i].m_HZ_y;
		}
	}
	m_vecJDElement[i].m_HY_x = m_vecJDElement[i].m_x;
	m_vecJDElement[i].m_HY_y = m_vecJDElement[i].m_y;
	m_vecJDElement[i].m_YH_x = m_vecJDElement[i].m_x;
	m_vecJDElement[i].m_YH_y = m_vecJDElement[i].m_y;
}

void ArxRleCurveElement::Calc_QZ_xy()
{
	double dx,dy,_x,_y,a,b,l;
	int i = 0;

	m_vecJDElement[i].m_QZ_x = m_vecJDElement[i].m_x;
	m_vecJDElement[i].m_QZ_y = m_vecJDElement[i].m_y;
	for (i=1;i< m_nJDNum-1;i++ )
	{
		dx = m_vecJDElement[i].m_YH_x - m_vecJDElement[i].m_HY_x;
		dy = m_vecJDElement[i].m_YH_y - m_vecJDElement[i].m_HY_y;
		a = (PI - fabs(m_vecJDElement[i].m_pj) + m_vecJDElement[i].m_ls / m_vecJDElement[i].m_R) / 2.0;

		//求所在圆圆心的大地坐标
		if (m_vecJDElement[i].m_pj > 0)
		{
			if (dx > 0)
				a = a - atan(dy/dx);
			if (dx < 0)
				a = a - atan(dy/dx) + PI;

			_x = m_vecJDElement[i].m_HY_x + m_vecJDElement[i].m_R * cos(a);
			_y = m_vecJDElement[i].m_HY_y - m_vecJDElement[i].m_R * sin(a);
		}
		if (m_vecJDElement[i].m_pj < 0)
		{
			if (dx > 0)
				a = a + atan(dy/dx);
			if (dx < 0)
				a = a + atan(dy/dx) + PI;

			_x = m_vecJDElement[i].m_HY_x + m_vecJDElement[i].m_R * cos(a);
			_y = m_vecJDElement[i].m_HY_y + m_vecJDElement[i].m_R * sin(a);
		}

		//根据圆心坐标计算所求里程点大地坐标
		dx = m_vecJDElement[i].m_HY_x - _x;
		dy = m_vecJDElement[i].m_HY_y - _y;
		l = m_vecJDElement[i].m_QZ - m_vecJDElement[i].m_HY;
		a = l / m_vecJDElement[i].m_R;
		b = atan(dy/dx);
		if (m_vecJDElement[i].m_pj > 0.0)
		{
			if (dx > 0) b = b - a;
			if (dx < 0) b = b - a + PI;
		}

		if (m_vecJDElement[i].m_pj < 0.0)
		{
			if (dx > 0) b = b + a;
			if (dx < 0) b = b + a - PI;
		}
		m_vecJDElement[i].m_QZ_x = _x + m_vecJDElement[i].m_R * cos(b);
		m_vecJDElement[i].m_QZ_y = _y + m_vecJDElement[i].m_R * sin(b);
	}
	m_vecJDElement[i].m_QZ_x = m_vecJDElement[i].m_x;
	m_vecJDElement[i].m_QZ_y = m_vecJDElement[i].m_y;
}

double ArxRleCurveElement::SqxCalc(double stat, SQXELEMENT sqx)
{
	double h = -9999.9999;
	double S = sqx.m_S, H = sqx.m_H, R = sqx.m_R, I1 = sqx.m_I1, I2 = sqx.m_I2, T = sqx.m_T, E = sqx.m_E, L = sqx.m_L;

	if(stat < (S - T) || stat > (S + T))
	{
		if (stat < S)
			h = H - (S - stat) * I1;
		else
			h = H + (stat - S) * I2;
	}
	else
	{
		if(stat < S)
			h = H - (S - stat) * I1 + (stat - S + T) * (stat - S + T) / 2 / R * ((I1 > I2) ? -1.0 : 1.0);
		else
			h = H + (stat - S) * I2 + (S + T - stat) * (S + T - stat) / 2 / R * ((I1 > I2) ? -1.0 : 1.0);
	}

	return h;
}

BOOL ArxRleCurveElement::IsSqx(double stat, SQXELEMENT sqx)
{
	double S = sqx.m_S, T = sqx.m_T;

	if(stat < (S - T) || stat > (S + T))
		return FALSE;

	return TRUE;
}

SQXELEMENT ArxRleCurveElement::GetSqx(double stat)
{
	if(m_nSQXNum == 0) return SQXELEMENT();
	if (stat <= m_vecSQXElement[0].m_S) return m_vecSQXElement[0];
	if (stat >= m_vecSQXElement[m_nSQXNum - 1].m_S) return m_vecSQXElement[m_nSQXNum - 1];
	for (int i = 1; i < m_nSQXNum; i++ )
	{
		SQXELEMENT sqx1 = m_vecSQXElement[i-1], sqx2 = m_vecSQXElement[i];
		if (stat >= sqx1.m_S - sqx1.m_T && stat <= sqx1.m_S + sqx1.m_T)
			return sqx1;
		if (stat > sqx1.m_S + sqx1.m_T && stat < sqx2.m_S - sqx2.m_T)
			return sqx1;
	}

	return SQXELEMENT();
}
