#pragma once

#include <atlImage.h>

class CCircle
{
public:
	CCircle() {};
	virtual ~CCircle() {};
	void Clear()
	{
		m_nStartX = 0;
		m_nStartY = 0;
		m_nRadius = 0;
		//m_nCenterX = 0;
		//m_nCenterY = 0;
		m_bVisible = false;
		m_bFill = false;
	}

	int GetCenterX() const {
		return m_nStartX + m_nRadius;
	}

	int GetCenterY() const {
		return m_nStartY + m_nRadius;
	}

	bool PtInRect(CPoint point)
	{
		CRect rect(m_nStartX, m_nStartY, m_nStartX + (m_nRadius * 2), m_nStartY + (m_nRadius*2)); // 시작점 (0, 0)

		if (rect.left <= point.x && point.x <= rect.right &&
			rect.top <= point.y && point.y && point.y <= rect.bottom) // margin ??
		{
			return true;
		}

		return false;
	}

public:
	int m_nStartX = 0;
	int m_nStartY = 0;
	int m_nRadius = 0;
	//int m_nCenterX = 0;
	//int m_nCenterY = 0;
	bool m_bVisible = false;
	bool m_bFill = true;
};

class CCanvas
{
public:
	CCanvas(int nWidth=640, int nHeight=480, int nBpp=8);
	~CCanvas();

	void CreateCircle(int nStartX, int nStartY, int nRadius=20);
	void UpdateCircle(int nIndex, int nStartX, int nStartY, int nRadius = 20);
	void UpdateBigCircle();
	bool UpdateCircleCenter();
	int FindCircle(CPoint point);
	void Draw(HDC dc);

	int GetCenterX() const {
		return m_Circle[0].GetCenterX();
	}

	int GetCenterY() const {
		return m_Circle[0].GetCenterY();
	}

	void Clear();
	void Update();

	CRect GetRect() const {
		return CRect(m_nLeft, m_nTop, m_nWidth, m_nHeight);
	}

protected:
	void Create();
	void Destroy();
	void DrawCircle(unsigned char* fm, int x, int y, int nRadius, int nGray, bool bFill);
	bool isInCircle(int i, int j, int nCenterX, int nCenterY, int nRadius);
	bool isOutCircle(int i, int j, int nCenterX, int nCenterY, int nRadius);
	bool PtInRect(CRect rect, int x, int y);
	

public:
	const int MAX_CIRCLE = 4;
	int m_nLeft = 0;
	int m_nTop = 0;
	int m_nWidth = 640;
	int m_nHeight = 480;
	int m_nBpp = 8;
	CImage m_Image; // 배경
	CCircle m_Circle[4]; // 0=정원, 1,2,3=작은원
	int m_nCircleIndex = 0;
	int m_nCircleCount = 0; // 정원.
	bool m_nDrawCircle = false;
	int m_nTickness = 10;
	int m_nDrawCount = 0;

};

