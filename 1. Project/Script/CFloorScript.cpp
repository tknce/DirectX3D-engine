#include "pch.h"
#include "CFloorScript.h"

#include "CPlayerScript.h"
#include <Engine/CSoundMgr.h>

CFloorScript::CFloorScript()
	:CScript(FLOORSCRIPT)
{
}

CFloorScript::~CFloorScript()
{
}

void CFloorScript::tick()
{
	
}

void CFloorScript::BeginOverlap(CCollider2D* _pOther)
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

void CFloorScript::Overlap(CCollider2D* _pOther)
{
	if (_pOther->GetOwner()->GetName() == L"Player")
	{
		_pOther->GetOwner()->GetScript<CPlayerScript>()->SetStair(false);
		if (KEY_TAP(KEY::LBTN))
		{
			_pOther->GetOwner()->RigidBody2D()->SetGround(false);
		}
		else
		{
			_pOther->GetOwner()->RigidBody2D()->SetGround(true);
		}
	}
	if (_pOther->GetOwner()->GetLayerIdx() == 3)
	{
		Vec3 Pos = _pOther->GetOwner()->Transform()->GetRelativePos();
		Pos.y += 1;
		_pOther->GetOwner()->Transform()->SetRelativePos(Pos);
	}
}

void CFloorScript::EndOverlap(CCollider2D* _pOther)
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