#include "pch.h"
#include "CMonsterScript.h"

#include <Engine\CGameObject.h>
#include <Engine/CComponent.h>
#include <Engine/CLevelMgr.h>
#include <Engine/CLevel.h>
#include <Engine/CAI.h>

CMonsterScript::CMonsterScript()
	: CScript(MONSTERSCRIPT)
	, m_pTargetObject(nullptr)
	, m_bAni(true)
{
	m_AniPlay.resize((UINT)MONSTER_STATE_FIGHT::END);
	m_AniPlay[(UINT)MONSTER_STATE_FIGHT::IDLE] = L"IDLE";
	m_AniPlay[(UINT)MONSTER_STATE_FIGHT::WALK] = L"WALK";
	m_AniPlay[(UINT)MONSTER_STATE_FIGHT::WALK_REVERSE] = L"WALK";
	m_AniPlay[(UINT)MONSTER_STATE_FIGHT::TURN] = L"TURN";
	m_AniPlay[(UINT)MONSTER_STATE_FIGHT::TURN_REVERSE] = L"TURN";
	m_AniPlay[(UINT)MONSTER_STATE_FIGHT::RUN] = L"RUN";
	m_AniPlay[(UINT)MONSTER_STATE_FIGHT::RUN_REVERSE] = L"RUN";
	m_AniPlay[(UINT)MONSTER_STATE_FIGHT::HURTFLY] = L"HURT_FLY";
	m_AniPlay[(UINT)MONSTER_STATE_FIGHT::HURTFLY_REVERSE] = L"HURT_FLY";
	m_AniPlay[(UINT)MONSTER_STATE_FIGHT::HURTGROUND] = L"HURT_GROUND";
	m_AniPlay[(UINT)MONSTER_STATE_FIGHT::HURTGROUND_REVERSE] = L"HURT_GROUND";
	m_AniPlay[(UINT)MONSTER_STATE_FIGHT::ATTACK_ALMOST] = L"ATTACK_ALMOST";


}

CMonsterScript::~CMonsterScript()
{
}

void CMonsterScript::begin()
{
	CLevel* CurLevel = CLevelMgr::GetInst()->GetCurLevel();
	m_pTargetObject = CurLevel->FindObjectByName(L"Player");
}

void CMonsterScript::tick()
{	
	DetectRange();
	Animator2D()->Play(L"AIM", true, true);

	CGameObject* Child = GetOwner()->GetChildwstring(L"Gun");
	CGameObject* RightArm = GetOwner()->GetChildwstring(L"RightArm");
	CGameObject* LeftArm = GetOwner()->GetChildwstring(L"LeftArm");
	if (Child != nullptr)
	{
		//RightArm->Animator2D()->Play(L"RIGHTARM", true);
		
		Child->Animator2D()->Play(L"Gun", true, true);
		LeftArm->Animator2D()->Play(L"LEFTARM", true);
	}
	
	if (KEY_TAP(KEY::LBTN))
	{
		m_vDir = Transform()->GetRelativePos() - m_pTargetObject->Transform()->GetRelativePos();
		m_vDir.Normalize();
	}

	return;
	if (m_bAni)
	{
		RigidBody2D()->SetGround(false);
		Animator2D()->Play(m_AniPlay[(UINT)MONSTER_STATE_FIGHT::IDLE], true);
		m_bAni = false;
	}
}

void CMonsterScript::DetectRange()
{
	m_fDistance = (Transform()->GetRelativePos() - m_pTargetObject->Transform()->GetRelativePos()).Length();
	if (m_fDistance < 500)
	{
		Transform()->SetRelativePos(Transform()->GetRelativePos() - m_vDir);
		int i = 0;
	}
}

void CMonsterScript::BeginOverlap(CCollider2D* _pOther)
{
	RigidBody2D()->SetGround(true);
	//m_bIsGround = true;
	Vec3 vPos = Transform()->GetRelativePos();
	vPos.y += 1;
	Transform()->SetRelativePos(vPos);

	if (_pOther->GetOwner()->GetName() == L"Player")
	{
		m_pTargetObject = _pOther->GetOwner();
		Animator2D()->Play(m_AniPlay[(UINT)MONSTER_STATE_FIGHT::HURTGROUND],false);
	}
}

void CMonsterScript::Overlap(CCollider2D* _pOther)
{
}

void CMonsterScript::EndOverlap(CCollider2D* _pOther)
{
}
