#include "pch.h"
#include "CDeadScript.h"

#include <Engine/CFontMgr.h>
#include <Engine/CPlayerMgr.h>

#include <Client/CSaveLoadMgr.h>

CDeadScript::CDeadScript()
	:CScript(DEADSCRIPT)
{
}

CDeadScript::~CDeadScript()
{
}
void CDeadScript::begin()
{
	//Animator2D()->Play(L"Menu", true);
}

void CDeadScript::tick()
{
	if (CPlayerMgr::GetInst()->GetDead() == 1)
	{
		Transform()->SetRelativePos(0.f, 0.f, 0.f);
		Transform()->SetRelativeScale(800.f, 400.f, 0.f);
		Animator2D()->Play(L"Menu", true);
		m_fAcctime += DT;
		char a;
		if (m_fAcctime > 0.05f)
		{
			a = 50;
			if (m_fAcctime > 0.07f)
				m_fAcctime = 0;

		}
		else
		{
			a = 255;
		}
		CFontMgr::GetInst()->SetDrawFont(L"            아니...", 650.f, 400.f, 10, FONT_RGBA(50, 50, 255, a));
		CFontMgr::GetInst()->SetDrawFont(L"  통하지 않을 거야.", 650.f, 430.f, 10, FONT_RGBA(50, 50, 255, a));
		CFontMgr::GetInst()->SetDrawFont(L"   (왼쪽 클릭으로 재시작)", 600.f, 490.f, 10, FONT_RGBA(50, 50, 255, a));
	}
	else if (CPlayerMgr::GetInst()->GetDead() == 2)
	{
		Transform()->SetRelativePos(0.f, 0.f, 0.f);
		Transform()->SetRelativeScale(3000.f, 1500.f, 0.f);
		Animator2D()->Play(L"Menu", true);
		m_fAcctime += DT;
		char a;
		if (m_fAcctime > 0.05f)
		{
			a = 50;
			if (m_fAcctime > 0.07f)
				m_fAcctime = 0;

		}
		else
		{
			a = 255;
		}
		TIMESTATE b = CSaveLoadMgr::GetInst()->GetTimestate();
		
		if (!b.play)
		{
			CFontMgr::GetInst()->SetDrawFont(L"그래, 이렇게 하면 되겠지.", 650.f, 400.f, 10, FONT_RGBA(255, 255, 255, a));
		}
		else
		{
			CFontMgr::GetInst()->SetDrawFont(L"재생", 150.f, 50.f, 30, FONT_RGBA(255, 255, 255, 210));
			CFontMgr::GetInst()->SetDrawFont(L"버튼을 눌러 나가기", 1300.f, 850.f, 10, FONT_RGBA(255, 255, 255, 210));
		}
	}

	else
	{
		Transform()->SetRelativePos(0.f, 1111111.f, 0.f);
	}
}



