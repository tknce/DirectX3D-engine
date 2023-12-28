#include "pch.h"
#include "CDoorScript.h"

#include <Engine/CAnimation2D.h>
#include <Engine/CSoundMgr.h>

CDoorScript::CDoorScript()
	:CScript(DOORSCRIPT)
	, m_bDoor(true)
{
}

CDoorScript::~CDoorScript()
{
}


void CDoorScript::begin()
{
	m_pAni = Animator2D()->Play(L"DOOR", false);
}

void CDoorScript::tick()
{
	if(m_bDoor)
	m_pAni = Animator2D()->Play(L"DOOR", false);
	else
	{
		if(m_pAni->GetFinish())
		Animator2D()->Play(L"DOOR_OPEN", false);
	}
}

void CDoorScript::BeginOverlap(CCollider2D* _pOther)
{
	if (_pOther->GetOwner()->GetName() == L"SWORD")
	{
		if(m_bDoor)
		CSoundMgr::GetInst()->FindSound(L"sound\\dooropen.wav",1,0.5f,true);
		m_bDoor = false;
	}
		

	if (_pOther->GetOwner()->GetName() == L"Player")
	{
		if (m_bDoor)
		{
			_pOther->GetOwner()->RigidBody2D()->SetWall_namal(true);
			Vec3 Pos = _pOther->GetOwner()->Transform()->GetRelativePos();
			if (Transform()->GetRelativePos().x < Pos.x)
			{
				Pos.x += 1;
			}
			else
			{
				Pos.x -= 1;
			}
			_pOther->GetOwner()->Transform()->SetRelativePos(Pos);
		}
		
	}
}

void CDoorScript::Overlap(CCollider2D* _pOther)
{
	if (_pOther->GetOwner()->GetName() == L"Player")
	{
		if (m_bDoor)
		{
			_pOther->GetOwner()->RigidBody2D()->SetWall_namal(true);
			Vec3 Pos = _pOther->GetOwner()->Transform()->GetRelativePos();
			if (Transform()->GetRelativePos().x < Pos.x)
			{
				Pos.x += 1;
			}
			else
			{
				Pos.x -= 1;
			}
			_pOther->GetOwner()->Transform()->SetRelativePos(Pos);
		}
	}
}

void CDoorScript::EndOverlap(CCollider2D* _pOther)
{
	if (_pOther->GetOwner()->GetName() == L"Player")
	{
		if (m_bDoor)
		{
			_pOther->GetOwner()->RigidBody2D()->SetWall_namal(false);
		}
	}
}



