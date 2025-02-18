
// Project1Dlg.cpp: 구현 파일
//

#include "pch.h"
#include "framework.h"
#include "Project1.h"
#include "Project1Dlg.h"
#include "afxdialogex.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif
#include <random>


// 응용 프로그램 정보에 사용되는 CAboutDlg 대화 상자입니다.

class CAboutDlg : public CDialogEx
{
public:
	CAboutDlg();

// 대화 상자 데이터입니다.
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_ABOUTBOX };
#endif

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.

// 구현입니다.
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


// CProject1Dlg 대화 상자

UINT ThreadFunc(LPVOID param)
{
	CProject1Dlg* pMainDlg = (CProject1Dlg*)param;

	TRACE("ThreadFunc: Start\n");

	while (pMainDlg->RandomMoveMode())
	{
		Sleep(2000);
	}

	TRACE("ThreadFunc: Stop\n");
	return 0;
}

CProject1Dlg::CProject1Dlg(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_PROJECT1_DIALOG, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CProject1Dlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_EDIT1, m_RadiusEdit);
	DDX_Control(pDX, IDC_EDIT2, m_TicknessEdit);
	DDX_Control(pDX, IDC_STATIC3, m_CircleInfo);
	DDX_Control(pDX, IDC_BUTTON1, m_TestButton);
	DDX_Control(pDX, IDC_BUTTON2, m_InitButton);
	DDX_Control(pDX, IDC_BUTTON3, m_RandomMoveButton);
}

BEGIN_MESSAGE_MAP(CProject1Dlg, CDialogEx)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(IDC_BUTTON1, &CProject1Dlg::OnBnClickedButton1)
	ON_WM_SHOWWINDOW()
	ON_BN_CLICKED(IDC_BUTTON2, &CProject1Dlg::OnBnClickedButton2)
	ON_BN_CLICKED(IDC_BUTTON3, &CProject1Dlg::OnBnClickedButton3)
	ON_WM_LBUTTONDOWN()
	ON_WM_MOUSEMOVE()
	ON_WM_LBUTTONUP()
	ON_WM_CLOSE()
	ON_WM_TIMER()
END_MESSAGE_MAP()


// CProject1Dlg 메시지 처리기

BOOL CProject1Dlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// 시스템 메뉴에 "정보..." 메뉴 항목을 추가합니다.

	// IDM_ABOUTBOX는 시스템 명령 범위에 있어야 합니다.
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

	// 이 대화 상자의 아이콘을 설정합니다.  응용 프로그램의 주 창이 대화 상자가 아닐 경우에는
	//  프레임워크가 이 작업을 자동으로 수행합니다.
	SetIcon(m_hIcon, TRUE);			// 큰 아이콘을 설정합니다.
	SetIcon(m_hIcon, FALSE);		// 작은 아이콘을 설정합니다.

	// TODO: 여기에 추가 초기화 작업을 추가합니다.
	
	m_RadiusEdit.SetWindowText(TEXT("20"));
	m_TicknessEdit.SetWindowText(TEXT("10"));
	m_CircleInfo.SetWindowText(TEXT("(0,0)"));

	m_Canvas.m_nLeft = 0; // 이미지 시작 좌표
	m_Canvas.m_nTop = 0;

#if _DEBUG
	RECT rect;

	GetWindowRect(&rect);
	TRACE(TEXT("(%ld, %ld, %ld, %ld)\n"), rect.left, rect.top, rect.right, rect.bottom);
#
	GetClientRect(&rect);
	TRACE(TEXT("(%ld, %ld, %ld, %ld)\n"), rect.left, rect.top, rect.right, rect.bottom);
#endif

	rect.right = rect.left + 1024;
	rect.bottom = rect.top + 768;
	MoveWindow(&rect);

	InitChildControls();

	return TRUE;  // 포커스를 컨트롤에 설정하지 않으면 TRUE를 반환합니다.
}

void CProject1Dlg::OnSysCommand(UINT nID, LPARAM lParam)
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

// 대화 상자에 최소화 단추를 추가할 경우 아이콘을 그리려면
//  아래 코드가 필요합니다.  문서/뷰 모델을 사용하는 MFC 애플리케이션의 경우에는
//  프레임워크에서 이 작업을 자동으로 수행합니다.

void CProject1Dlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // 그리기를 위한 디바이스 컨텍스트입니다.

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// 클라이언트 사각형에서 아이콘을 가운데에 맞춥니다.
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// 아이콘을 그립니다.
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CPaintDC dc(this);
		m_Canvas.Draw(dc.GetSafeHdc());

		CDialogEx::OnPaint();
	}
}

// 사용자가 최소화된 창을 끄는 동안에 커서가 표시되도록 시스템에서
//  이 함수를 호출합니다.
HCURSOR CProject1Dlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}

