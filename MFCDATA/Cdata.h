#pragma once


#if !defined(AFX_ADOCONN_H__AC448F02_AF26_45E4_9B2D_D7ECB8FFCFB9__INCLUDED_)
#define AFX_ADOCONN_H__AC448F02_AF26_45E4_9B2D_D7ECB8FFCFB9__INCLUDED_
#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// Cdata ����Ŀ��

class Cdata : public CDatabase
{
public:
	//���һ��ָ��Connection�����ָ��:
	_ConnectionPtr m_pConnection;
	//���һ��ָ��Recordset�����ָ��:
	_RecordsetPtr m_pRecordset;

public:
	Cdata();
	virtual ~Cdata();
	// ��ʼ�����������ݿ�
	void  OnInitADOConn(); // ִ�в�ѯ
	afx_msg void OnFind();
	_RecordsetPtr& GetRecordSet(_bstr_t bstrSQL);
	// ִ��SQL��䣬Insert Update _variant_t
	BOOL ExecuteSQL(_bstr_t bstrSQL);
	void ExitConnect();
};

#endif // !defined(AFX_ADOCONN_H__AC448F02_AF26_45E4_9B2D_D7ECB8FFCFB9__INCLUDED_)



