
// CTabControlDlg.cpp : implementation file
//

#include "stdafx.h"
#include "CTabControl.h"
#include "CTabControlDlg.h"
#include "afxdialogex.h"
#include "ErrorHandler.h"
#include "Execution.h"
#include "ModelView.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CAboutDlg dialog used for App About

class CAboutDlg : public CDialogEx
{
public:
	CAboutDlg();

// Dialog Data
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_ABOUTBOX };
#endif

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

// Implementation
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


// CCTabControlDlg dialog



CCTabControlDlg::CCTabControlDlg(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_CTABCONTROL_DIALOG, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CCTabControlDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_TABCONTROL, m_tabcontrol);
	DDX_Control(pDX, IDC_LIST1, m_log);
}

BEGIN_MESSAGE_MAP(CCTabControlDlg, CDialogEx)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_NOTIFY(TCN_SELCHANGE, AFX_ID_PREVIEW_PREV, &CCTabControlDlg::OnTcnSelchangeIdPreviewPrev)
	ON_NOTIFY(TCN_SELCHANGE, IDC_TABCONTROL, &CCTabControlDlg::OnSelchangeTabcontrol)
	ON_BN_CLICKED(IDOK, &CCTabControlDlg::OnBnClickedOk)
	ON_BN_CLICKED(IDCANCEL, &CCTabControlDlg::OnBnClickedCancel)
	ON_BN_CLICKED(ID_BTN5, &CCTabControlDlg::OnBnClickedViewModel)
	ON_MESSAGE(WM_UPDATE_MESH_FILE, &CCTabControlDlg::OnUpdateMeshFile)
END_MESSAGE_MAP()


// CCTabControlDlg message handlers

BOOL CCTabControlDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// Add "About..." menu item to system menu.

	// IDM_ABOUTBOX must be in the system command range.
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

	// Set the icon for this dialog.  The framework does this automatically
	//  when the application's main window is not a dialog
	SetIcon(m_hIcon, TRUE);			// Set big icon
	SetIcon(m_hIcon, FALSE);		// Set small icon

	// TODO: Add extra initialization here

	CTabCtrl* pTabCtrl = (CTabCtrl*)GetDlgItem(IDC_TABCONTROL);
	m_tab1.Create(IDD_TAB1, pTabCtrl);
	m_tab1.SetParentDlg(this);

	CTabCtrl* pTabCtrl2 = (CTabCtrl*)GetDlgItem(IDC_TABCONTROL);
	m_tab2.Create(IDD_TAB2, pTabCtrl2);

	TCITEM item1,item2;
	item1.mask = TCIF_TEXT | TCIF_PARAM;
	item1.lParam = (LPARAM)& m_tab1;
	item1.pszText = _T("Directory");
	pTabCtrl->InsertItem(0, &item1);

	item2.mask = TCIF_TEXT | TCIF_PARAM;
	item2.lParam = (LPARAM)& m_tab2;
	item2.pszText = _T("Cut Plane");
	pTabCtrl2->InsertItem(1, &item2);

	CRect rcItem;
	pTabCtrl->GetItemRect(0, &rcItem);
	m_tab1.SetWindowPos(NULL, rcItem.left, rcItem.bottom + 1, 0, 0, SWP_NOSIZE | SWP_NOZORDER);


	CRect rcItem2;
	pTabCtrl2->GetItemRect(0, &rcItem2);
	m_tab2.SetWindowPos(NULL, rcItem2.left, rcItem2.bottom + 1, 0, 0, SWP_NOSIZE | SWP_NOZORDER);

	m_tab1.InitializeParam();
	m_tab2.InitializeParam();

	m_tab1.ShowWindow(SW_SHOW);
	m_tab2.ShowWindow(SW_HIDE);
	
	return TRUE;  // return TRUE  unless you set the focus to a control
}

void CCTabControlDlg::OnSysCommand(UINT nID, LPARAM lParam)
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

