#include "pch.h"
#include "CTitleScript.h"

#include <Engine/CTimeMgr.h>
#include <Engine/CLevelMgr.h>
#include <Engine/CLevel.h>
#include <Client/CSaveLoadMgr.h>
#include <Engine/CSoundMgr.h>

CTitleScript::CTitleScript()
	: CScript(TITLESCRIPT)
{
}

CTitleScript::~CTitleScript()
{
}


void CTitleScript::begin()
{
	m_fAcctime += DT;
	if (GetOwner() != nullptr)
	{
		Ptr<CSound> pSound = CResMgr::GetInst()->FindRes<CSound>(L"sound\\menu\\rain_title.wav");
		CSoundMgr::GetInst()->RigsterSound(pSound);
		pSound->Play(0, 0.6f, true);

		pSound = CResMgr::GetInst()->FindRes<CSound>(L"sound\\menu\\song_rainonbrick.ogg");
		CSoundMgr::GetInst()->RigsterSound(pSound);
		pSound->Play(1, 0.6f, true);

		pSound = CResMgr::GetInst()->FindRes<CSound>(L"sound\\menu\\menu_wind.wav");
		CSoundMgr::GetInst()->RigsterSound(pSound);
		pSound->Play(0, 0.6f, true);

		GetOwner()->Animator2D()->Play(L"TITLE", false);
		vector<CGameObject*> child = GetOwner()->GetChildObject();
		for (size_t i = 0; i < child.size(); ++i)
		{
			if (child[i]->GetName() == L"Title_Zer")
			{
				m_pZer = child[i];
				child[i]->Animator2D()->Play(L"ZER", false);
			}
				

			if (child[i]->GetName() == L"Title_O")
			{
				m_pO = child[i];
				child[i]->Animator2D()->Play(L"O", false);
			}
				
		}
		m_pMenu = CLevelMgr::GetInst()->GetCurLevel()->FindObjectByName(L"Menu_1");
	}
}

void CTitleScript::tick()
{
	m_fAcctime_Zer += DT;
	m_fAcctime_O += DT;
	m_fAcctime += DT;

	if (m_fAcctime_Zer > 5.f)
	{
		m_pZer->Transform()->SetRelativeScale(1.f, 1.f,0.f);
		m_fAcctime_Zer = 0;
	}

	if (m_fAcctime_Zer > 0.2f)
	{
		m_pZer->Transform()->SetRelativeScale(1000.f, 1000.f, 0.f);
	}
	if (m_fAcctime_Zer > 0.25f)
	{
		m_pZer->Transform()->SetRelativeScale(1.f, 1.f, 0.f);
	}
	if (m_fAcctime_Zer > 0.3f)
	{
		m_pZer->Transform()->SetRelativeScale(1000.f, 1000.f, 0.f);
	}
	if (m_fAcctime_Zer > 0.35f)
	{
		m_pZer->Transform()->SetRelativeScale(1.f, 1.f, 0.f);
	}
	if (m_fAcctime_Zer > 0.4f)
	{
		m_pZer->Transform()->SetRelativeScale(1000.f, 1000.f, 0.f);
	}
	if (m_fAcctime_Zer > 0.45f)
	{
		m_pZer->Transform()->SetRelativeScale(1.f, 1.f, 0.f);
	}
	if (m_fAcctime_Zer > 0.5f)
	{
		m_pZer->Transform()->SetRelativeScale(1000.f, 1000.f, 0.f);
	}
	if (m_fAcctime_Zer > 0.55f)
	{
		m_pZer->Transform()->SetRelativeScale(1.f, 1.f, 0.f);
	}
	if (m_fAcctime_Zer > 0.60f)
	{
		m_pZer->Transform()->SetRelativeScale(1000.f, 1000.f, 0.f);
	}




	if (m_fAcctime_O > 2.7f)
	{
		m_pO->Transform()->SetRelativeScale(1.f, 1.f, 0.f);
		m_fAcctime_O = 0;
	}

	if (m_fAcctime_O > 0.25f)
	{
		m_pO->Transform()->SetRelativeScale(1000.f, 1000.f, 0.f);
	}
	if (m_fAcctime_O > 0.3f)
	{
		m_pO->Transform()->SetRelativeScale(1.f, 1.f, 0.f);
	}
	if (m_fAcctime_O > 0.32f)
	{
		m_pO->Transform()->SetRelativeScale(1000.f, 1000.f, 0.f);
	}
	if (m_fAcctime_O > 0.4f)
	{
		m_pO->Transform()->SetRelativeScale(1.f, 1.f, 0.f);
	}
	if (m_fAcctime_O > 0.45f)
	{
		m_pO->Transform()->SetRelativeScale(1000.f, 1000.f, 0.f);
	}


	if (m_pMenu != nullptr)
	{
		Vec3 Pos = m_pMenu->Transform()->GetRelativePos();
		if (KEY_TAP(KEY::W))
		{
			
			if (Pos.y < -120)
			{
				Ptr<CSound> pSound = CResMgr::GetInst()->FindRes<CSound>(L"sound\\menu\\menubeep_up.wav");
				CSoundMgr::GetInst()->RigsterSound(pSound);
				pSound->Play(1, 0.6f, true);
				Pos.y += 50.f;
			}
		}
		if (KEY_TAP(KEY::S))
		{
			
			if (Pos.y > -170)
			{
				Ptr<CSound> pSound = CResMgr::GetInst()->FindRes<CSound>(L"sound\\menu\\menubeep.wav");
				CSoundMgr::GetInst()->RigsterSound(pSound);
				pSound->Play(1, 0.6f, true);
				Pos.y -= 50.f;
				
			}
		}
		if (Pos.y == -120 && KEY_TAP(KEY::SPACE))
		{
			Ptr<CSound> pSound = CResMgr::GetInst()->FindRes<CSound>(L"sound\\menu\\menu_intro.wav");
			CSoundMgr::GetInst()->RigsterSound(pSound);
			pSound->Play(1, 0.6f, true);
			m_bNexttime = true;
		}
		
		if (m_bNexttime)
		{
			if (m_fAcctime > 1.5f)
			{

				ChangeLevel(CSaveLoadMgr::GetInst()->LoadLevel(L"level\\factory_1.lv"));
				//CLevelMgr::GetInst()->ChangeLevel(CSaveLoadMgr::GetInst()->LoadLevel(L"level\\factory_1.lv"));
			}
		}
		else
		{
			m_fAcctime = 0.f;
		}

		if (Pos.y == -170 && KEY_TAP(KEY::SPACE))
		{
			// 종료 함수
			PostQuitMessage(WM_QUIT);
		}
		m_pMenu->Transform()->SetRelativePos(Pos);



	}
}


