#pragma once
#include "Options.h"

// CTAB2 dialog

class CTAB2 : public CDialogEx
{
	DECLARE_DYNAMIC(CTAB2)

public:
	CTAB2(CWnd* pParent = nullptr);   // standard constructor
	virtual ~CTAB2();

// Dialog Data
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_TAB2 };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
	afx_msg void OnEnChangeEdit1();
	afx_msg void OnCheckX();
	afx_msg void OnCheckY();
	afx_msg void OnCheckZ();
public:
	void InitializeParam();
	CutPlaneOpt GetCutPlaneOption();
private:
	CutPlaneOpt m_CutPlaneOpt;
};
