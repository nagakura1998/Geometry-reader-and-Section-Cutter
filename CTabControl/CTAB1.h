#pragma once
#include "Options.h"
// CTAB1 dialog
#define WM_UPDATE_MESH_FILE			40323

class CTAB1 : public CDialogEx
{
	DECLARE_DYNAMIC(CTAB1)

public:
	CTAB1(CWnd* pParent = nullptr);   // standard constructor
	virtual ~CTAB1();

	void InitializeParam();

	void SetParentDlg(CWnd* pParent);

	DirectoryOpt GetDirectoryOption();

// Dialog Data
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_TAB1 };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	DECLARE_MESSAGE_MAP()
	afx_msg void OnBrowseFolder();
	afx_msg void OnBrowseFile();
private:
	DirectoryOpt m_DirOpt;
	CWnd* m_pParent = nullptr;
};
