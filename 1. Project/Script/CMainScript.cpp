#include "pch.h"
#include "CMainScript.h"

#include <Engine/CLevelMgr.h>
#include <Engine/CLevel.h>
#include <Engine/CKeyMgr.h>
#include <Engine/CFontMgr.h>
CMainScript::CMainScript()
	: CScript(MAINSCRIPT)
	, m_bNextStage(false)
{
}

CMainScript::~CMainScript()
{
}


void CMainScript::begin()
{

	if (CLevelMgr::GetInst()->GetCurLevel()->FindObjectByName(L"Title") != nullptr)
	{
		CLevelMgr::GetInst()->GetCurLevel()->FindObjectByName(L"Title")->Animator2D()->Play(L"Title", false);
		m_pMenu.push_back(CLevelMgr::GetInst()->GetCurLevel()->FindObjectByName(L"Title"));
	}
	if (CLevelMgr::GetInst()->GetCurLevel()->FindObjectByName(L"Fance") != nullptr)
	{
		CLevelMgr::GetInst()->GetCurLevel()->FindObjectByName(L"Fance")->Animator2D()->Play(L"FANCE", false);
		m_pMenu.push_back(CLevelMgr::GetInst()->GetCurLevel()->FindObjectByName(L"Fance"));
	}
	if (CLevelMgr::GetInst()->GetCurLevel()->FindObjectByName(L"Back") != nullptr)
	{
		CLevelMgr::GetInst()->GetCurLevel()->FindObjectByName(L"Back")->Animator2D()->Play(L"BACK", false);
		m_pMenu.push_back(CLevelMgr::GetInst()->GetCurLevel()->FindObjectByName(L"Back"));
	}
	if (CLevelMgr::GetInst()->GetCurLevel()->FindObjectByName(L"Title_katana") != nullptr)
	{
		CLevelMgr::GetInst()->GetCurLevel()->FindObjectByName(L"Title_katana")->Animator2D()->Play(L"TITLE_KATANA", false);
		m_pMenu.push_back(CLevelMgr::GetInst()->GetCurLevel()->FindObjectByName(L"Title_katana"));
	}
	if (CLevelMgr::GetInst()->GetCurLevel()->FindObjectByName(L"LEAF") != nullptr)
	{
		CLevelMgr::GetInst()->GetCurLevel()->FindObjectByName(L"LEAF")->Animator2D()->Play(L"LEAF", true);
		m_pMenu.push_back(CLevelMgr::GetInst()->GetCurLevel()->FindObjectByName(L"LEAF"));
	}
	if (CLevelMgr::GetInst()->GetCurLevel()->FindObjectByName(L"Grass") != nullptr)
	{
		CLevelMgr::GetInst()->GetCurLevel()->FindObjectByName(L"Grass")->Animator2D()->Play(L"GRASS", true);
		m_pMenu.push_back(CLevelMgr::GetInst()->GetCurLevel()->FindObjectByName(L"Grass"));
	}
	if (CLevelMgr::GetInst()->GetCurLevel()->FindObjectByName(L"Menu") != nullptr)
	{
		CLevelMgr::GetInst()->GetCurLevel()->FindObjectByName(L"Menu")->Animator2D()->Play(L"Menu", true);
		m_pMenu.push_back(CLevelMgr::GetInst()->GetCurLevel()->FindObjectByName(L"Menu"));
	}
	if (CLevelMgr::GetInst()->GetCurLevel()->FindObjectByName(L"Menu_1") != nullptr)
	{
		CLevelMgr::GetInst()->GetCurLevel()->FindObjectByName(L"Menu_1")->Animator2D()->Play(L"Menu_1", true);
		m_pMenu.push_back(CLevelMgr::GetInst()->GetCurLevel()->FindObjectByName(L"Menu_1"));
	}
	if (CLevelMgr::GetInst()->GetCurLevel()->FindObjectByName(L"black") != nullptr)
	{
		m_pMenu.push_back(CLevelMgr::GetInst()->GetCurLevel()->FindObjectByName(L"black"));
	}
}

