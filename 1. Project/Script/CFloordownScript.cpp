#include "pch.h"
#include "CFloordownScript.h"

#include <Engine/CKeyMgr.h>
#include <Engine/CSoundMgr.h>

CFloordownScript::CFloordownScript()
	:CScript(FLOORDOWNSCRIPT)
{
}

CFloordownScript::~CFloordownScript()
{
}


void CFloordownScript::tick()
{
}

void CFloordownScript::BeginOverlap(CCollider2D* _pOther)
{
	if (_pOther->GetOwner()->GetName() == L"Player")
	{
		_pOther->GetOwner()->RigidBody2D()->SetStair(false);
		if (_pOther->GetOwner()->Transform()->GetRelativePos().y > Transform()->GetRelativePos().y)
		{
			CSoundMgr::GetInst()->FindSound(L"sound\\player\\player_wallkick.wav", 1, 0.6f);
			_pOther->GetOwner()->RigidBody2D()->SetGround(true);
		}
		else
		{
			Vec3 Pos = _pOther->GetOwner()->Transform()->GetRelativePos();
			_pOther->GetOwner()->RigidBody2D()->SetVelocity(Vec2(_pOther->GetOwner()->RigidBody2D()->GetVelocity().x, 0.f));
			Pos.y -= 1;
			_pOther->GetOwner()->Transform()->SetRelativePos(Pos);
		}
	}
}

void CFloordownScript::Overlap(CCollider2D* _pOther)
{
	if (_pOther->GetOwner()->GetName() == L"Player")
	{
		
		if (KEY_TAP(KEY::S))
		{
			_pOther->GetOwner()->RigidBody2D()->SetGround(false);
		}

		
	}
}

void CFloordownScript::EndOverlap(CCollider2D* _pOther)
{
	if (_pOther->GetOwner()->GetName() == L"Player")
	{
		Vec3 Pos = _pOther->GetOwner()->Transform()->GetRelativePos();
		//_pOther->GetOwner()->RigidBody2D()->SetVelocity(Vec2(_pOther->GetOwner()->RigidBody2D()->GetVelocity().x, 0.f));
		Pos.y += 1;
		_pOther->GetOwner()->Transform()->SetRelativePos(Pos);
		_pOther->GetOwner()->RigidBody2D()->SetGround(false);
	}
}



