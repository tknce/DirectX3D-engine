#include "pch.h"
#include "CUIcursorScript.h"

#include <Engine/CKeyMgr.h>

CUIcursorScript::CUIcursorScript()
	:CScript(UICURSORSCRIPT)
{
}

CUIcursorScript::~CUIcursorScript()
{
}



void CUIcursorScript::begin()
{
	if (Animator2D() != nullptr)
	{
		Animator2D()->Play(L"cursor", false);
	}
	
}

void CUIcursorScript::tick()
{
	SetCursor(NULL);
	Vec2 MousePos = CKeyMgr::GetInst()->GetMousePos();
	MousePos.x -= 800.f;
	MousePos.y -= 450.f;
	MousePos.y *= -1;
	Vec3 Pos = Vec3(MousePos.x, MousePos.y, -5);

	Transform()->SetRelativePos(Pos);
}



