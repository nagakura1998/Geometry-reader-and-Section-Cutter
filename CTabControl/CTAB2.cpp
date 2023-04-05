// CTAB2.cpp : implementation file
//

#include "stdafx.h"
#include "CTabControl.h"
#include "CTAB2.h"
#include "afxdialogex.h"


// CTAB2 dialog

IMPLEMENT_DYNAMIC(CTAB2, CDialogEx)

CTAB2::CTAB2(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_TAB2, pParent)
{

}

CTAB2::~CTAB2()
{
}

void CTAB2::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_EDIT5, m_CutPlaneOpt.X);
	DDX_Text(pDX, IDC_EDIT6, m_CutPlaneOpt.Y);
	DDX_Text(pDX, IDC_EDIT7, m_CutPlaneOpt.Z);
	DDX_Check(pDX, IDC_CHECK1, m_CutPlaneOpt.bCutByX);
	DDX_Check(pDX, IDC_CHECK2, m_CutPlaneOpt.bCutByY);
	DDX_Check(pDX, IDC_CHECK3, m_CutPlaneOpt.bCutByZ);
}

BEGIN_MESSAGE_MAP(CTAB2, CDialogEx)
	ON_BN_CLICKED(IDC_CHECK1, &CTAB2::OnCheckX)
	ON_BN_CLICKED(IDC_CHECK2, &CTAB2::OnCheckY)
	ON_BN_CLICKED(IDC_CHECK3, &CTAB2::OnCheckZ)
	//ON_EN_CHANGE(IDC_EDIT1, &CTAB2::OnEnChangeEdit1)
END_MESSAGE_MAP()


// CTAB2 message handlers


void CTAB2::OnEnChangeEdit1()
{
	// TODO:  If this is a RICHEDIT control, the control will not
	// send this notification unless you override the CDialogEx::OnInitDialog()
	// function and call CRichEditCtrl().SetEventMask()
	// with the ENM_CHANGE flag ORed into the mask.

	// TODO:  Add your control notification handler code here
}

void CTAB2::InitializeParam()
{
	UpdateData(FALSE);
}

void CTAB2::OnCheckX()
{
	UpdateData();
	if (m_CutPlaneOpt.bCutByX) {
		GetDlgItem(IDC_EDIT5)->EnableWindow(TRUE);
		GetDlgItem(IDC_STATIC1)->EnableWindow(TRUE);
	}
	else {
		GetDlgItem(IDC_EDIT5)->EnableWindow(FALSE);
		GetDlgItem(IDC_STATIC1)->EnableWindow(FALSE);
	}
}

void CTAB2::OnCheckY()
{
	UpdateData();
	if (m_CutPlaneOpt.bCutByY) {
		GetDlgItem(IDC_EDIT6)->EnableWindow(TRUE);
		GetDlgItem(IDC_STATIC2)->EnableWindow(TRUE);
	}
	else {
		GetDlgItem(IDC_EDIT6)->EnableWindow(FALSE);
		GetDlgItem(IDC_STATIC2)->EnableWindow(FALSE);
	}
}

void CTAB2::OnCheckZ()
{
	UpdateData();
	if (m_CutPlaneOpt.bCutByZ) {
		GetDlgItem(IDC_EDIT7)->EnableWindow(TRUE);
		GetDlgItem(IDC_STATIC3)->EnableWindow(TRUE);
	}
	else {
		GetDlgItem(IDC_EDIT7)->EnableWindow(FALSE);
		GetDlgItem(IDC_STATIC3)->EnableWindow(FALSE);
	}
}

CutPlaneOpt CTAB2::GetCutPlaneOption()
{
	UpdateData();
	return m_CutPlaneOpt;
}