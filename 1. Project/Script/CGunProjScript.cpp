#include "pch.h"
#include "CGunProjScript.h"

#include <Engine/CLevelMgr.h>
#include <Engine/CLevel.h>
#include <Engine/CSoundMgr.h>
#include <Engine/CPlayerMgr.h>

CGunProjScript::CGunProjScript()
	: CScript(GUNPROJSCRIPT)
	, m_TargetObj(nullptr)
	, m_vDir()
	, m_bSpark(false)
{
}

CGunProjScript::~CGunProjScript()
{
}

void CGunProjScript::begin()
{
	CLevel* CurLevel = CLevelMgr::GetInst()->GetCurLevel();
	m_TargetObj = CurLevel->FindObjectByName(L"Player");
	m_vDir = Transform()->GetRelativePos() - m_TargetObj->Transform()->GetRelativePos();
	m_vDir.Normalize();
	Vec3 vRot = Transform()->GetRelativeRotation();
	Vec3 Pos = Transform()->GetRelativePos();
	Vec3 TargetPos = m_TargetObj->Transform()->GetRelativePos();
	float height;
	float bottom;
	float YRot = 0;
	if (TargetPos.x > Pos.x)
	{
		height = TargetPos.y - Pos.y;
		bottom = TargetPos.x - Pos.x;
	}
	else
	{
		height = Pos.y - TargetPos.y;
		bottom = Pos.x - TargetPos.x;
		YRot = XM_PI;
	}

	float Z = atan(height / bottom);
	Transform()->SetRelativeRotation(0, YRot, Z);
	Collider2D()->SetRotationZ(Z);
}

void CGunProjScript::tick()
{
	Transform()->SetRelativePos(Transform()->GetRelativePos() - (m_vDir * 2000) * DT  );


	Animator2D()->Play_Rotation(L"BULLET", true);
}

void CGunProjScript::BeginOverlap(CCollider2D* _pOther)
{
	if (_pOther->GetOwner()->GetName() == L"SWORD")
	{
		CSoundMgr::GetInst()->FindSound(L"sound\\player_slash_bulley.wav", 1, 0.6f, true);
		m_vDir =  m_TargetObj->Transform()->GetRelativePos() - Transform()->GetRelativePos();
		m_vDir.Normalize();

		/*CGameObject* SPARK = CLevelMgr::GetInst()->FindObjectByName(L"PLAYERSPARK");
		SPARK->Transform()->SetRelativePos(GetOwner()->Transform()->GetRelativePos());
		SPARK->Animator2D()->Play(L"PADDING", false);*/
		return;
	}


		if (_pOther->GetOwner()->GetName() == L"Player")
		{
			CSoundMgr::GetInst()->FindSound(L"sound\\enemy_punch_hit.wav", 1, 0.6f, true);
			
				_pOther->GetOwner()->GetScript<CPlayerScript>()->IsDead(GetOwner()->Transform()->GetRelativePos());
		}
	
	//Transform()->SetRelativePos(50000.f, 50000.f,0);
	//Destroy();
}

void CGunProjScript::Overlap(CCollider2D* _pOther)
{
}

void CGunProjScript::EndOverlap(CCollider2D* _pOther)
{
}



