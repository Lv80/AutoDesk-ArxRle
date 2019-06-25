// ArxRleCurveElement.h: interface for the ArxRleCurveElement class.
//

#ifndef ARXRLE_CRUVE_ELEMENT_H
#define ARXRLE_CRUVE_ELEMENT_H

typedef struct tagJDELEMENT
{
	CString m_JDNr;								    // ������
	double m_x, m_y;						        // ��������
	double m_R, m_ls, m_A;				            // �뾶���������߳����������߲���

	double m_fwj, m_pj, m_hqj;	                    // ��λ�ǡ�ƫ�ǡ��������߽�
	double m_jdz, m_p, m_q;                         // ����ࡢ���ƾ�,��������
	double m_T, m_L, m_E, m_ly;					    // ���߳������߳�����ʸ�ࡢԲ���߳�

	double m_JD;                                  // �������
	double m_ZH, m_HY, m_QZ, m_YH, m_HZ;  // �����������

	double m_ZH_x,m_ZH_y,m_HZ_x,m_HZ_y;				// ֱ����ֱ������
	double m_HY_x,m_HY_y,m_YH_x,m_YH_y;				// ��ԲԲ��������
	double m_QZ_x,m_QZ_y;                           // ��������

	tagJDELEMENT()
	{
		m_JDNr = _T("JDXX");
		m_x = m_y = 0;
		m_R = m_ls = m_A = 0;

		m_fwj = m_pj = m_hqj = 0;
		m_jdz = m_p = m_q = 0;
		m_T = m_L = m_E = m_ly = 0;

		m_JD = 0;
		m_ZH = m_HY = m_QZ = m_YH = m_HZ = 0;

		m_ZH_x = m_ZH_y = m_HZ_x = m_HZ_y = 0;
		m_HY_x = m_HY_y = m_YH_x = m_YH_y = 0;
		m_QZ_x = m_QZ_y = 0;
	}

	tagJDELEMENT(const tagJDELEMENT& src)
	{
		m_JDNr = src.m_JDNr;
		m_x = src.m_x, m_y = src.m_y;
		m_R = src.m_R, m_ls = src.m_ls , m_A = src.m_A;

		m_fwj = src.m_fwj, m_pj = src.m_pj, m_hqj = src.m_hqj;
		m_jdz = src.m_jdz, m_p = src.m_p, m_q = src.m_q;
		m_T = src.m_T, m_L = src.m_L, m_E = src.m_E, m_ly = src.m_ly;

		m_JD = src.m_JD;
		m_ZH = src.m_ZH, m_HY = src.m_HY, m_QZ = src.m_QZ, m_YH = src.m_YH, m_HZ = src.m_HZ;
		
		m_ZH_x = src.m_ZH_x, m_ZH_y = src.m_ZH_y, m_HZ_x = src.m_HZ_x, m_HZ_y = src.m_HZ_y;
		m_HY_x = src.m_HY_x, m_HY_y = src.m_HY_y, m_YH_x = src.m_YH_x, m_YH_y = src.m_YH_y;
		m_QZ_x = src.m_QZ_x, m_QZ_y = src.m_QZ_y;
	}

	tagJDELEMENT operator =(const tagJDELEMENT& src)
	{
		m_JDNr = src.m_JDNr;
		m_x = src.m_x, m_y = src.m_y;
		m_R = src.m_R, m_ls = src.m_ls , m_A = src.m_A;

		m_fwj = src.m_fwj, m_pj = src.m_pj, m_hqj = src.m_hqj;
		m_jdz = src.m_jdz, m_p = src.m_p, m_q = src.m_q;
		m_T = src.m_T, m_L = src.m_L, m_E = src.m_E, m_ly = src.m_ly;

		m_JD = src.m_JD;
		m_ZH = src.m_ZH, m_HY = src.m_HY, m_QZ = src.m_QZ, m_YH = src.m_YH, m_HZ = src.m_HZ;
		
		m_ZH_x = src.m_ZH_x, m_ZH_y = src.m_ZH_y, m_HZ_x = src.m_HZ_x, m_HZ_y = src.m_HZ_y;
		m_HY_x = src.m_HY_x, m_HY_y = src.m_HY_y, m_YH_x = src.m_YH_x, m_YH_y = src.m_YH_y;
		m_QZ_x = src.m_QZ_x, m_QZ_y = src.m_QZ_y;

		return *this;
	}
}JDELEMENT;

typedef struct tagSQXELEMENT
{
	double m_S, m_H, m_R, m_I1, m_I2;
	double m_T, m_E, m_L;

	tagSQXELEMENT()
	{
		m_S=m_H=m_R=m_I1=m_I2=m_T=m_E=m_L = 0;
	}

	tagSQXELEMENT(const tagSQXELEMENT& src)
	{
		m_S = src.m_S;
		m_H = src.m_H;
		m_R = src.m_R;
		m_I1 = src.m_I1;
		m_I2 = src.m_I2;
		m_T = src.m_T;
		m_E = src.m_E;
		m_L = src.m_L;
	}

	tagSQXELEMENT operator =(const tagSQXELEMENT& src)
	{
		m_S = src.m_S;
		m_H = src.m_H;
		m_R = src.m_R;
		m_I1 = src.m_I1;
		m_I2 = src.m_I2;
		m_T = src.m_T;
		m_E = src.m_E;
		m_L = src.m_L;

		return *this;
	}

}SQXELEMENT;

class ArxRleCurveElement
{
public:
	ArxRleCurveElement();
	~ArxRleCurveElement();

	void InputJDData(CString inputfilename);//�ļ����뺯��
	void InputSQXData(CString inputfilename);//�ļ����뺯��
	void ReadMDBTable(CString mdbFname,CString TabFname);
	void Calc_fwj_jdz();//���㷽λ�ǡ�����֮����뺯��
	void Calc_pj_L_A_p_q();//����ת�ǡ����߳��ȡ��������߲��������ƾ�
	void Calc_T_E();//�������߳������ƾ�
	void Calc_ml();//����������̺���
	void OutputJDResult(CString outputfilename);//�ļ��������
	void WriteMDBTable(CString mdbFname,CString TabFname);
	BOOL Execute(CString strSQL);
	void ml_to_xy(double ml, double& x, double& y, double& fwj, double& qxj);//���ת���꺯��

	void Calc_ZH_HZ_xy();
	void Calc_HY_YH_xy();
	void Calc_QZ_xy();

	SQXELEMENT GetSqx(double stat);
	void GetSqxs(CString filePath);
	double SqxCalc(double stat, SQXELEMENT sqx);
	BOOL IsSqx(double stat, SQXELEMENT sqx);

	int m_nJDNum;
	int m_nSQXNum;
	vector<JDELEMENT> m_vecJDElement;
	vector<SQXELEMENT> m_vecSQXElement;

	CString m_strConnect;
	CString	m_strSQL;
	XTPREPORTMSADODB::_ConnectionPtr m_pConnection;
	XTPREPORTMSADODB::_RecordsetPtr m_pRecordset;
};

#endif // ARXRLE_CRUVE_ELEMENT_H
