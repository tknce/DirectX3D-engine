#include "pch.h"
#include "CLighterScript.h"

CLighterScript::CLighterScript()
	:CScript(LIGHTERSCRIPT)
{
}

CLighterScript::~CLighterScript()
{
}

void CLighterScript::begin()
{
	if (Animator2D() != nullptr)
	{
		Animator2D()->Play(L"lighter_start", true);
	}
}

void CLighterScript::tick()
{
	SetCursor(NULL);
	Vec2 MousePos = CKeyMgr::GetInst()->GetMousePos();
	MousePos.x -= 800.f;
	MousePos.y -= 450.f;
	MousePos.y *= -1;
	Vec3 Pos = Vec3(MousePos.x, MousePos.y, -5);

	Transform()->SetRelativePos(Pos);

}

void CLighterScript::BeginOverlap(CCollider2D* _pOther)
{
}

void CLighterScript::Overlap(CCollider2D* _pOther)
{
}

void CLighterScript::EndOverlap(CCollider2D* _pOther)
{
}



