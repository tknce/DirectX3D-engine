#include "pch.h"
#include "CTargetScript.h"

#include <Engine/CFontMgr.h>
#include <Engine/CSoundMgr.h>

CTargetScript::CTargetScript()
	:CScript(TARGETSCRIPT)
	, m_b(false)
{
}

CTargetScript::~CTargetScript()
{
}


void CTargetScript::begin()
{
	CSoundMgr::GetInst()->FindSound(L"sound\\song_ending.ogg", 0, 0.6f, true);
	
	if (Animator2D() != nullptr)
	{
		Animator2D()->Play(L"target", true);
	}
}

void CTargetScript::tick()
{
	m_fAcctime += DT;

	if (m_fAcctime > 3.f)
	{
		if (m_b)
			CFontMgr::GetInst()->SetDrawFont(L"  감사합니다", 700.f, 400.f, 10, FONT_RGBA(255, 255, 255, 255));
	}

}

void CTargetScript::BeginOverlap(CCollider2D* _pOther)
{
	int a = 1;
	MeshRender()->GetDynamicMaterial(0)->SetTexParam(TEX_1, CResMgr::GetInst()->FindRes<CTexture>(L"texture\\particle\\smokeparticle.png"));
	MeshRender()->GetDynamicMaterial(0)->SetScalarParam(INT_3, &a);
	m_fAcctime = 0;
	m_b = true;
	
}

void CTargetScript::Overlap(CCollider2D* _pOther)
{
}

void CTargetScript::EndOverlap(CCollider2D* _pOther)
{
}



