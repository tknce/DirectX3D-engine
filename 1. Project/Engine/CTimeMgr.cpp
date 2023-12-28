#include "pch.h"
#include "CTimeMgr.h"
#include "CKeyMgr.h"
#include "CEngine.h"
#include "CFontMgr.h"



CTimeMgr::CTimeMgr()
	: m_llFrequence{}
	, m_llCurCount{}
	, m_llPrevCount{}
	, m_fAccTime(0.f)
	, m_fDeltaTime(0.f)
{

}

CTimeMgr::~CTimeMgr()
{

}

void CTimeMgr::init()
{
	// �ʴ� ������ 1000
	// GetTickCount();

	// �ʴ� ������ õ��
	QueryPerformanceFrequency(&m_llFrequence);

	// ���� ī����
	QueryPerformanceCounter(&m_llPrevCount);
}

void CTimeMgr::tick()
{
	QueryPerformanceCounter(&m_llCurCount);

	m_fDeltaTime = (float)(m_llCurCount.QuadPart - m_llPrevCount.QuadPart) / (float)m_llFrequence.QuadPart;

	m_llPrevCount.QuadPart = m_llCurCount.QuadPart;

	// 60 �������� �ʰ��� ��� ���� ���
	if ((1.f / 60.f) < m_fDeltaTime)
	{
		m_fDeltaTime = (1.f / 60.f);
	}
	if (KEY_PRESSED(KEY::LSHIFT))
	{
		float a = 0.25;
		m_fDeltaTime *= a;
	}

	g_global.fDT = m_fDeltaTime;
	g_global.fAccTime += m_fDeltaTime;
}

void CTimeMgr::render()
{
	static int iCount = 0;
	++iCount;

	// 1 �ʿ� �ѹ�
	m_fAccTime += m_fDeltaTime;
	static wchar_t szFloat[50] = {};
	if (1.f < m_fAccTime)
	{
		HWND hWnd = CEngine::GetInst()->GetMainHwnd();

		
		float FPS = 1.f / m_fDeltaTime;
		swprintf_s(szFloat, 50, L"DeltaTime : %d", iCount);
		int iLen = (int)wcsnlen_s(szFloat, 50);
		//TextOut(_dc, 10, 10, szFloat, iLen);

		SetWindowText(hWnd, szFloat);

		// �����ð�, ī��Ʈ �ʱ�ȭ
		m_fAccTime = 0.f;
		iCount = 0;
	}


	//CFontMgr::GetInst()->DrawFont(szFloat, 10.f, 30.f, 20, FONT_RGBA(255, 20, 20, 127));
	/*wstring Start = L"START";
	CFontMgr::GetInst()->DrawFont(Start.c_str(), 10.f, 60.f, 100, FONT_RGBA(255, 255, 255, 255));*/
}

int CTimeMgr::GetRandem()
{
	float deltatime = m_fDeltaTime;
	deltatime *= 100000000;
	srand((UINT)deltatime);
	int randem = rand();

	return randem;
}