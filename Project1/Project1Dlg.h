
// Project1Dlg.h: 헤더 파일
//

#pragma once

#include "Canvas.h"

// CProject1Dlg 대화 상자
class CProject1Dlg : public CDialogEx
{
// 생성입니다.
public:
	CProject1Dlg(CWnd* pParent = nullptr);	// 표준 생성자입니다.

// 대화 상자 데이터입니다.
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_PROJECT1_DIALOG };
#endif

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV 지원입니다.


// 구현입니다.
protected:
	HICON m_hIcon;

	// 생성된 메시지 맵 함수
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	afx_msg void OnShowWindow(BOOL, UINT);
	DECLARE_MESSAGE_MAP()

	int GetTickness();
	int GetRadius();
	void DrawCircleInfo();
	void DrawRandomMove();
	
	void EnableButtons(BOOL bEnabled);
	void InitChildControls();

	int m_nCircleMode = 0; // 1:INSERT, 2:EDIT, 3:RANDOM MOVE
	int m_nCircleIndex = -1;
	int m_nRandomMoveCount = 0;
	CPoint m_CurPos;
	CWinThread* m_pThread = nullptr;
	CCanvas m_Canvas;
	int const MAX_MOVE_COUNT = 10;

public:
	bool m_isWorkingThread = false;
	afx_msg void OnBnClickedButton1();
	afx_msg void OnBnClickedButton2();
	CEdit m_RadiusEdit;
	CEdit m_TicknessEdit;
	CStatic m_CircleInfo;
	afx_msg void OnBnClickedButton3();
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnClose();
	afx_msg void OnTimer(UINT_PTR nIDEvent);
	CButton m_TestButton;
	CButton m_InitButton;
	CButton m_RandomMoveButton;
	BOOL RandomMoveMode();
};
