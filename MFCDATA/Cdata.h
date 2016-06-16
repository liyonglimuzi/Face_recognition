#pragma once


#if !defined(AFX_ADOCONN_H__AC448F02_AF26_45E4_9B2D_D7ECB8FFCFB9__INCLUDED_)
#define AFX_ADOCONN_H__AC448F02_AF26_45E4_9B2D_D7ECB8FFCFB9__INCLUDED_
#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// Cdata 命令目标

class Cdata : public CDatabase
{
public:
	//添加一个指向Connection对象的指针:
	_ConnectionPtr m_pConnection;
	//添加一个指向Recordset对象的指针:
	_RecordsetPtr m_pRecordset;

public:
	Cdata();
	virtual ~Cdata();
	// 初始化—连接数据库
	void  OnInitADOConn(); // 执行查询
	afx_msg void OnFind();
	_RecordsetPtr& GetRecordSet(_bstr_t bstrSQL);
	// 执行SQL语句，Insert Update _variant_t
	BOOL ExecuteSQL(_bstr_t bstrSQL);
	void ExitConnect();
};

#endif // !defined(AFX_ADOCONN_H__AC448F02_AF26_45E4_9B2D_D7ECB8FFCFB9__INCLUDED_)