void CMainScript::tick()
{
	if (m_pMenu.data() == nullptr)
	{
		if (CLevelMgr::GetInst()->GetCurLevel()->FindObjectByName(L"Title") != nullptr)
		{
			CLevelMgr::GetInst()->GetCurLevel()->FindObjectByName(L"Title")->Animator2D()->Play(L"Title", false);
			m_pMenu.push_back(CLevelMgr::GetInst()->GetCurLevel()->FindObjectByName(L"Title"));
		}
		if (CLevelMgr::GetInst()->GetCurLevel()->FindObjectByName(L"Fance") != nullptr)
		{
			CLevelMgr::GetInst()->GetCurLevel()->FindObjectByName(L"Fance")->Animator2D()->Play(L"FANCE", false);
			m_pMenu.push_back(CLevelMgr::GetInst()->GetCurLevel()->FindObjectByName(L"Fance"));
		}
		if (CLevelMgr::GetInst()->GetCurLevel()->FindObjectByName(L"Back") != nullptr)
		{
			CLevelMgr::GetInst()->GetCurLevel()->FindObjectByName(L"Back")->Animator2D()->Play(L"BACK", false);
			m_pMenu.push_back(CLevelMgr::GetInst()->GetCurLevel()->FindObjectByName(L"Back"));
		}
		if (CLevelMgr::GetInst()->GetCurLevel()->FindObjectByName(L"Title_katana") != nullptr)
		{
			CLevelMgr::GetInst()->GetCurLevel()->FindObjectByName(L"Title_katana")->Animator2D()->Play(L"TITLE_KATANA", false);
			m_pMenu.push_back(CLevelMgr::GetInst()->GetCurLevel()->FindObjectByName(L"Title_katana"));
		}
		if (CLevelMgr::GetInst()->GetCurLevel()->FindObjectByName(L"LEAF") != nullptr)
		{
			CLevelMgr::GetInst()->GetCurLevel()->FindObjectByName(L"LEAF")->Animator2D()->Play(L"LEAF", true);
			m_pMenu.push_back(CLevelMgr::GetInst()->GetCurLevel()->FindObjectByName(L"LEAF"));
		}
		if (CLevelMgr::GetInst()->GetCurLevel()->FindObjectByName(L"Grass") != nullptr)
		{
			CLevelMgr::GetInst()->GetCurLevel()->FindObjectByName(L"Grass")->Animator2D()->Play(L"GRASS", true);
			m_pMenu.push_back(CLevelMgr::GetInst()->GetCurLevel()->FindObjectByName(L"Grass"));
		}
		if (CLevelMgr::GetInst()->GetCurLevel()->FindObjectByName(L"Menu") != nullptr)
		{
			CLevelMgr::GetInst()->GetCurLevel()->FindObjectByName(L"Menu")->Animator2D()->Play(L"Menu", true);
			m_pMenu.push_back(CLevelMgr::GetInst()->GetCurLevel()->FindObjectByName(L"Menu"));
		}
		if (CLevelMgr::GetInst()->GetCurLevel()->FindObjectByName(L"Menu_1") != nullptr)
		{
			CLevelMgr::GetInst()->GetCurLevel()->FindObjectByName(L"Menu_1")->Animator2D()->Play(L"Menu_1", true);
			m_pMenu.push_back(CLevelMgr::GetInst()->GetCurLevel()->FindObjectByName(L"Menu_1"));
		}
		if (CLevelMgr::GetInst()->GetCurLevel()->FindObjectByName(L"black") != nullptr)
		{
			m_pMenu.push_back(CLevelMgr::GetInst()->GetCurLevel()->FindObjectByName(L"black"));
		}
	}

	//DrawFont(const wchar_t* _pStr, float _fPosX, float _fPosY, float _fFontSize, UINT _Color);
	m_fAcctime += DT;
	if (!m_bNextStage)
	{
		
		char a = 125;
		if (m_fAcctime > 0.5f)
		{
			a = 255;
			if (m_fAcctime > 1.f)
				m_fAcctime = 0;
		}


		CFontMgr::GetInst()->SetDrawFont(L"START", 750.f, 550.f, 20, FONT_RGBA(255, 252, 255, a));
		CFontMgr::GetInst()->SetDrawFont(L"EXIT", 762.f, 600.f, 20, FONT_RGBA(255, 252, 255, a));
	}


	if (KEY_TAP(KEY::SPACE) || m_bNextStage)
	{
		m_bNextStage = true;
		wstring Name;
		Vec3 Pos;
		CGameObject* game = CLevelMgr::GetInst()->GetCurLevel()->FindObjectByName(L"Title");

			Name = L"Title";
			Pos = m_pMenu[0]->Transform()->GetRelativePos();
			Pos.y += 300.f * DT;
			m_pMenu[0]->Transform()->SetRelativePos(Pos);
		

			Name = L"Fance";
			Pos = m_pMenu[1]->Transform()->GetRelativePos();
			Pos.y += 1.f * DT;
			m_pMenu[1]->Transform()->SetRelativePos(Pos);
		

			Name = L"Back";
			Pos = m_pMenu[2]->Transform()->GetRelativePos();
			Pos.y += 1.f * DT;
			m_pMenu[2]->Transform()->SetRelativePos(Pos);
		

			Name = L"Title_katana";
			Pos = m_pMenu[3]->Transform()->GetRelativePos();
			Pos.y += 300.f * DT;
			m_pMenu[3]->Transform()->SetRelativePos(Pos);
		

			Name = L"LEAF";
			Pos = m_pMenu[4]->Transform()->GetRelativePos();
			Pos.y += 1000.f * DT;
			m_pMenu[4]->Transform()->SetRelativePos(Pos);
		

			Name = L"Grass";
			Pos = m_pMenu[5]->Transform()->GetRelativePos();
			Pos.y += 700.f * DT;
			m_pMenu[5]->Transform()->SetRelativePos(Pos);


			Name = L"Menu";

				Pos = m_pMenu[6]->Transform()->GetRelativePos();
				Pos.y += 300.f * DT;
				m_pMenu[6]->Transform()->SetRelativePos(Pos);
				
			
			

			Name = L"Menu_1";
			if (m_pMenu[7] != nullptr)
			{
				/*Pos = m_pMenu[7]->Transform()->GetRelativePos();
				Pos.y += 300.f * DT;
				m_pMenu[7]->Transform()->SetRelativePos(Pos);*/
				m_pMenu[7]->Transform()->SetRelativeScale(0.f, 0.f, 0.f);
			}
			Name = L"black";
			Pos = m_pMenu[8]->Transform()->GetRelativePos();
			Pos.y += 700.f * DT;
			m_pMenu[8]->Transform()->SetRelativePos(Pos);
		
	}
}


