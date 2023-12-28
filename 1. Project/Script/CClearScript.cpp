#include "pch.h"
#include "CClearScript.h"

#include <Client/CSaveLoadMgr.h>
#include <Engine/CPlayerMgr.h>

CClearScript::CClearScript()
	:CScript(CLEARSCRIPT)
{
}

CClearScript::~CClearScript()
{
}

void CClearScript::begin()
{
}

void CClearScript::tick()
{
	if(nextstap)
		if (KEY_TAP(KEY::P))
		{
			ChangeLevel(CSaveLoadMgr::GetInst()->LoadLevel(L"level\\factory_3.lv"));
		}
}

void CClearScript::BeginOverlap(CCollider2D* _pOther)
{
	CPlayerMgr::GetInst()->SetDead(2);
	nextstap = true;
	
}

void CClearScript::Overlap(CCollider2D* _pOther)
{
}

void CClearScript::EndOverlap(CCollider2D* _pOther)
{
}