// If you add a minimize button to your dialog, you will need the code below
//  to draw the icon.  For MFC applications using the document/view model,
//  this is automatically done for you by the framework.

void CCTabControlDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // device context for painting

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// Center icon in client rectangle
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// Draw the icon
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialogEx::OnPaint();
	}
}

// The system calls this function to obtain the cursor to display while the user drags
//  the minimized window.
HCURSOR CCTabControlDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}



void CCTabControlDlg::OnTcnSelchangeIdPreviewPrev(NMHDR *pNMHDR, LRESULT *pResult)
{
	// TODO: Add your control notification handler code here
	*pResult = 0;
}


void CCTabControlDlg::OnSelchangeTabcontrol(NMHDR *pNMHDR, LRESULT *pResult)
{
	// TODO: Add your control notification handler code here
	*pResult = 0;

	int iSel = m_tabcontrol.GetCurSel();

	if (iSel == 0)
	{
		m_tab1.ShowWindow(SW_SHOW);
		m_tab2.ShowWindow(SW_HIDE);
	}
	else if (iSel == 1)
	{
		m_tab2.ShowWindow(SW_SHOW);
		m_tab1.ShowWindow(SW_HIDE);
	}
	else if (iSel == 2)
	{
		m_tab1.ShowWindow(SW_HIDE);
		m_tab2.ShowWindow(SW_HIDE);
	}
}

void CCTabControlDlg::OnBnClickedOk()
{
	// TODO: Add your control notification handler code here
	if (DoOperation()) {

	}
	//CDialogEx::OnOK();
}


void CCTabControlDlg::OnBnClickedCancel()
{
	// TODO: Add your control notification handler code here
	CDialogEx::OnCancel();
}

void CCTabControlDlg::OnBnClickedViewModel()
{
	auto view = CModelView::CreateModelView();
	view->OnDraw();
}

