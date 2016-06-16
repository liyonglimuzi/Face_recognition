
// MFCDATADlg.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "MFCDATA.h"
#include "MFCDATADlg.h"
#include"BrowseDir.h"
#include"Prehelper.h"
#include"VideoInfo.h"
#include"CvvImage.h"
#include"ShowData.h"
#include "afxdialogex.h"
#include "Cdata.h"
#include"Cconnection.h"
#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// ����Ӧ�ó��򡰹��ڡ��˵���� CAboutDlg �Ի���
CvCapture* capture;
CRect rect;
CDC *pDC;
HDC hDC;
CWnd *pwnd;
CString m_EditString;
Prehelper  *m_prehelper;
// ������Ƶ��
VideoInfo *m_VideoInfo;
class CAboutDlg : public CDialogEx
{
public:
	CAboutDlg();

// �Ի�������
	enum { IDD = IDD_ABOUTBOX };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

// ʵ��
protected:
	DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CDialogEx(CAboutDlg::IDD)
{
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialogEx)
END_MESSAGE_MAP()


// CMFCDATADlg �Ի���



CMFCDATADlg::CMFCDATADlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(CMFCDATADlg::IDD, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CMFCDATADlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_LIST1, m_list);
	DDX_Control(pDX, IDC_PICTURE, m_picture);
	DDX_Control(pDX, IDC_BROWSE_FILE, m_browse_file);
	DDX_Control(pDX, IDC_BUTTON1, bu_export);
}

BEGIN_MESSAGE_MAP(CMFCDATADlg, CDialogEx)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_EN_CHANGE(IDC_BROWSE_FILE, &CMFCDATADlg::OnEnChangeBrowseFile)
	ON_BN_CLICKED(IDC_BUTTON_OPEN, &CMFCDATADlg::OnBnClickedButtonOpen)
	ON_BN_CLICKED(IDC_BUTTON2, &CMFCDATADlg::OnBnClickedButton2)
	ON_WM_TIMER()
	ON_BN_CLICKED(IDC_BUTTON1, &CMFCDATADlg::OnBnClickedButton1)
END_MESSAGE_MAP()


// CMFCDATADlg ��Ϣ�������

BOOL CMFCDATADlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// ��������...���˵�����ӵ�ϵͳ�˵��С�

	// IDM_ABOUTBOX ������ϵͳ���Χ�ڡ�
	ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
	ASSERT(IDM_ABOUTBOX < 0xF000);

	CMenu* pSysMenu = GetSystemMenu(FALSE);
	if (pSysMenu != NULL)
	{
		BOOL bNameValid;
		CString strAboutMenu;
		bNameValid = strAboutMenu.LoadString(IDS_ABOUTBOX);
		ASSERT(bNameValid);
		if (!strAboutMenu.IsEmpty())
		{
			pSysMenu->AppendMenu(MF_SEPARATOR);
			pSysMenu->AppendMenu(MF_STRING, IDM_ABOUTBOX, strAboutMenu);
		}
	}

	// ���ô˶Ի����ͼ�ꡣ  ��Ӧ�ó��������ڲ��ǶԻ���ʱ����ܽ��Զ�
	//  ִ�д˲���
	SetIcon(m_hIcon, TRUE);			// ���ô�ͼ��
	SetIcon(m_hIcon, FALSE);		// ����Сͼ��

	// TODO:  �ڴ���Ӷ���ĳ�ʼ������
	
	pwnd = GetDlgItem(IDC_PICTURE);
	pDC = pwnd->GetDC();
	hDC = pDC->GetSafeHdc();
	pwnd->GetClientRect(&rect);
	m_prehelper = new Prehelper;
	m_VideoInfo = new VideoInfo;
	m_list.SetExtendedStyle(LVS_EX_FLATSB | LVS_EX_FULLROWSELECT | LVS_EX_HEADERDRAGDROP | LVS_EX_ONECLICKACTIVATE | LVS_EX_GRIDLINES);
	m_list.InsertColumn(1, _T("ѧ��"), LVCFMT_CENTER, 100, -1);
	m_list.InsertColumn(2, _T("����"), LVCFMT_CENTER, 80, -1);
	m_list.InsertColumn(3, _T("�γ�"), LVCFMT_CENTER, 80, -1);
	m_list.InsertColumn(4, _T("�༶"), LVCFMT_CENTER, 80, -1);
  
	
		
	



	return TRUE;  // ���ǽ��������õ��ؼ������򷵻� TRUE
}

void CMFCDATADlg::OnSysCommand(UINT nID, LPARAM lParam)
{
	if ((nID & 0xFFF0) == IDM_ABOUTBOX)
	{
		CAboutDlg dlgAbout;
		dlgAbout.DoModal();
	}
	else
	{
		CDialogEx::OnSysCommand(nID, lParam);
	}
}

// �����Ի��������С����ť������Ҫ����Ĵ���
//  �����Ƹ�ͼ�ꡣ  ����ʹ���ĵ�/��ͼģ�͵� MFC Ӧ�ó���
//  �⽫�ɿ���Զ���ɡ�

void CMFCDATADlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // ���ڻ��Ƶ��豸������

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// ʹͼ���ڹ����������о���
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// ����ͼ��
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialogEx::OnPaint();
	}
}

//���û��϶���С������ʱϵͳ���ô˺���ȡ�ù��
//��ʾ��
HCURSOR CMFCDATADlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}



