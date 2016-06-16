#include "stdafx.h"
#include "ShowData.h"


#include "ShowData.h"
#include"Cdata.h"
#include"MFCDATA.h"
#include"MFCDATADlg.h"

ShowData::ShowData()
{
}
int ShowData::getData(int num)
{
	int	number = num;
	return number;
}
void ShowData::data(int order)
{
	_RecordsetPtr m_pRecordset;
     CMFCDATADlg faceDlg;
	/*Cdata ado;
	ado.OnInitADOConn();
	CString sql;
	_variant_t	number;
	INT num;
	sql = "select *from attendant_sheet where 序号=3";
	sql.Format(_T("select *from attendant_sheet where NUM=3"));
	sql = sql + _T(""@order"");
	AfxMessageBox(sql);
	m_pRecordset = ado.GetRecordSet((_bstr_t)sql);
	
	while ((m_pRecordset->adoEOF == 0) && (m_pRecordset != NULL))
	{
		number = faceDlg.m_pRecordset->GetCollect("NUM");
		
		num = number;

		m_pRecordset->MoveNext();
	}
	
	int linecout = faceDlg.m_list.GetItemCount();

	for (int i = 0; (i < linecout) && (number != faceDlg.m_list.GetItemText(1, i)); i++)
	{  
		faceDlg.m_list.SetItemText(0, 4, (LPCTSTR)(_bstr_t)m_pRecordset->GetCollect("CLASS"));
		faceDlg.m_list.InsertItem(0, _T("zhesjiegg"));
		faceDlg.m_list.SetItemText(0, 1, _T("zhesjiegg"));
		faceDlg.m_list.SetItemText(0, 2, (LPCTSTR)(_bstr_t)m_pRecordset->GetCollect("NAME"));
		faceDlg.m_list.SetItemText(0, 3, (LPCTSTR)(_bstr_t)m_pRecordset->GetCollect("PROJECT"));
		
		m_pRecordset->MoveNext();
	}
	ado.ExitConnect();*/
	Cdata m_AdoConn;
	// // m_pConnection.CreateInstance(“m_AdoConn.Connection”)
	//	
	m_AdoConn.OnInitADOConn();
	CString sql;
	sql.Format(_T("select * from attendant_sheet where NUM=3"));
	//	
	m_pRecordset = m_AdoConn.GetRecordSet((_bstr_t)sql);
	while (m_AdoConn.m_pRecordset->adoEOF == 0)
	{
		faceDlg.m_list.InsertItem(0, _T(""));
		faceDlg.m_list.SetItemText(0, 0, (LPCTSTR)(_bstr_t)m_pRecordset->GetCollect("NUMBER"));
		faceDlg.m_list.SetItemText(0, 1, (LPCTSTR)(_bstr_t)m_pRecordset->GetCollect("NAME"));
		faceDlg.m_list.SetItemText(0, 2, (LPCTSTR)(_bstr_t)m_pRecordset->GetCollect("PROJECT"));
		faceDlg.m_list.SetItemText(0, 3, (LPCTSTR)(_bstr_t)m_pRecordset->GetCollect("CLASS"));
		m_pRecordset->MoveNext();
	}
	m_AdoConn.ExitConnect();
}
int ShowData::Folder(char* dir)   
{
	int nSubDirCount = 0;//子文件夹的数目
	CFileFind finder;
	CString dir_tostring(dir);
	dir_tostring += _T("\\*.*");
	BOOL bWorking = finder.FindFile(dir_tostring);
	while (bWorking)
	{
		bWorking = finder.FindNextFile();
		if (finder.IsDots())
		{
			continue;
		}
		if (finder.IsDirectory())
		{
			nSubDirCount++;
		}

	}
	finder.Close();
	return nSubDirCount;

}


ShowData::~ShowData()
{

}

