#include "pch.h"
#include "CUIIconScript.h"


#include <Engine/CLevel.h>
#include <Engine/CLevelMgr.h>
#include <Engine/CKeyMgr.h>
// icon
// katana

CUIIconScript::CUIIconScript()
	:CScript(UIICONSCRIPT)
{
}

CUIIconScript::~CUIIconScript()
{
}


void CUIIconScript::begin()
{
	if (Animator2D() != nullptr)
	{
		Animator2D()->Play(L"icon", false);
	}

	if (nullptr != CLevelMgr::GetInst()->GetCurLevel()->FindObjectByName(L"UIkatana"))
	{
		m_pkatana = CLevelMgr::GetInst()->GetCurLevel()->FindObjectByName(L"UIkatana");
		m_pkatana->Animator2D()->Play(L"katana", false);
	}
}

void CUIIconScript::tick()
{
}