void CMFCDATADlg::OnEnChangeBrowseFile()
{
	// TODO:  ����ÿؼ��� RICHEDIT �ؼ���������
	// ���ʹ�֪ͨ��������д CDialogEx::OnInitDialog()
	// ���������� CRichEditCtrl().SetEventMask()��
	// ͬʱ�� ENM_CHANGE ��־�������㵽�����С�
	UpdateData(TRUE);

	GetDlgItemText(IDC_BROWSE_FILE, m_EditString);
	UpdateData(FALSE);
	AfxMessageBox(m_EditString);
	// TODO:  �ڴ���ӿؼ�֪ͨ����������
}


void CMFCDATADlg::OnBnClickedButtonOpen()
{
	// TODO:  �ڴ���ӿؼ�֪ͨ����������
	m_VideoInfo->m_capture = cvCaptureFromCAM(0);
	if (!m_VideoInfo->m_capture){
		AfxMessageBox(_T("Capture from camera didn't work"));
		return;
	}


	// ���ü�ʱ��,ÿ10ms����һ���¼�  
	SetTimer(1,100, NULL);
}


void CMFCDATADlg::OnBnClickedButton2()
{
	// TODO:  �ڴ���ӿؼ�֪ͨ����������
	cvReleaseCapture(&m_VideoInfo->m_capture);
}


void CMFCDATADlg::OnTimer(UINT_PTR nIDEvent)
{
	// TODO:  �ڴ������Ϣ�����������/�����Ĭ��ֵ
	IplImage* iplimage;
	iplimage = m_prehelper->StartFace(m_VideoInfo->m_capture);

	CvvImage cvvImage;

	cvvImage.CopyOf(iplimage);

	cvvImage.DrawToHDC(hDC, &rect);
	Data(m_prehelper->getData());
	CDialogEx::OnTimer(nIDEvent);
}
void  CMFCDATADlg::Data(int number)
{
	Cdata m_AdoConn;
	//Prehelper prehel;
	// // m_pConnection.CreateInstance(��m_AdoConn.Connection��)
	//	
	m_AdoConn.OnInitADOConn();

	BOOL judge=TRUE;
	CString num;
	num.Format(TEXT("%d"), number);
	CString	sql = _T("select * from attendant_sheet where NUM like '%" + num + "%'");
	//	
	//	AfxMessageBox(sql);

	m_pRecordset = m_AdoConn.GetRecordSet((_bstr_t)sql);
	while ((m_pRecordset->adoEOF == 0) && (m_pRecordset != NULL))
	{
		num = m_pRecordset->GetCollect("NUMBER");



		m_pRecordset->MoveNext();
	}

	int linecout = m_list.GetItemCount();
	
	for (int i = 0; (i < linecout); i++)
	{
		if (num == m_list.GetItemText(i, 0))

		{
			judge = FALSE;
			break;
		}
	}
	if (judge)
	{
		m_pRecordset = m_AdoConn.GetRecordSet((_bstr_t)sql);
		while (m_pRecordset->adoEOF == 0)
		{
			m_list.InsertItem(linecout, _T(" "));
			
			m_list.SetItemText(linecout, 0, (LPCTSTR)(_bstr_t)m_pRecordset->GetCollect("NUMBER"));
			m_list.SetItemText(linecout, 1, (LPCTSTR)(_bstr_t)m_pRecordset->GetCollect("NAME"));
			m_list.SetItemText(linecout, 2, (LPCTSTR)(_bstr_t)m_pRecordset->GetCollect("PROJECT"));
			
			m_list.SetItemText(linecout, 3, (LPCTSTR)(_bstr_t)m_pRecordset->GetCollect("CLASS"));
			m_pRecordset->MoveNext();
		} 
	}	
		//m_list.InsertItem(0, _T(""));
		m_AdoConn.ExitConnect();
	
		//m_pRecordset->MoveNext();
	
	


}



char*  CMFCDATADlg::GetEditString()
{
	USES_CONVERSION;
	char * pFileName = T2A(m_EditString);

	return pFileName;
}

void CMFCDATADlg::OnBnClickedButton1()
{
	// TODO:  �ڴ���ӿؼ�֪ͨ����������
	CFileDialog filedlg(FALSE,NULL,NULL,OFN_HIDEREADONLY|OFN_OVERWRITEPROMPT,_T("TXT|*.*||"),AfxGetMainWnd());
	CString  strPath,strText;
	if (filedlg.DoModal()==IDOK)
	{
		strPath = filedlg.GetPathName();
		if (strPath.Right(4)!=".txt")
		{
			strPath += ".txt";
			const int COL_NUM = 4;
			CFile file;
			file.Open(strPath, CFile::modeCreate | CFile::modeWrite);
			LVCOLUMNW column;
			char szCol[20];
			
			column.cchTextMax = 20;
			column.mask = LVCF_TEXT;
			CString strColumnName[4] = {0};
			column.pszText = (LPWSTR)szCol;
			for (int i = 0; i <COL_NUM; i++)
			{
				m_list.GetColumn(i, &column);
				strColumnName[i] = column.pszText;
				WORD unicode = 0xFEFF;  //�����Ҫ
				file.Write(&unicode, 2);  //�����Ҫ
				file.Write(strColumnName[i], sizeof(wchar_t)*wcslen(strColumnName[i]));
				file.Write("\t", 2);
			}
			
			
		file.Write("\n", 2);
			//���ÿһ������
			for (int i = 0; i <m_list.GetItemCount(); i++)
			{
				for (int j = 0; j <COL_NUM ; j++)
				{
					CString text = m_list.GetItemText(i, j);
					text += "\t";
					
					file.Write(text, sizeof(wchar_t)*wcslen(text));
				}
				file.Write("\n",2);

			}
			file.Close();
			

		}
	}
}

