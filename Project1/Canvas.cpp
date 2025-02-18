#include "pch.h"
#include "Canvas.h"

CCanvas::CCanvas(int nStartX, int nStartY, int nRadius)
{
	m_nWidth = nRadius * 2;
	m_nHeight = nRadius * 2;
	m_nWidth = 640;
	m_nHeight = 480;
	m_nBpp = 8;
	//m_nStartX = nStartX;
	//m_nStartY = nStartY;
	//m_nRadius = nRadius;

	Create();
};

CCanvas::~CCanvas()
{
	Destroy();
}

void CCanvas::Create()
{
	m_Image.Create(m_nWidth, -m_nHeight, m_nBpp);
	if (m_nBpp == 8) {
		static RGBQUAD rgb[256];
		for (int i = 0; i < 256; i++)
			rgb[i].rgbRed = rgb[i].rgbGreen = rgb[i].rgbBlue = i;
		m_Image.SetColorTable(0, 256, rgb);
	}

	int nPitch = m_Image.GetPitch();
	unsigned char* fm = (unsigned char*)m_Image.GetBits();

	//memset(fm, 0x00, m_nWidth * m_nHeight);

	for (int j = 0; j < m_nHeight; j++) {
		for (int i = 0; i < m_nWidth; i++) {
			fm[j * nPitch + i] = 0x00;
		}
	}
}

void CCanvas::Destroy()
{
	m_Image.Destroy();
}

void CCanvas::Draw(HDC dc)
{
	m_Image.Draw(dc, m_nLeft, m_nTop);

#if 0
	CString strFile;
	strFile.Format(_TEXT("image%d.jpg"), ++m_nDrawCount);
	m_Image.Save(strFile);
#endif
}

void CCanvas::DrawCircle(unsigned char* fm, int x, int y, int nRadius, int nGray, bool bFill)
{
	int nCenterX = x + nRadius;
	int nCenterY = y + nRadius;
	int nPitch = m_Image.GetPitch();


	if (bFill)
	{
		for (int j = y; j < y + nRadius * 2; j++) {
			if (j < 0)
				continue;
			if (j >= m_nHeight)
				break;
			for (int i = x; i < x + nRadius * 2; i++) {
				if (i < 0)
					continue;
				if (i >= m_nWidth)
					break;
				if (isInCircle(i, j, nCenterX, nCenterY, nRadius))
					fm[j * nPitch + i] = nGray;
			}
		}
	}
	else
	{
		// 작은 원 이동시 정원 그리는 알고리즘을 변경할 필요가 있다.
		
		// 둘레만 칠한다.
		for (int j = y; j < y + nRadius * 2; j++) {
			if (j < 0)
				continue;
			if (j >= m_nHeight)
				break;
			for (int i = x; i < x + nRadius * 2; i++) {
				if (i < 0)
					continue;
				if (i >= m_nWidth)
					break;
				if (isOutCircle(i, j, nCenterX, nCenterY, nRadius))
					fm[j * nPitch + i] = nGray;
			}
		}
	}
}

bool CCanvas::isInCircle(int i, int j, int nCenterX, int nCenterY, int nRadius)
{
	bool bRet = false;

	double dX = i - nCenterX;
	double dY = j - nCenterY;
	double dDist = dX * dX + dY * dY;

	if (dDist < nRadius * nRadius) {
		bRet = true;
	}

	return bRet;
}

bool CCanvas::isOutCircle(int i, int j, int nCenterX, int nCenterY, int nRadius)
{
	bool bRet = false;
	//double dWidth = pow(i - nCenterX, 2);
	//double dHeight = pow(j - nCenterY, 2);
	double dWidth = pow(nCenterX - i, 2);
	double dHeight = pow(nCenterY - j, 2);
	double dDist = sqrt(dWidth + dHeight);

	if ((nRadius - m_nTickness <= dDist) && (dDist < nRadius)) {
	//if (((nRadius - (m_nTickness/2)) <= dDist) && (dDist <= nRadius+ (m_nTickness / 2))) { // 가장자리 두께는 사용자로부터 입력 받습니다. m_nTickness 활용 두께 계산 어찌하지 -_-;

		bRet = true;
	}

	return bRet;
}

void CCanvas::Clear()
{
	m_nCircleIndex = 0;
	m_nCircleCount = 0;
	m_nTickness = 0;

	for (int i = 0; i < MAX_CIRCLE; i++)
	{
		m_Circle[i].Clear();
	}

	unsigned char* fm = (unsigned char*)m_Image.GetBits();
	memset(fm, 0x00, m_nWidth * m_nHeight);
}

void CCanvas::CreateCircle(int nStartX, int nStartY, int nRadius)
{
	if (m_nCircleCount >= 3)
	{
		// 정원 그리기
		return;
	}

	m_nCircleIndex = 1 + m_nCircleCount;
	m_Circle[m_nCircleIndex].m_nStartX = nStartX;
	m_Circle[m_nCircleIndex].m_nStartY = nStartY;
	m_Circle[m_nCircleIndex].m_nRadius = nRadius;
	m_Circle[m_nCircleIndex].m_bVisible = true;
	m_Circle[m_nCircleIndex].m_bFill = true;

	m_nCircleCount++;

	/*
	if (m_nCircleCount >= 3)
	{
		// 정원 그리기
		// 좌표 계산해야 함. 복잡함
		m_Circle[0].m_nStartX = 50;
		m_Circle[0].m_nStartY = 50;
		m_Circle[0].m_nRadius = 100;
		m_Circle[0].m_bVisible = true;
		m_Circle[0].m_bFill = false;
	}
	*/
}

