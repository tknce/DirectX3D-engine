#include "pch.h"
#include "CTurnbackScript.h"

#include <Client/CSaveLoadMgr.h>
#include <Engine/CPlayerMgr.h>
#include <Engine/CSoundMgr.h>

CTurnbackScript::CTurnbackScript()
	:CScript(TURNBACKSCRIPT)
{
}

CTurnbackScript::~CTurnbackScript()
{
}

void CTurnbackScript::begin()
{
	m_fpos = 2;
	m_fUV = 2;
}

void CTurnbackScript::tick()
{
	m_fAccTime += DT;
	//float2  UV
	//float3  지지직강도
	//m_fpos = 2;
	//m_fUV = 2;
	//CSaveLoadMgr::GetInst()->
	if (CPlayerMgr::GetInst()->GetDead() == 1)
	{
		if (KEY_TAP(KEY::LBTN))
		{
			CSoundMgr::GetInst()->FindSound(L"sound\\rewind.wav", 1, 0.6f, true);
			TIMESTATE a = CSaveLoadMgr::GetInst()->GetTimestate();
			a.play = true;
			CSaveLoadMgr::GetInst()->SetTimeState(a);
			CPlayerMgr::GetInst()->Setreturn(true);
			//CPlayerMgr::GetInst()->SetDead(0);
			m_bDead = true;
		}
		
	}
	if (CPlayerMgr::GetInst()->GetDead() == 2)
	{
		if (KEY_TAP(KEY::LBTN))
		{
			CSoundMgr::GetInst()->FindSound(L"sound\\tape_change.wav", 1, 0.6f, true);
			TIMESTATE a = CSaveLoadMgr::GetInst()->GetTimestate();
			a.play = true;
			CSaveLoadMgr::GetInst()->SetTimeState(a);
			CPlayerMgr::GetInst()->Setreturn(true);

			m_bDead = true;
		}

	}
	if (m_bDead)
	{
		


	}
	/*MeshRender()->GetDynamicMaterial()->SetScalarParam(FLOAT_2, &m_fUV);
	MeshRender()->GetDynamicMaterial()->SetScalarParam(FLOAT_3, &m_fpos);*/
	//MeshRender()->GetSharedMaterial()->GetShader()->
	
	//MeshRender()->GetDynamicMaterial()->SetScalarParam(FLOAT_2, &m_fUV);
	//MeshRender()->GetDynamicMaterial()->SetScalarParam(FLOAT_3, &m_fpos);
}


