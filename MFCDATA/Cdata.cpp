// Cdata.cpp : 实现文件
//

#include "stdafx.h"
#include "MFCDATA.h"
#include "Cdata.h"
#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#define new DEBUG_NEW
#endif

// Cdata

Cdata::Cdata()
{
}

Cdata::~Cdata()
{
}


// Cdata 成员函数

// 初始化—连接数据库
void  Cdata::OnInitADOConn()
{
	// 初始化OLE/COM库环境 
	::CoInitialize(NULL);

	try
	{
		// 创建Connection对象
		m_pConnection.CreateInstance("ADODB.Connection");
		m_pConnection->Open("DSN=serversock;Server= localhost;Database=student", "root", "5201314", adModeUnknown);
	}
	// 捕捉异常
	catch (_com_error e)
	{
		// 显示错误信息
		AfxMessageBox(e.Description());
	}
}

// 执行查询
_RecordsetPtr&  Cdata::GetRecordSet(_bstr_t bstrSQL)
{
	try
	{
		// 连接数据库，如果Connection对象为空，则重新连接数据库
		if (m_pConnection == NULL)
			OnInitADOConn();
		// 创建记录集对象
		m_pRecordset.CreateInstance(__uuidof(Recordset));
		// 取得表中的记录
		m_pRecordset->Open(bstrSQL, m_pConnection.GetInterfacePtr(), adOpenDynamic, adLockOptimistic, adCmdText);
	}
	// 捕捉异常
	catch (_com_error e)
	{
		// 显示错误信息
		AfxMessageBox(e.Description());
	}
	// 返回记录集
	return m_pRecordset;
}

// 执行SQL语句，Insert Update _variant_t
BOOL Cdata::ExecuteSQL(_bstr_t bstrSQL)
{
	//    _variant_t RecordsAffected;
	try
	{
		// 是否已经连接数据库
		if (m_pConnection == NULL)
			OnInitADOConn();
		// Connection对象的Execute方法:(_bstr_t CommandText, 
		// VARIANT * RecordsAffected, long Options ) 
		// 其中CommandText是命令字串，通常是SQL命令。
		// 参数RecordsAffected是操作完成后所影响的行数, 
		// 参数Options表示CommandText的类型：adCmdText-文本命令；adCmdTable-表名
		// adCmdProc-存储过程；adCmdUnknown-未知
		m_pConnection->Execute(bstrSQL, NULL, adCmdText);
		return true;
	}
	catch (_com_error e)
	{
		AfxMessageBox(e.Description());
		return false;
	}
}

void Cdata::ExitConnect()
{
	// 关闭记录集和连			接
	if (m_pRecordset != NULL)
		m_pRecordset->Close();
	m_pConnection->Close();
	// 释放环境
	::CoUninitialize();
}