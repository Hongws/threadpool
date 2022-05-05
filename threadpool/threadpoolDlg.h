
// threadpoolDlg.h: 头文件
//

#pragma once
#include "MyThreadPool.h"
#include "ThreadUse.h"

// CthreadpoolDlg 对话框
class CthreadpoolDlg : public CDialogEx
{
// 构造
public:
	CthreadpoolDlg(CWnd* pParent = nullptr);	// 标准构造函数
	~CthreadpoolDlg();

// 对话框数据
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_THREADPOOL_DIALOG };
#endif

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV 支持

private:
	MyThreadPool* m_pMythradPool;
	CThreadUse* m_pThreadUse;
	
// 实现
protected:
	HICON m_hIcon;

	// 生成的消息映射函数
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedButton1();
	afx_msg void OnBnClickedButton2();
	afx_msg void OnBnClickedButton3();
	afx_msg void OnBnClickedButton4();
	afx_msg void OnBnClickedButton5();
	afx_msg void OnBnClickedButton6();
	afx_msg void OnBnClickedButton7();
	afx_msg void OnBnClickedButton8();
	afx_msg void OnBnClickedButton9();
	afx_msg void OnBnClickedButton10();
	afx_msg void OnBnClickedButton11();
};
