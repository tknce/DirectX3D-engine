#include "pch.h"
#include "CFactory_1Script.h"
#include <Engine/CSoundMgr.h>
#include <Engine/CLevelMgr.h>
#include <Engine/CEventMgr.h>
#include <Client/CSaveLoadMgr.h>

CFactory_1Script::CFactory_1Script()
	:CScript(FACTORY_1SCRIPT)
{
}

CFactory_1Script::~CFactory_1Script()
{
}

void CFactory_1Script::begin()
{
	CSoundMgr::GetInst()->FindSound(L"sound\\factory_1\\Factory.ogg", 1, 0.6f, true);
}

void CFactory_1Script::tick()
{
	if (KEY_TAP(KEY::R))
	{
		
		ChangeLevel(CSaveLoadMgr::GetInst()->LoadLevel(CLevelMgr::GetInst()->GetCurLevel()->GetName()));

		/*tEvent evn = {};
		evn.eType = EVENT_TYPE::CHANGE_LEVEL_STATE;
		evn.wParam = (DWORD_PTR)LEVEL_STATE::PLAY;
		CEventMgr::GetInst()->AddEvent(evn);*/
	}
}


