#include "pch.h"
#include "CNongrabwallScript.h"

CNongrabwallScript::CNongrabwallScript()
	:CScript(NONGRABWALLSCRIPT)
{
}

CNongrabwallScript::~CNongrabwallScript()
{
}

void CNongrabwallScript::tick()
{
}

void CNongrabwallScript::BeginOverlap(CCollider2D* _pOther)
{
	if (_pOther->GetOwner()->GetName() == L"Player")
	{
		Vec3 Pos = _pOther->GetOwner()->Transform()->GetRelativePos();
		if (Transform()->GetRelativePos().x < Pos.x)
		{
			Pos.x += 2;
		}
		else
		{
			Pos.x -= 2;
		}
		_pOther->GetOwner()->Transform()->SetRelativePos(Pos);
	}
}

void CNongrabwallScript::Overlap(CCollider2D* _pOther)
{
	if (_pOther->GetOwner()->GetName() == L"Player")
	{
		Vec3 Pos = _pOther->GetOwner()->Transform()->GetRelativePos();
		if (Transform()->GetRelativePos().x < Pos.x)
		{
			Pos.x += 2;
		}
		else
		{
			Pos.x -= 2;
		}
		_pOther->GetOwner()->Transform()->SetRelativePos(Pos);
	}
}

void CNongrabwallScript::EndOverlap(CCollider2D* _pOther)
{
}
