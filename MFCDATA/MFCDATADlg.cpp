
// MFCDATADlg.cpp : 实现文件
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


// 用于应用程序“关于”菜单项的 CAboutDlg 对话框
CvCapture* capture;
CRect rect;
CDC *pDC;
HDC hDC;
CWnd *pwnd;
CString m_EditString;
Prehelper  *m_prehelper;
// 输入视频流
VideoInfo *m_VideoInfo;
class CAboutDlg : public CDialogEx
{
public:
	CAboutDlg();

// 对话框数据
	enum { IDD = IDD_ABOUTBOX };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

// 实现
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


// CMFCDATADlg 对话框



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


// CMFCDATADlg 消息处理程序

BOOL CMFCDATADlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// 将“关于...”菜单项添加到系统菜单中。

	// IDM_ABOUTBOX 必须在系统命令范围内。
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

	// 设置此对话框的图标。  当应用程序主窗口不是对话框时，框架将自动
	//  执行此操作
	SetIcon(m_hIcon, TRUE);			// 设置大图标
	SetIcon(m_hIcon, FALSE);		// 设置小图标

	// TODO:  在此添加额外的初始化代码
	
	pwnd = GetDlgItem(IDC_PICTURE);
	pDC = pwnd->GetDC();
	hDC = pDC->GetSafeHdc();
	pwnd->GetClientRect(&rect);
	m_prehelper = new Prehelper;
	m_VideoInfo = new VideoInfo;
	m_list.SetExtendedStyle(LVS_EX_FLATSB | LVS_EX_FULLROWSELECT | LVS_EX_HEADERDRAGDROP | LVS_EX_ONECLICKACTIVATE | LVS_EX_GRIDLINES);
	m_list.InsertColumn(1, _T("学号"), LVCFMT_CENTER, 100, -1);
	m_list.InsertColumn(2, _T("姓名"), LVCFMT_CENTER, 80, -1);
	m_list.InsertColumn(3, _T("课程"), LVCFMT_CENTER, 80, -1);
	m_list.InsertColumn(4, _T("班级"), LVCFMT_CENTER, 80, -1);
  
	
		
	



	return TRUE;  // 除非将焦点设置到控件，否则返回 TRUE
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

// 如果向对话框添加最小化按钮，则需要下面的代码
//  来绘制该图标。  对于使用文档/视图模型的 MFC 应用程序，
//  这将由框架自动完成。

void CMFCDATADlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // 用于绘制的设备上下文

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// 使图标在工作区矩形中居中
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// 绘制图标
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialogEx::OnPaint();
	}
}

//当用户拖动最小化窗口时系统调用此函数取得光标
//显示。
HCURSOR CMFCDATADlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}



void CMFCDATADlg::OnEnChangeBrowseFile()
{
	// TODO:  如果该控件是 RICHEDIT 控件，它将不
	// 发送此通知，除非重写 CDialogEx::OnInitDialog()
	// 函数并调用 CRichEditCtrl().SetEventMask()，
	// 同时将 ENM_CHANGE 标志“或”运算到掩码中。
	UpdateData(TRUE);

	GetDlgItemText(IDC_BROWSE_FILE, m_EditString);
	UpdateData(FALSE);
	AfxMessageBox(m_EditString);
	// TODO:  在此添加控件通知处理程序代码
}


void CMFCDATADlg::OnBnClickedButtonOpen()
{
	// TODO:  在此添加控件通知处理程序代码
	m_VideoInfo->m_capture = cvCaptureFromCAM(0);
	if (!m_VideoInfo->m_capture){
		AfxMessageBox(_T("Capture from camera didn't work"));
		return;
	}


	// 设置计时器,每10ms触发一次事件  
	SetTimer(1,100, NULL);
}


void CMFCDATADlg::OnBnClickedButton2()
{
	// TODO:  在此添加控件通知处理程序代码
	cvReleaseCapture(&m_VideoInfo->m_capture);
}


void CMFCDATADlg::OnTimer(UINT_PTR nIDEvent)
{
	// TODO:  在此添加消息处理程序代码和/或调用默认值
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
	// // m_pConnection.CreateInstance(“m_AdoConn.Connection”)
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
	// TODO:  在此添加控件通知处理程序代码
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
				WORD unicode = 0xFEFF;  //这句重要
				file.Write(&unicode, 2);  //这句重要
				file.Write(strColumnName[i], sizeof(wchar_t)*wcslen(strColumnName[i]));
				file.Write("\t", 2);
			}
			
			
		file.Write("\n", 2);
			//获得每一行数据
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

