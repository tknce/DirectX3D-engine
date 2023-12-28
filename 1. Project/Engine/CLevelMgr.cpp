#include "pch.h"
#include "CLevelMgr.h"

#include "CResMgr.h"
#include "CCollisionMgr.h"
#include "CFontMgr.h"
#include "CEventMgr.h"

#include "CLevel.h"
#include "CKeyMgr.h"
#include "CGameObject.h"
#include "GlobalComponent.h"
#include "CTimeMgr.h"



CLevelMgr::CLevelMgr()
	: m_pCurLevel(nullptr)
	, m_a(false)
{
}

CLevelMgr::~CLevelMgr()
{
	if (nullptr != m_pCurLevel)
		delete m_pCurLevel;
}

LEVEL_STATE CLevelMgr::GetLevelState()
{
	return m_pCurLevel->GetState();
}

void CLevelMgr::init()
{
	// Level 하나 제작하기
	m_pCurLevel = new CLevel;
	m_pCurLevel->SetName(L"Level 0");

	//CLevel* Level = CSaveLoadMgr::GetInst()->LoadLevel(L"level\\factory_1.lv");

	//CLevelMgr::GetInst()->ChangeLevel(Level);
	int a = 0;
#ifdef _DEBUG
	//m_pCurLevel->SetState(LEVEL_STATE::PLAY);
	if (LEVEL_STATE::PLAY == m_pCurLevel->GetState())
	{
		m_pCurLevel->begin();
	}
#else
	
	if (LEVEL_STATE::PLAY == m_pCurLevel->GetState())
	{
		m_pCurLevel->begin();
	}
	tEvent evn = {};
	evn.eType = EVENT_TYPE::CHANGE_LEVEL_STATE;
	evn.wParam = (DWORD_PTR)LEVEL_STATE::PLAY;
	CEventMgr::GetInst()->AddEvent(evn);
#endif
	/*tEvent evn = {};
	evn.eType = EVENT_TYPE::CHANGE_LEVEL_STATE;
	evn.wParam = (DWORD_PTR)LEVEL_STATE::PLAY;
	CEventMgr::GetInst()->AddEvent(evn);*/

	//m_pCurLevel->begin();
	


	
	
}

void CLevelMgr::Progress()
{
	m_facctime += DT;
#ifdef _DEBUG
#else
	/*if (m_facctime > 1.5f && m_pCurLevel->GetState() != LEVEL_STATE::PLAY && m_a)
	{
		m_pCurLevel->SetState(LEVEL_STATE::PLAY);
		m_a = false;
	}*/
#endif
	m_pCurLevel->ClearLayer();
	
	

	if (LEVEL_STATE::PLAY == m_pCurLevel->GetState())
	{
		m_pCurLevel->tick();
	}
	
	m_pCurLevel->finaltick();

	//if (KEY_TAP(KEY::H))
	//{
	//	tEvent evn = {};
	//	evn.eType = EVENT_TYPE::CHANGE_LEVEL_STATE;
	//	evn.wParam = (DWORD_PTR)LEVEL_STATE::PLAY;
	//	CEventMgr::GetInst()->AddEvent(evn);
	//}
}

CGameObject* CLevelMgr::FindObjectByName(const wstring& _strName)
{
	return m_pCurLevel->FindObjectByName(_strName);
}

CGameObject* CLevelMgr::FindObjectByID(int _ID)
{


	return nullptr;
}

void CLevelMgr::FindObjectByName(const wstring& _strName, vector<CGameObject*>& _out)
{
	m_pCurLevel->FindObjectByName(_strName, _out);
}

void CLevelMgr::FindObjectByPos(CGameObject* _Pos, vector<CGameObject*>& _out)
{
	m_pCurLevel->FindObjectByPos(_Pos, _out);
}

void CLevelMgr::ChangeLevel(CLevel* _NextLevel)
{
	if (m_pCurLevel)
	{
		delete m_pCurLevel;
	}
	
	m_pCurLevel = _NextLevel;
	//ChangeLevelState(LEVEL_STATE::PLAY);
}

void CLevelMgr::ChangeLevelState(LEVEL_STATE _State)
{
	assert(!(m_pCurLevel->GetState() == _State));

	m_pCurLevel->SetState(_State);

	if (LEVEL_STATE::PLAY == _State)
		m_pCurLevel->begin();
}
