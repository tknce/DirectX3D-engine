#include "pch.h"
#include "CLastScript.h"

#include <Engine/CPlayerMgr.h>

void CLastScript::begin()
{

}

void CLastScript::tick()
{
	if (nextstap)
		if (KEY_TAP(KEY::P))
		{
			ChangeLevel(CSaveLoadMgr::GetInst()->LoadLevel(L"level\\Test1.lv"));
		}
}

void CLastScript::BeginOverlap(CCollider2D* _pOther)
{
	if (!nextstap)
	{
		CPlayerMgr::GetInst()->SetDead(2);
		nextstap = true;
	}
}

void CLastScript::Overlap(CCollider2D* _pOther)
{
}

void CLastScript::EndOverlap(CCollider2D* _pOther)
{
}



CLastScript::CLastScript()
	:CScript(LASTSCRIPT)
{
}

CLastScript::~CLastScript()
{
}
