#include "pch.h"
#include "CClear_1Script.h"

#include <Client/CSaveLoadMgr.h>
#include <Engine/CPlayerMgr.h>
#include <Engine/CKeyMgr.h>



CClear_1Script::CClear_1Script()
	:CScript(CLEAR_1SCRIPT)
	, nextstap(false)
{
}

CClear_1Script::~CClear_1Script()
{
}


void CClear_1Script::begin()
{
}

void CClear_1Script::tick()
{
	if(nextstap)
		if (KEY_TAP(KEY::P))
		{
			ChangeLevel(CSaveLoadMgr::GetInst()->LoadLevel(L"level\\factory_2.lv"));
		}
}

void CClear_1Script::BeginOverlap(CCollider2D* _pOther)
{
	if (!nextstap)
	{
		CPlayerMgr::GetInst()->SetDead(2);
		nextstap = true;
	}
	
	
}

void CClear_1Script::Overlap(CCollider2D* _pOther)
{
}

void CClear_1Script::EndOverlap(CCollider2D* _pOther)
{
}


