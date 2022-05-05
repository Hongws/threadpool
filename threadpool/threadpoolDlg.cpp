
// threadpoolDlg.cpp: 实现文件
//

#include "pch.h"
#include "framework.h"
#include "threadpool.h"
#include "threadpoolDlg.h"
#include "afxdialogex.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// 用于应用程序“关于”菜单项的 CAboutDlg 对话框

class CAboutDlg : public CDialogEx
{
public:
	CAboutDlg();

// 对话框数据
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_ABOUTBOX };
#endif

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

// 实现
protected:
	DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CDialogEx(IDD_ABOUTBOX)
{
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialogEx)
END_MESSAGE_MAP()


// CthreadpoolDlg 对话框



CthreadpoolDlg::CthreadpoolDlg(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_THREADPOOL_DIALOG, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
	m_pMythradPool = nullptr;
	m_pThreadUse = nullptr;
}

CthreadpoolDlg::~CthreadpoolDlg()
{
	if (m_pMythradPool)
	{
		delete m_pMythradPool;
		m_pMythradPool = nullptr;
	}	

	if (m_pThreadUse)
	{
		delete m_pThreadUse;
		m_pThreadUse = nullptr;
	}
}

void CthreadpoolDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CthreadpoolDlg, CDialogEx)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(IDC_BUTTON1, &CthreadpoolDlg::OnBnClickedButton1)
	ON_BN_CLICKED(IDC_BUTTON2, &CthreadpoolDlg::OnBnClickedButton2)
	ON_BN_CLICKED(IDC_BUTTON3, &CthreadpoolDlg::OnBnClickedButton3)
	ON_BN_CLICKED(IDC_BUTTON4, &CthreadpoolDlg::OnBnClickedButton4)
	ON_BN_CLICKED(IDC_BUTTON5, &CthreadpoolDlg::OnBnClickedButton5)
	ON_BN_CLICKED(IDC_BUTTON6, &CthreadpoolDlg::OnBnClickedButton6)
	ON_BN_CLICKED(IDC_BUTTON7, &CthreadpoolDlg::OnBnClickedButton7)
	ON_BN_CLICKED(IDC_BUTTON8, &CthreadpoolDlg::OnBnClickedButton8)
	ON_BN_CLICKED(IDC_BUTTON9, &CthreadpoolDlg::OnBnClickedButton9)
	ON_BN_CLICKED(IDC_BUTTON10, &CthreadpoolDlg::OnBnClickedButton10)
	ON_BN_CLICKED(IDC_BUTTON11, &CthreadpoolDlg::OnBnClickedButton11)
END_MESSAGE_MAP()


// CthreadpoolDlg 消息处理程序

BOOL CthreadpoolDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// 将“关于...”菜单项添加到系统菜单中。

	// IDM_ABOUTBOX 必须在系统命令范围内。
	ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
	ASSERT(IDM_ABOUTBOX < 0xF000);

	CMenu* pSysMenu = GetSystemMenu(FALSE);
	if (pSysMenu != nullptr)
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

	// TODO: 在此添加额外的初始化代码

	return TRUE;  // 除非将焦点设置到控件，否则返回 TRUE
}

void CthreadpoolDlg::OnSysCommand(UINT nID, LPARAM lParam)
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

void CthreadpoolDlg::OnPaint()
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
HCURSOR CthreadpoolDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}



void CthreadpoolDlg::OnBnClickedButton1()
{
	// TODO: 在此添加控件通知处理程序代码
	if (!m_pMythradPool)
	{
		m_pMythradPool = new MyThreadPool();
		m_pMythradPool->CreateWorkThread();
	}
}


void CthreadpoolDlg::OnBnClickedButton2()
{
	// TODO: 在此添加控件通知处理程序代码
	if (m_pMythradPool)
	{
		CString cstr;
		cstr.Format(L"%d", m_pMythradPool->GetSizeMessageQueue());
		AfxMessageBox(cstr);
	}	
}


void CthreadpoolDlg::OnBnClickedButton3()
{
	// TODO: 在此添加控件通知处理程序代码
	if (m_pMythradPool)
	{
		CString cstr;
		cstr.Format(L"%d", m_pMythradPool->GetSizeCurTask());
		AfxMessageBox(cstr);
	}
}


void CthreadpoolDlg::OnBnClickedButton4()
{
	// TODO: 在此添加控件通知处理程序代码
	if (m_pMythradPool)
	{
		m_pMythradPool->StartEvent();
	}
}


void CthreadpoolDlg::OnBnClickedButton5()
{
	// TODO: 在此添加控件通知处理程序代码
	if (m_pMythradPool)
	{
		m_pMythradPool->TestPushQueue();
	}
}




void CthreadpoolDlg::OnBnClickedButton6()
{
	// TODO: 在此添加控件通知处理程序代码
	if (!m_pThreadUse)
	{
		m_pThreadUse = new CThreadUse();
		m_pThreadUse->CreateTaskThread();
	}
}


void CthreadpoolDlg::OnBnClickedButton7()
{
	// TODO: 在此添加控件通知处理程序代码
	if (m_pThreadUse)
	{		
		CString cstr;
		cstr.Format(L"%d", m_pThreadUse->GetQueueSize());
		AfxMessageBox(cstr);
	}
}


void CthreadpoolDlg::OnBnClickedButton8()
{
	// TODO: 在此添加控件通知处理程序代码
	if (m_pThreadUse)
	{
		CString cstr;
		cstr.Format(L"%d", m_pThreadUse->GetTaskThreadSize());
		AfxMessageBox(cstr);
	}
}


void CthreadpoolDlg::OnBnClickedButton9()
{
	// TODO: 在此添加控件通知处理程序代码
}


void CthreadpoolDlg::OnBnClickedButton10()
{
	// TODO: 在此添加控件通知处理程序代码
	if (m_pThreadUse)
	{
		m_pThreadUse->PushQueue();
	}

}


void CthreadpoolDlg::OnBnClickedButton11()
{
	// TODO: 在此添加控件通知处理程序代码
	if (m_pThreadUse)
	{
		m_pThreadUse->PushQueueEx();
	}
}
