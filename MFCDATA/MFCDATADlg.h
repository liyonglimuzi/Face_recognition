
// MFCDATADlg.h : 头文件
//

#pragma once
#include "afxcmn.h"
#include"Cdata.h"
#include "afxwin.h"
#include "afxeditbrowsectrl.h"
#include"resource.h"
#include"VideoInfo.h"
#include"MFCDATADlg.h"

// CMFCDATADlg 对话框
class CMFCDATADlg : public CDialogEx
{
// 构造
public:
	CMFCDATADlg(CWnd* pParent = NULL);	// 标准构造函数

// 对话框数据
	enum { IDD = IDD_MFCDATA_DIALOG };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV 支持


// 实现
protected:
	HICON m_hIcon;

	// 生成的消息映射函数
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP();

public:
	
	
	_RecordsetPtr m_pRecordset;
	afx_msg void OnEnChangeBrowseFile();
	afx_msg void OnBnClickedButtonOpen();
	afx_msg void OnBnClickedButton2();
	afx_msg  void  CMFCDATADlg::Data(int number);
	// 图片窗口控件变量
	CStatic m_picture;
	CListCtrl m_list;
	// 浏览文件
	CMFCEditBrowseCtrl m_browse_file;
	char*  CMFCDATADlg::GetEditString();
	afx_msg void OnTimer(UINT_PTR nIDEvent);
	afx_msg void OnBnClickedButton1();
	// 导出数据
	CButton bu_export;
};
