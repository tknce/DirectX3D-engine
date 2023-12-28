#include "pch.h"
#include "CTimerScript.h"

#include <Engine/CLevel.h>
#include <Engine/CLevelMgr.h>
#include <Engine/CPlayerMgr.h>
#include <Engine/CEventMgr.h>

CTimerScript::CTimerScript()
	:CScript(TIMERSCRIPT)
	, m_fAcctime(0)
{
}

CTimerScript::~CTimerScript()
{
}

void CTimerScript::begin()
{
	if (Animator2D() != nullptr)
	{
		Animator2D()->Play(L"Timerbar",false);
	}
	if (nullptr != CLevelMgr::GetInst()->GetCurLevel()->FindObjectByName(L"UItimer"))
	{
		m_pTimer = CLevelMgr::GetInst()->GetCurLevel()->FindObjectByName(L"UItimer");
		if (m_pTimer->Animator2D() != nullptr)
		{
			m_pTimer->Animator2D()->Play(L"Timer", true);
		}
	}
	if (nullptr != CLevelMgr::GetInst()->GetCurLevel()->FindObjectByName(L"UIreduce"))
	{
		m_pTReduce = CLevelMgr::GetInst()->GetCurLevel()->FindObjectByName(L"UIreduce");
		m_pTReduce->Transform()->SetRelativePos(105.f, 414.f, -1.f);
		m_pTReduce->Transform()->SetRelativeScale(0.f, 20.f, 1.f);
	}
	
}

void CTimerScript::tick()
{
	m_fAcctime += DT;
	m_fAcctime_Reduce += DT;

	if (m_fAcctime > 3.f)
	{
		Animator2D()->Play(L"Timerbar", false);
		m_fAcctime = 0;
	}

	if (m_fAcctime_Reduce < 45.f)
	{
		if (m_pTReduce != nullptr)
		{
			Vec3 Pos = m_pTReduce->Transform()->GetRelativePos();
			Vec3 Scale = m_pTReduce->Transform()->GetRelativeScale();

			Pos.x -= (DT) *2;
			Scale.x += DT * 4;

			m_pTReduce->Transform()->SetRelativePos(Pos);
			m_pTReduce->Transform()->SetRelativeScale(Scale);
		}
		
	}
	else
	{
		Vec3 Pos = Vec3(102.921f, 414.f, -1);
		Vec3 Scale = Vec3(4.159f, 20.f, 1);
		m_pTReduce->Transform()->SetRelativePos(Pos);
		m_pTReduce->Transform()->SetRelativeScale(Scale);
		m_fAcctime_Reduce = 0;
		CPlayerMgr::GetInst()->SetDead(1);
	}
	if (CPlayerMgr::GetInst()->GetDead() == 2 || CPlayerMgr::GetInst()->GetDead() == 1)
	{
		Vec3 Pos = Vec3(102.921f, 414.f, -1);
		Vec3 Scale = Vec3(4.159f, 20.f, 1);
		m_pTReduce->Transform()->SetRelativePos(Pos);
		m_pTReduce->Transform()->SetRelativeScale(Scale);
		m_fAcctime_Reduce = 0;
	}
}


