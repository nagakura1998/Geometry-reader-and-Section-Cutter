
// CTabControlDlg.h : header file
//

#pragma once
#include "CTAB1.h"
#include "CTAB2.h"
#include "Preprocess.h"

// CCTabControlDlg dialog
class CCTabControlDlg : public CDialogEx
{
// Construction
public:
	CCTabControlDlg(CWnd* pParent = nullptr);	// standard constructor
	
// Dialog Data
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_CTABCONTROL_DIALOG };
#endif

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV support


// Implementation
protected:
	HICON m_hIcon;
	// Generated message map functions
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	LRESULT OnUpdateMeshFile(WPARAM wParam, LPARAM lParam);
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnTcnSelchangeIdPreviewPrev(NMHDR *pNMHDR, LRESULT *pResult);
	CListBox m_log;
	CTabCtrl m_tabcontrol;
	CTAB1 m_tab1;
	CTAB2 m_tab2;
	DataPreprocessing* m_pre{nullptr};
	afx_msg void OnSelchangeTabcontrol(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnBnClickedOk();
	afx_msg void OnBnClickedCancel();
	afx_msg void OnBnClickedViewModel();
	bool DoOperation();
};