int CProject1Dlg::GetRadius()
{
	CString text;
	int value = 0;
	m_RadiusEdit.GetWindowText(text);
	//text.Format(_T("%d"), value);
	value = _ttoi(text);

	if (value < 10)
	{
		value = 10;
		m_RadiusEdit.SetWindowText(_TEXT("10"));
	}

	if (value > 20)
	{
		value = 20;
		m_RadiusEdit.SetWindowText(_TEXT("20"));
	}

	return value;
}

int CProject1Dlg::GetTickness()
{
	CString text;
	int value = 0;
	m_TicknessEdit.GetWindowText(text);
	//text.Format(_T("%d"), value);
	value = _ttoi(text);

	if (value < 2)
	{
		value = 2;
		m_TicknessEdit.SetWindowText(_TEXT("2"));
	}

	if (value > 10)
	{
		value = 10;
		m_TicknessEdit.SetWindowText(_TEXT("10"));
	}

	return value;
}
void CProject1Dlg::OnBnClickedButton1()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	m_nCircleMode = 0;
	m_Canvas.Clear();

	CRect rect = m_Canvas.GetRect();

	m_Canvas.m_nTickness = GetTickness();

	int nRadius = GetRadius();

	// 랜덤 함수
	std::random_device rn;			//시드 값
	std::mt19937_64 rnd(rn());	//random_device를 시드 값으로 하여 초기화

	//m_Canvas.CreateCircle(0, 0, nRadius);
	//m_Canvas.CreateCircle(100, 100, nRadius);
	//m_Canvas.CreateCircle(200, 200, nRadius);
	//m_Canvas.CreateCircle(300, 300, nRadius);

	// 랜덤 위치
	for (int i = 1; i < 4; i++)
	{
		std::uniform_real_distribution<double> nX(0 + nRadius, m_Canvas.m_nWidth - nRadius);
		int x = nX(rnd);

		std::uniform_real_distribution<double> nY(0 + nRadius, m_Canvas.m_nHeight - nRadius);
		int y = nY(rnd);

		int nStartX = x - nRadius - rect.left;
		int nStartY = y - nRadius - rect.top;
		m_Canvas.CreateCircle(nStartX, nStartY, nRadius);
	}

	m_Canvas.UpdateCircleCenter();
	m_Canvas.Update();
	//CRect rect(0, 0, 640, 480);
	//InvalidateRect(&rect);
	DrawCircleInfo();
	Invalidate(FALSE);
}

void CProject1Dlg::OnShowWindow(BOOL bShow, UINT nStatus) 
	{
		if (bShow)
		{

		}
	}


void CProject1Dlg::OnBnClickedButton2()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	m_nCircleMode = 0;

	m_Canvas.Clear();
	DrawCircleInfo();
	Invalidate(FALSE); // 전체 화면을 다시 그린다.
}


void CProject1Dlg::OnBnClickedButton3()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.

	if (m_nCircleMode == 3)
	{
		return;
	}

	EnableButtons(FALSE);

	m_nCircleMode = 3;
	m_nRandomMoveCount = 0;

	//DrawRandomMove();

	m_isWorkingThread = true;
	m_pThread = AfxBeginThread(ThreadFunc, this);

	//SetTimer(1, 2000, NULL);
}


void CProject1Dlg::OnLButtonDown(UINT nFlags, CPoint point)
{
	// TODO: 여기에 메시지 처리기 코드를 추가 및/또는 기본값을 호출합니다.
	if (m_nCircleMode == 3)
		return;

	m_nCircleMode = 0;
	m_nCircleIndex = -1;
	CRect rect = m_Canvas.GetRect();
	// 이미지 클릭시
	if (PtInRect(rect, point))
	{
		m_Canvas.m_nTickness = GetTickness();
		int nRadius = GetRadius();

		if (m_Canvas.m_nCircleCount >= 3)
		{
			CPoint point1;
			point1.x = point.x - rect.left; // 이미지 시작 좌표만큼 빼준다.
			point1.y = point.y - rect.top;
			m_nCircleIndex = m_Canvas.FindCircle(point1);
			if (m_nCircleIndex > 0) // 0은 드래그 무시.
			{
				m_nCircleMode = 2;
				m_CurPos = point;
				TRACE("[DOWN] m_CurPos: (%d, %d)\n", m_CurPos.x, m_CurPos.y);
				return;
			}
			else
			{
				return;
			}
		}

		// 빈 영역 클릭시 : 원 추가
		else
		{
			// 원 클릭시 : 원 드래그 후 다시 그리기
			m_nCircleMode = 1;

			// 클릭 좌표는 센터 좌표로 저장하여 시작 좌표를 계산하여 전달.
			int nStartX = point.x - nRadius - rect.left;
			int nStartY = point.y - nRadius - rect.top;
			m_Canvas.CreateCircle(nStartX, nStartY, nRadius);

			// ****** 정원 위치 보정 ******
			m_Canvas.UpdateCircleCenter();

			m_Canvas.Update();
		}
	}

	CDialogEx::OnLButtonDown(nFlags, point);
}