LRESULT CCTabControlDlg::OnUpdateMeshFile(WPARAM wParam, LPARAM lParam)
{
	auto dirOpt = m_tab1.GetDirectoryOption();
	CT2CA val(dirOpt.keywordFile);
	std::string filePath(val);

	m_pre = new DataPreprocessing(filePath);
	m_pre->SetActiveDlg(this);

	m_pre->Preprocessing();

	auto reader = m_pre->GetReader();

	std::vector <Vertex> vertices;
	std::vector <GLuint> indexes;
	std::unordered_map<int, int>newNodeID;
	int nNodes = 0;
	//Generate faces
	/*for (auto itr = reader->m_elems.begin(); itr != reader->m_elems.end(); itr++) {
		if (itr->second.size() == 4) {
			int order[2][3] = { {0,1,2},{0,2,3} };
			for (int i = 0; i < 2; i++) {
				for (int j = 0; j < 3; j++) {
					if (newNodeID.find(itr->second[order[i][j]]) == newNodeID.end()) {
						newNodeID[itr->second[order[i][j]]] = nNodes;
						indexes.push_back(nNodes);
						vertices.push_back(Vertex{ glm::vec3(
							reader->m_nodes[itr->second[order[i][j]]].vec[0],
							reader->m_nodes[itr->second[order[i][j]]].vec[1],
							reader->m_nodes[itr->second[order[i][j]]].vec[2]) });
						nNodes++;
					}
					else {
						indexes.push_back(newNodeID[itr->second[order[i][j]]]);
					}
				}
			}
		}
		else if (itr->second.size() == 3) {
			for (int nID = 0; nID < itr->second.size(); nID++) {
				if (newNodeID.find(itr->second[nID]) == newNodeID.end()) {
					newNodeID[itr->second[nID]] = nNodes;
					indexes.push_back(nNodes);
					vertices.push_back(Vertex{ glm::vec3(
						reader->m_nodes[itr->second[nID]].vec[0],
						reader->m_nodes[itr->second[nID]].vec[1],
						reader->m_nodes[itr->second[nID]].vec[2]) });
					nNodes++;
				}
				else {
					indexes.push_back(newNodeID[itr->second[nID]]);
				}
				
			}
		}
		
	}*/

	//GenerateLine
	for (auto itr = reader->m_elems.begin(); itr != reader->m_elems.end(); itr++) {
		if (itr->second.size() == 4) {
			int order[2][3] = { {0,1,2},{0,2,3} };
			for (int i = 0; i < 2; i++) {
				for (int j = 0; j < 3; j++) {
					if (newNodeID.find(itr->second[order[i][j] % 3]) == newNodeID.end()) {
						newNodeID[itr->second[order[i][j] % 3]] = nNodes;
						indexes.push_back(nNodes);
						vertices.push_back(Vertex{ glm::vec3(
							reader->m_nodes[itr->second[order[i][j] % 3]].vec[0],
							reader->m_nodes[itr->second[order[i][j] % 3]].vec[1],
							reader->m_nodes[itr->second[order[i][j] % 3]].vec[2]) });
						nNodes++;
					}
					else {
						indexes.push_back(newNodeID[itr->second[order[i][j] % 3]]);
					}
					if (newNodeID.find(itr->second[(order[i][j] + 1) % 3]) == newNodeID.end()) {
						newNodeID[itr->second[(order[i][j] + 1) % 3]] = nNodes;
						indexes.push_back(nNodes);
						vertices.push_back(Vertex{ glm::vec3(
							reader->m_nodes[itr->second[(order[i][j] + 1) % 3]].vec[0],
							reader->m_nodes[itr->second[(order[i][j] + 1) % 3]].vec[1],
							reader->m_nodes[itr->second[(order[i][j] + 1) % 3]].vec[2]) });
						nNodes++;
					}
					else {
						indexes.push_back(newNodeID[itr->second[(order[i][j] + 1) % 3]]);
					}
				}
			}
		}
		else if (itr->second.size() == 3) {
			for (int nID = 0; nID < itr->second.size(); nID++) {
				indexes.push_back(nNodes++);
				vertices.push_back(Vertex{ glm::vec3(
					reader->m_nodes[itr->second[nID % 3]].vec[0],
					reader->m_nodes[itr->second[nID % 3]].vec[1],
					reader->m_nodes[itr->second[nID % 3]].vec[2]) });
				indexes.push_back(nNodes);
				vertices.push_back(Vertex{ glm::vec3(
					reader->m_nodes[itr->second[(nID + 1) % 3]].vec[0],
					reader->m_nodes[itr->second[(nID + 1) % 3]].vec[1],
					reader->m_nodes[itr->second[(nID + 1) % 3]].vec[2]) });
			}
		}

	}

	auto view = CModelView::CreateModelView();
	view->m_vertices = std::move(vertices);
	view->m_indexes = std::move(indexes);

	return 0;
}

bool CCTabControlDlg::DoOperation()
{
	auto dirOpt = m_tab1.GetDirectoryOption();
	auto cutPlaneOpt = m_tab2.GetCutPlaneOption();

	try {
		ErrorHandler::CheckPathExist(_T("STL file"), dirOpt.keywordFile);
		ErrorHandler::CheckPathExist(_T("Logging Directory"), dirOpt.logDir);
	}
	catch (MLError err){
		AfxMessageBox(err.GetMessage(), MB_ICONERROR);
		return false;
	}

	m_log.AddString(_T("Configuration:"));
	m_log.AddString(_T("-STL file Path: ") + dirOpt.keywordFile);
	m_log.AddString(_T("-Logging Directory: ") + dirOpt.logDir);

	m_log.AddString(_T("Preprocessing..."));

	m_log.UpdateWindow();
	
	CCmdGeomClassification cmd(dirOpt, cutPlaneOpt);
	cmd.SetActiveDlg(this);
	if (!cmd.Execute()) {
		m_log.AddString(_T("Something wrong! Check the step which was still processing!"));
	}
	return true;
}
