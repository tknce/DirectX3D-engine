#include "pch.h"
#include "CVentScript.h"

#include <Engine/CAnimator2D.h>
#include <Engine/CAnimation2D.h>
#include "CPlayerScript.h"

CVentScript::CVentScript()
	:CScript(VENTSCRIPT)
{
}

CVentScript::~CVentScript()
{

}

void CVentScript::begin()
{
	if (GetOwner()->Animator2D() != nullptr)
	{
		m_pCurAni = GetOwner()->Animator2D()->Play(L"VENT", true);

		GetOwner()->GetChildObject()[0]->Animator2D()->Play(L"fan",false);
		GetOwner()->GetChildObject()[1]->Animator2D()->Play(L"fan_back", false);
	}
}

void CVentScript::tick()
{
	m_fAcctime += DT;
	if (m_pCurAni != nullptr)
	{
		vector<tAnim2DFrm> Frm = m_pCurAni->GetAnimFrm();
		if (KEY_PRESSED(KEY::LSHIFT))
		{
			for (size_t i = 0; i < Frm.size(); ++i)
			{
				Frm[i].fDuration = 0.03;

			}
			m_pCurAni->SetFrm(Frm);
		}
		else
		{
			for (size_t i = 0; i < Frm.size(); ++i)
			{
				Frm[i].fDuration = 0.001;
			}
			m_pCurAni->SetFrm(Frm);
		}
	}
	if (m_fAcctime > 2.5f)
	{
		m_fAcctime = 0;
	}
	
}

void CVentScript::BeginOverlap(CCollider2D* _pOther)
{
	if (m_pCurAni != nullptr)
	{
		if (KEY_PRESSED(KEY::LSHIFT))
		{
			if (m_fAcctime > 2)
			{
				if (_pOther->GetOwner()->GetName() == L"Player" )
				{
					if(_pOther->GetOwner()->GetScript<CPlayerScript>()->GetState() != PLAYER_STATE_FIGHT::HURTGROUND)
					_pOther->GetOwner()->GetScript<CPlayerScript>()->IsDead(GetOwner()->Transform()->GetRelativePos());
				}
				m_fAcctime = 0;
			}
		}
		else
		{
			if (_pOther->GetOwner()->GetName() == L"Player")
			{
				if (_pOther->GetOwner()->GetScript<CPlayerScript>()->GetState() != PLAYER_STATE_FIGHT::HURTGROUND)
				_pOther->GetOwner()->GetScript<CPlayerScript>()->IsDead(GetOwner()->Transform()->GetRelativePos());
			}
		}
	}
}

void CVentScript::Overlap(CCollider2D* _pOther)
{
	if (m_pCurAni != nullptr)
	{
		if (KEY_PRESSED(KEY::LSHIFT))
		{
			/*if (m_fAcctime > 2)
			{
				if (_pOther->GetOwner()->GetName() == L"Player")
				{
					if (_pOther->GetOwner()->GetScript<CPlayerScript>()->GetState() != PLAYER_STATE_FIGHT::HURTGROUND)
					_pOther->GetOwner()->GetScript<CPlayerScript>()->IsDead(GetOwner()->Transform()->GetRelativePos());
				}
				m_fAcctime = 0;
			}*/
		}
		else
		{
			if (_pOther->GetOwner()->GetName() == L"Player")
			{
				if (_pOther->GetOwner()->GetScript<CPlayerScript>()->GetState() != PLAYER_STATE_FIGHT::HURTGROUND)
				_pOther->GetOwner()->GetScript<CPlayerScript>()->IsDead(GetOwner()->Transform()->GetRelativePos());
			}
		}
	}
}

void CVentScript::EndOverlap(CCollider2D* _pOther)
{
}