void CProject1Dlg::OnMouseMove(UINT nFlags, CPoint point)
{
	// TODO: 여기에 메시지 처리기 코드를 추가 및/또는 기본값을 호출합니다.
	if (m_nCircleMode == 2)
	{
		m_CurPos = point; // 마우스 좌표.
		CRect rect = m_Canvas.GetRect();
		int nRadius = GetRadius();
		int nStartX = m_CurPos.x - nRadius - rect.left;
		int nStartY = m_CurPos.y - nRadius - rect.top;
		TRACE("[MOVE] m_CurPos: (%d, %d)\n", m_CurPos.x, m_CurPos.y);
		m_Canvas.UpdateCircle(m_nCircleIndex, nStartX, nStartY, nRadius);

		// ****** 정원 위치 보정 ******
		m_Canvas.UpdateCircleCenter();

		m_Canvas.Update();
		DrawCircleInfo();
		Invalidate(FALSE);
		//CRect rect1(0, 0, 640, 480);
		//InvalidateRect(&rect1, FALSE);
	}

	CDialogEx::OnMouseMove(nFlags, point);
}


void CProject1Dlg::OnLButtonUp(UINT nFlags, CPoint point)
{
	// TODO: 여기에 메시지 처리기 코드를 추가 및/또는 기본값을 호출합니다.
	if (m_nCircleMode == 2) // EDIT
	{
		m_nCircleMode = 0;
	}
	else if (m_nCircleMode == 1) // INSERT
	{
		m_nCircleMode = 0;
		DrawCircleInfo();
		Invalidate(FALSE);
	}

	CDialogEx::OnLButtonUp(nFlags, point);
}


void CProject1Dlg::DrawCircleInfo()
{

	// 라벨로 바꾸기. \n으로 한번에 출력하기
	CString str;
	str.Format(TEXT("P0: (%d, %d)\r\nP1: (%d, %d)\r\nP2: (%d, %d)\nP4: (%d, %d)\n"),
		m_Canvas.m_Circle[0].GetCenterX(), m_Canvas.m_Circle[0].GetCenterY(),
		m_Canvas.m_Circle[1].GetCenterX(), m_Canvas.m_Circle[1].GetCenterY(),
		m_Canvas.m_Circle[2].GetCenterX(), m_Canvas.m_Circle[2].GetCenterY(),
		m_Canvas.m_Circle[3].GetCenterX(), m_Canvas.m_Circle[3].GetCenterY());
	m_CircleInfo.SetWindowText(str);
	TRACE(str);
}

void CProject1Dlg::OnClose()
{
	// TODO: 여기에 메시지 처리기 코드를 추가 및/또는 기본값을 호출합니다.

	//if (m_nCircleMode == 3 && m_nRandomMoveCount < 10)
	//	KillTimer(1);

	if (m_pThread && m_isWorkingThread)
	{
		m_isWorkingThread = false;
		WaitForSingleObject(m_pThread->m_hThread, 2000); // 쓰레드 자동 종료 됨.
		//delete m_pThread;
		m_pThread = NULL;
	}
	CDialogEx::OnClose();
}


void CProject1Dlg::OnTimer(UINT_PTR nIDEvent)
{
	// TODO: 여기에 메시지 처리기 코드를 추가 및/또는 기본값을 호출합니다.
	if (nIDEvent == 1)
	{
		if (m_nCircleMode == 3)
		{
			if (m_nRandomMoveCount < 10)
			{
				DrawRandomMove();

				m_nRandomMoveCount++;
				TRACE("%d\n", m_nRandomMoveCount);
			}
			
			if (m_nRandomMoveCount >= 10)
			{
				m_nCircleMode = 0;
				m_nRandomMoveCount = 0;
				EnableButtons(TRUE);
				KillTimer(1);
			}
		}
	}
	CDialogEx::OnTimer(nIDEvent);
}

