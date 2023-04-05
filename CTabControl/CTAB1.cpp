// CTAB1.cpp : implementation file
//

#include "stdafx.h"
#include "CTabControl.h"
#include "CTAB1.h"
#include "afxdialogex.h"


// CTAB1 dialog

IMPLEMENT_DYNAMIC(CTAB1, CDialogEx)

CTAB1::CTAB1(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_TAB1, pParent)
{

}

CTAB1::~CTAB1()
{
}

void CTAB1::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_EDIT1, m_DirOpt.keywordFile);
	DDX_Text(pDX, IDC_EDIT2, m_DirOpt.logDir);
}


BEGIN_MESSAGE_MAP(CTAB1, CDialogEx)
	ON_BN_CLICKED(ID_BTN1, &CTAB1::OnBrowseFile)
	ON_BN_CLICKED(ID_BTN2, &CTAB1::OnBrowseFolder)
END_MESSAGE_MAP()

// CTAB1 message handlers

void CTAB1::InitializeParam()
{
	int cx = GetSystemMetrics(SM_CXSMICON);  // scaled for system DPI!
	int cy = GetSystemMetrics(SM_CYSMICON);  // scaled for system DPI!

	HICON hIcon = NULL;
	HRESULT hr = LoadIconWithScaleDown(AfxGetInstanceHandle(), MAKEINTRESOURCE(IDC_SELECTPATH),
		cx, cy, &hIcon);
	if (SUCCEEDED(hr)) {
		((CButton*)GetDlgItem(ID_BTN1))->SetIcon(hIcon);
		((CButton*)GetDlgItem(ID_BTN2))->SetIcon(hIcon);
	}
}

void CTAB1::SetParentDlg(CWnd* pParent)
{
	m_pParent = pParent;
}

DirectoryOpt CTAB1::GetDirectoryOption()
{
	UpdateData();
	return m_DirOpt;
}

void CTAB1::OnBrowseFolder()
{
	CFolderPickerDialog dlgFolder(NULL, 0, NULL, 0);
	if (dlgFolder.DoModal() != IDOK) {
		return;
	}

	CString FolderPath = dlgFolder.GetFolderPath();

	((CEdit*)GetDlgItem(IDC_EDIT2))->SetWindowTextW(FolderPath);
	
}

void CTAB1::OnBrowseFile()
{
	CString szFilter = _T("Keywork File (*.*)|*.*||");
	CString strPath;

	CFileDialog fileDlg(TRUE, _T("*.*"), NULL, OFN_OVERWRITEPROMPT, szFilter);

	if (fileDlg.DoModal() == IDOK)
	{
		strPath = fileDlg.GetPathName();
		((CEdit*)GetDlgItem(IDC_EDIT1))->SetWindowTextW(strPath);

		m_pParent->SendMessage(WM_UPDATE_MESH_FILE, 0, 0);
	}
}
