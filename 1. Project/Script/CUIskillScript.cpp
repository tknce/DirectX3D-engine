#include "pch.h"
#include "CUIskillScript.h"

#include <Engine/CLevel.h>
#include <Engine/CLevelMgr.h>
#include <Engine/CKeyMgr.h>

// gameobjact list
// battery			 - owner
// battery_part
// UIshift

CUIskillScript::CUIskillScript()
	:CScript(UISKILLSCRIPT)
	, m_fAcctime()
{
}

CUIskillScript::~CUIskillScript()
{
}

void CUIskillScript::begin()
{
	if (Animator2D() != nullptr)
	{
		Animator2D()->Play(L"battery",false);
	}
	if (nullptr != CLevelMgr::GetInst()->GetCurLevel()->FindObjectByName(L"UIshift"))
	{
		m_pShift = CLevelMgr::GetInst()->GetCurLevel()->FindObjectByName(L"UIshift");
		m_pShift->Animator2D()->Play(L"shift_none", false);
	}
	if (nullptr != CLevelMgr::GetInst()->GetCurLevel()->FindObjectByName(L"UIbattery_part"))
	{
		wstring name = L"UIbattery_part";
		m_pSkilltime.push_back(CLevelMgr::GetInst()->GetCurLevel()->FindObjectByName(L"UIbattery_part"));
		m_pSkilltime[0]->Animator2D()->Play(L"part", true);
		name += L"_";
		m_iIdx = 1;
		while (true)
		{
			wstring savename = name;
			savename += std::to_wstring(m_iIdx);
			if (CLevelMgr::GetInst()->GetCurLevel()->FindObjectByName(savename) != nullptr)
			{
				m_pSkilltime.push_back(CLevelMgr::GetInst()->GetCurLevel()->FindObjectByName(savename));
				m_pSkilltime[m_iIdx]->Animator2D()->Play(L"part", true);
				float pos = 12 * m_iIdx;
				m_pSkilltime[m_iIdx]->Transform()->SetRelativePos(-742.f + pos, 401.f, 0);
			}
			else
			{
				break;
			}
			++m_iIdx;
		}
		//m_pShift->Animator2D()->Play(L"battery_part", false);
		--m_iIdx;
	}

}

void CUIskillScript::tick()
{
	int skill = 1;
	m_fAccrecover += DT;
	if (KEY_PRESSED(KEY::LSHIFT) || KEY_TAP(KEY::LSHIFT))
	{
		m_pShift->Animator2D()->Play(L"shift_none", false);
		skill = 4;
		m_fAccSkill += DT * skill;
	}
	else
	{
		if(m_pShift != nullptr)
		m_pShift->Animator2D()->Play(L"shift_play", false);
		if (m_fAccSkill < 0)
			m_fAccSkill -= DT * skill;
	}
	m_fAcctime += DT * skill;
	if (m_fAcctime > 3.0f)
	{
		//m_iSpark = 1;
		if (m_fAcctime > 3.05f)
			m_fAcctime = 0;
	}
	else
	{
		m_iSpark = 0;
	}
	MeshRender()->GetCurMaterial(0)->SetScalarParam(SCALAR_PARAM::INT_0, &m_iSpark);

	
	if (KEY_PRESSED(KEY::LSHIFT) || KEY_TAP(KEY::LSHIFT))
	{
		if (m_fAccSkill > 0.5f)
		{
			int a = 1;
			if (m_iIdx != -1)
			{
				m_pSkilltime[m_iIdx]->MeshRender()->GetDynamicMaterial(0)->SetScalarParam(SCALAR_PARAM::INT_1, &a);
				m_fAccSkill = 0;
				m_fskillreturn = false;
				if (m_iIdx != 0)
					--m_iIdx;
			}
		}
	}
	if(m_fAccrecover > 0.5)
	{
		if (m_iIdx != m_pSkilltime.size())
		{
			int a = 0;
			m_pSkilltime[m_iIdx]->MeshRender()->GetDynamicMaterial(0)->SetScalarParam(SCALAR_PARAM::INT_1, &a);
			m_fAccrecover = 0;
			if(m_iIdx != m_pSkilltime.size() -1)
			++m_iIdx;
		}
	}
}