bool CCanvas::UpdateCircleCenter()
{
	if (m_nCircleCount < 3) return false;

	// https://minstudyroom.tistory.com/4

	// 중점.
	int x1 = m_Circle[1].m_nStartX + m_Circle[1].m_nRadius;
	int y1 = m_Circle[1].m_nStartY + m_Circle[1].m_nRadius;

	int x2 = m_Circle[2].m_nStartX + m_Circle[2].m_nRadius;
	int y2 = m_Circle[2].m_nStartY + m_Circle[2].m_nRadius;

	int x3 = m_Circle[3].m_nStartX + m_Circle[3].m_nRadius;
	int y3 = m_Circle[3].m_nStartY + m_Circle[3].m_nRadius;

	TRACE(TEXT("P1(%d, %d)\n"), x1, y1);
	TRACE(TEXT("P2(%d, %d)\n"), x2, y2);
	TRACE(TEXT("P3(%d, %d)\n"), x3, y3);

	m_Circle[0].m_bVisible = false;

	// http://www.borlandforum.com/impboard/impboard.dll?action=read&db=bcb_tip&no=922
	double px1 = (x1 + x2) / 2;
	double py1 = (y1 + y2) / 2;
	double px2 = (x1 + x3) / 2;
	double py2 = (y1 + y3) / 2;
	double dy1 = y1 - y2;
	double dy2 = y1 - y3;
	double r = 0;
	double d = 0;
	double d2 = 0;
	double yi;
	double cx;
	double cy;

	if (dy1 != 0) {
		d = (x2 - x1) / dy1;
		yi = py1 - d * px1;
		if (dy2 != 0) {
			d2 = (x3 - x1) / dy2;
			if (d != d2) cx = (yi - (py2 - d2 * px2)) / (d2 - d);
			else return false;
		}
		else if (x3 - x1 == 0) return false;
		else cx = px2;
	}
	else if (dy2 != 0 && x2  - x1 != 0) {
		d = (x2 - x1) / dy2;
		yi = py2 - d * px2;
		cx = px1;
	}
	else return false;
	cy = d * cx + yi;
	r = sqrt((x1 - cx) * (x1- cx) + (y1- cy) * (y1 - cy));

	TRACE(TEXT("cx: %g\n"), cx);
	TRACE(TEXT("cy: %g\n"), cy);
	TRACE(TEXT("d: %g\n"), d);
	
	m_Circle[0].m_nStartX = cx - r;
	m_Circle[0].m_nStartY = cy - r;
	m_Circle[0].m_nRadius = r;
	m_Circle[0].m_bVisible = true;
	m_Circle[0].m_bFill = false;

	TRACE(TEXT("(%d, %d) R: %d\n"), m_Circle[0].m_nStartX, m_Circle[0].m_nStartY, m_Circle[0].m_nRadius);

	return true;
}

void CCanvas::UpdateCircle(int nIndex, int nStartX, int nStartY, int nRadius)
{
	if (nIndex < 1 || 3 < nIndex)
		return;

	m_Circle[nIndex].m_nStartX = nStartX;
	m_Circle[nIndex].m_nStartY = nStartY;
	m_Circle[nIndex].m_nRadius = nRadius;
	m_Circle[nIndex].m_bVisible = true;
	m_Circle[nIndex].m_bFill = true;
}


int CCanvas::FindCircle(CPoint point)
{
	for (int i = 1; i < MAX_CIRCLE; i++)
	{
		if (m_Circle[i].m_bVisible && m_Circle[i].PtInRect(point))
		{
			return i;
		}
	}

	return -1;
}

void CCanvas::Update()
{
	int nGray = 80;
	int nWidth = m_Image.GetWidth();
	int nHeight = m_Image.GetHeight();
	int nPitch = m_Image.GetPitch();
	//int nRadius = 20;
	unsigned char* fm = (unsigned char*)m_Image.GetBits();

	memset(fm, 0x00, m_nWidth * m_nHeight);

	for (int i = 0; i < MAX_CIRCLE; i++)
	{
		if (m_Circle[i].m_bVisible)
			DrawCircle(fm, m_Circle[i].m_nStartX, m_Circle[i].m_nStartY, m_Circle[i].m_nRadius, nGray, m_Circle[i].m_bFill);
	}
}

void CCanvas::UpdateBigCircle()
{
	//if (m_nCircleCount >= 3)
	{
		// 정원 그리기
		// 좌표 계산해야 함. 복잡함
		m_Circle[0].m_nStartX = 50;
		m_Circle[0].m_nStartY = 50;
		m_Circle[0].m_nRadius = 100;
		m_Circle[0].m_bVisible = true;
		m_Circle[0].m_bFill = false;
	}

	// 1. 중점을 구한다. 제일 간단함.
	// 세점과 중점까지의 거리는 동일하다. 
	// 세점의 무게 중심 계산해본다? 삼각형이 되어버림.
	//
	// 모든 3점은 원에 속해야 하므로 원주율, 각도을 계산해본다?
	//  두 점 간의 아크 좌표
	// 2. 정교한 알고리즘을 구현한다. (이게 맞는듯)
}