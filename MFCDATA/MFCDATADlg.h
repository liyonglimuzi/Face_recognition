
// MFCDATADlg.h : ͷ�ļ�
//

#pragma once
#include "afxcmn.h"
#include"Cdata.h"
#include "afxwin.h"
#include "afxeditbrowsectrl.h"
#include"resource.h"
#include"VideoInfo.h"
#include"MFCDATADlg.h"

// CMFCDATADlg �Ի���
class CMFCDATADlg : public CDialogEx
{
// ����
public:
	CMFCDATADlg(CWnd* pParent = NULL);	// ��׼���캯��

// �Ի�������
	enum { IDD = IDD_MFCDATA_DIALOG };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV ֧��


// ʵ��
protected:
	HICON m_hIcon;

	// ���ɵ���Ϣӳ�亯��
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
	// ͼƬ���ڿؼ�����
	CStatic m_picture;
	CListCtrl m_list;
	// ����ļ�
	CMFCEditBrowseCtrl m_browse_file;
	char*  CMFCDATADlg::GetEditString();
	afx_msg void OnTimer(UINT_PTR nIDEvent);
	afx_msg void OnBnClickedButton1();
	// ��������
	CButton bu_export;
};
