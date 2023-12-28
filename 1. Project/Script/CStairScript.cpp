#include "pch.h"
#include "CStairScript.h"

#include "CPlayerScript.h"

CStairScript::CStairScript()
	:CScript(STAIRSCRIPT)
{
}

CStairScript::~CStairScript()
{
}


void CStairScript::tick()
{
}

void CStairScript::BeginOverlap(CCollider2D* _pOther)
{
	if (_pOther->GetOwner()->GetName() == L"Player")
	{
		Vec3 a = _pOther->GetOwner()->Transform()->GetRelativePos();
		a.y += 2;
		_pOther->GetOwner()->RigidBody2D()->SetStair(true);
		_pOther->GetOwner()->Transform()->SetRelativePos(a);
	}

}

void CStairScript::Overlap(CCollider2D* _pOther)
{
	if (_pOther->GetOwner()->GetName() == L"Player")
	{
		_pOther->GetOwner()->GetScript<CPlayerScript>()->SetStair(true);
		Vec3 a = _pOther->GetOwner()->Transform()->GetRelativePos();
		a.y += 2;
		_pOther->GetOwner()->Transform()->SetRelativePos(a);
	}
}

void CStairScript::EndOverlap(CCollider2D* _pOther)
{
	if (_pOther->GetOwner()->GetName() == L"Player")
	{
		_pOther->GetOwner()->RigidBody2D()->SetStair(false);
	}
}


