#include "pch.h"
#include "CRazerScript.h"
#include "CPlayerScript.h"
#include <Engine/CPlayerMgr.h>
#include <Engine/CSoundMgr.h>

CRazerScript::CRazerScript()
	:CScript(RAZERSCRIPT)
{
}

CRazerScript::~CRazerScript()
{
}


void CRazerScript::begin()
{
	CSoundMgr::GetInst()->FindSound(L"sound\\laser\\laser_on.wav", 1, 0.6f);

	if(nullptr != GetOwner()->GetChildObject()[0])
	GetOwner()->GetChildObject()[0]->Animator2D()->Play(L"laser", true);
	//Instantiate(CResMgr::GetInst()->FindRes<CPrefab>(L"prefab\\Razer_Spark.pref").Get()->Instantiate(), Vec3(0,0,0));
}

void CRazerScript::tick()
{
	if(CPlayerMgr::GetInst()->GetDead() == 0)
		m_fRazer = 0;
}

void CRazerScript::BeginOverlap(CCollider2D* _pOther)
{
	if (_pOther->GetOwner()->GetName() == L"Player")
	{
		if (CPlayerMgr::GetInst()->GetDead() == 0 && _pOther->GetOwner()->GetScript<CPlayerScript>()->GetPower() > 0.5f)
		{
			CSoundMgr::GetInst()->FindSound(L"sound\\laser\\laser_explosion.wav", 1, 0.6f);
			Vec3 Pos = _pOther->Transform()->GetRelativePos();
			Pos.y += _pOther->Transform()->GetRelativeScale().y / 2 - 50;
			if (m_pParticle == nullptr)
			{
				m_pParticle = CResMgr::GetInst()->FindRes<CPrefab>(L"prefab\\Razer\\Razer_Spark.pref").Get()->Instantiate();
				m_pParticle->ParticleSystem()->SetMesh(CResMgr::GetInst()->FindRes<CMesh>(L"PointMesh"));
				m_pParticle->ParticleSystem()->SetSharedMaterial(CResMgr::GetInst()->FindRes<CMaterial>(L"ParticleRenderMtrl"),0);
				m_pParticle->ParticleSystem()->GetCurMaterial(0)->SetTexParam(TEX_0, CResMgr::GetInst()->FindRes<CTexture>(L"texture\\particle\\Sparks.png"));
				//Razer->SetName(L"1");
				Instantiate(m_pParticle, Pos);
			}
			else
			{
				m_pParticle->Transform()->SetRelativePos(Pos);
			}
			int a = 1;
			_pOther->GetOwner()->GetScript<CPlayerScript>()->SetRazerDead(true);
			CPlayerMgr::GetInst()->SetDead(1);
			_pOther->GetOwner()->MeshRender()->GetDynamicMaterial(0)->SetScalarParam(INT_1, &a);
			//CPlayerMgr::GetInst()->SetDead(1);
		}
		else
		{
			
		}
		
	}
}

void CRazerScript::Overlap(CCollider2D* _pOther)
{
	if (_pOther->GetOwner()->GetName() == L"Player")
	{
		if (CPlayerMgr::GetInst()->GetDead() == 1)
		{
			m_fRazer += 0.00003;
			if (m_pParticle != nullptr && !m_pParticle->IsDead())
			{
				Vec3 Pos = m_pParticle->Transform()->GetRelativePos();
				Pos.y -= 50 * DT;
				m_pParticle->Transform()->SetRelativePos(Pos);
				if (m_fRazer > 0.019)
				{
					m_pParticle->Transform()->SetRelativePos(0.f, 90000.f, 0.f);
				}

			}
			_pOther->GetOwner()->MeshRender()->GetDynamicMaterial(0)->SetScalarParam(FLOAT_3, &m_fRazer);
		}
		else
		{
			m_fRazer = 0;
			_pOther->GetOwner()->MeshRender()->GetDynamicMaterial(0)->SetScalarParam(FLOAT_3, &m_fRazer);
		}
		
		if (KEY_TAP(KEY::LBTN))
		{
			int a = 0;
			_pOther->GetOwner()->MeshRender()->GetDynamicMaterial(0)->SetScalarParam(INT_1, &a);
		}
	}
}

void CRazerScript::EndOverlap(CCollider2D* _pOther)
{
}