void CProject1Dlg::DrawRandomMove()
{
	CRect rect = m_Canvas.GetRect();

	m_Canvas.m_nTickness = GetTickness();

	int nRadius = GetRadius();

	// 랜덤 함수
	std::random_device rn;			//시드 값
	std::mt19937_64 rnd(rn());	//random_device를 시드 값으로 하여 초기화

	// 랜덤 위치
	for (int i = 1; i < 4; i++)
	{
		std::uniform_real_distribution<double> nX(0 + nRadius, m_Canvas.m_nWidth - nRadius);
		int x = nX(rnd);

		std::uniform_real_distribution<double> nY(0 + nRadius, m_Canvas.m_nHeight - nRadius);
		int y = nY(rnd);

		int nStartX = x - nRadius - rect.left;
		int nStartY = y - nRadius - rect.top;
		m_Canvas.UpdateCircle(i, nStartX, nStartY, nRadius);
	}

	m_Canvas.m_nCircleCount = 3;

	// ****** 정원 위치 보정 ******
	m_Canvas.UpdateCircleCenter();

	m_Canvas.Update();
	DrawCircleInfo();
	Invalidate(FALSE);
}

BOOL CProject1Dlg::RandomMoveMode()
{
	BOOL bRet = FALSE;

	if (m_nCircleMode == 3 && m_isWorkingThread)
	{
		if (m_nRandomMoveCount < MAX_MOVE_COUNT)
		{
			DrawRandomMove();

			bRet = TRUE;
			m_nRandomMoveCount++;
			TRACE("%d\n", m_nRandomMoveCount);
		}

		if (m_nRandomMoveCount >= MAX_MOVE_COUNT)
		{
			m_nCircleMode = 0;
			m_nRandomMoveCount = 0;
			m_isWorkingThread = false;
			EnableButtons(TRUE);
		}
	}

	return bRet;
}

void CProject1Dlg::EnableButtons(BOOL bEnabled)
{
	m_RadiusEdit.EnableWindow(bEnabled);
	m_TicknessEdit.EnableWindow(bEnabled);

	m_InitButton.EnableWindow(bEnabled);
	m_TestButton.EnableWindow(bEnabled);
	m_RandomMoveButton.EnableWindow(bEnabled);
}

void CProject1Dlg::InitChildControls()
{
	int margin = 10;

	CRect dlgRect;
	this->GetClientRect(&dlgRect);

	// 3열
	CRect childRect;
	m_TestButton.GetWindowRect(&childRect);
	m_TestButton.ScreenToClient(&childRect);

	int left = dlgRect.Width() - childRect.Width() - margin;
	m_TestButton.MoveWindow(left, childRect.top+margin, childRect.Width(), childRect.Height());
	
	int top0 = childRect.top;
	int top = childRect.top + margin + +childRect.Height();

	int left0 = left;
	int left1 = left;

	m_InitButton.GetWindowRect(&childRect);
	m_InitButton.ScreenToClient(&childRect);

	left = dlgRect.Width() - childRect.Width() - margin;
	m_InitButton.MoveWindow(left, top + margin, childRect.Width(), childRect.Height());
	int top1 = top;
	top = top + margin + childRect.Height();
	

	m_RandomMoveButton.GetWindowRect(&childRect);
	m_RandomMoveButton.ScreenToClient(&childRect);

	left = dlgRect.Width() - childRect.Width() - margin;
	m_RandomMoveButton.MoveWindow(left, top + margin, childRect.Width(), childRect.Height());
	int top2 = top;

	// 2열
	m_RadiusEdit.GetWindowRect(&childRect);
	m_RadiusEdit.ScreenToClient(&childRect);

	left = left0 - childRect.Width() - margin;
	top = top0;
	m_RadiusEdit.MoveWindow(left, top + margin, childRect.Width(), childRect.Height());
	top = top + margin + childRect.Height();

	m_TicknessEdit.GetWindowRect(&childRect);
	m_TicknessEdit.ScreenToClient(&childRect);

	left = left0 - childRect.Width() - margin;
	m_TicknessEdit.MoveWindow(left, top + margin, childRect.Width(), childRect.Height());
	top = top + margin + childRect.Height();

	left0 = left;
	top = top0;

	// 1열
	CStatic* pStatic = (CStatic*)GetDlgItem(IDC_STATIC1);
	if (pStatic)
	{
		pStatic->GetWindowRect(&childRect);
		pStatic->ScreenToClient(&childRect);

		left = left0 - childRect.Width() - margin;
		top = top0;
		pStatic->MoveWindow(left, top + margin, childRect.Width(), childRect.Height());
	}

	pStatic = (CStatic*)GetDlgItem(IDC_STATIC2);
	if(pStatic)
	{
		pStatic->GetWindowRect(&childRect);
		pStatic->ScreenToClient(&childRect);

		left = left0 - childRect.Width() - margin;
		top = top1;
		pStatic->MoveWindow(left, top + margin, childRect.Width(), childRect.Height());
	}

	int width = left1 - left;
	int height = childRect.Height() * 4;

	left1 = left;
	{
		m_CircleInfo.GetWindowRect(&childRect);
		m_CircleInfo.ScreenToClient(&childRect);
		left = left1;
		top = top2;
		m_CircleInfo.MoveWindow(left, top + margin, width, height);

	}

}