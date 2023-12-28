#include "pch.h"
#include "CEventMgr.h"

#include "CLevelMgr.h"
#include "CLevel.h"
#include "CLayer.h"
#include "CGameObject.h"
#include "CTransform.h"
#include "CSoundMgr.h"

#include <Client/CSaveLoadMgr.h>

#include "CResMgr.h"

CEventMgr::CEventMgr()
{
}

CEventMgr::~CEventMgr()
{
}

void CEventMgr::tick()
{
	// �̺�Ʈ ó��
	m_bLevelChanged = false;
	// �޸� ����
	for (size_t i = 0; i < m_vecGarbage.size(); ++i)
	{		
		
		delete m_vecGarbage[i];
		m_vecGarbage[i] = nullptr;
		m_bLevelChanged = true;
	}
	m_vecGarbage.clear();

	for (size_t i = 0; i < m_vecGarbageComponent.size(); ++i)
	{
		m_vecGarbage[i];
		m_bLevelChanged = true;
	}
	m_vecGarbageComponent.clear();

	
	for (size_t i = 0; i < m_vecEvent.size(); ++i)
	{
		switch (m_vecEvent[i].eType)
		{
		case EVENT_TYPE::CREATE_OBJECT:
		{
			// wParam : GameObject Adress
			// lParam : Layer Index
			CGameObject* pNewObj = (CGameObject*)m_vecEvent[i].wParam;
			int iLayerIdx = (int)m_vecEvent[i].lParam;

			CLevel* pLevel = CLevelMgr::GetInst()->GetCurLevel();
			pLevel->AddGameObject(pNewObj, iLayerIdx);		
			pNewObj->begin();
			m_bLevelChanged = true;
		}
			break;
		case EVENT_TYPE::DELETE_OBJECT:
		{
			// wParam : GameObject Adress
			CGameObject* pObj = (CGameObject*)m_vecEvent[i].wParam;

			if (!pObj->IsDead())
			{				
				// ����ó���� �ֻ��� �θ� �������� �ִ´�.
				m_vecGarbage.push_back(pObj);

				// ������ ������Ʈ ����, ��� �ڽĿ�����Ʈ�� Dead üũ�Ѵ�.
				static list<CGameObject*> queue;
				queue.push_back(pObj);
				while (!queue.empty())
				{
					CGameObject* pObj = queue.front();
					queue.pop_front();

					const vector<CGameObject*>& vecChild = pObj->GetChildObject();
					for (size_t i = 0; i < vecChild.size(); ++i)
					{
						queue.push_back(vecChild[i]);
					}

					pObj->m_bDead = true;
				}
			}
		}
			break;
		case EVENT_TYPE::ADD_CHILD:
		{
			// wParam : Child Adress, IParam : Parent Adress
			CGameObject* pParent = (CGameObject*)m_vecEvent[i].lParam;
			CGameObject* pChild = (CGameObject*)m_vecEvent[i].wParam;

			pParent->AddChild(pChild);
			m_bLevelChanged = true;
		}
			break;

		case EVENT_TYPE::ADD_RES:
		{
			// wParam : RES_TYPE, lParam : Resource Adress	
			CRes* pRes = (CRes*)m_vecEvent[i].lParam;
			wstring strKey = pRes->GetKey();
			CResMgr::GetInst()->AddRes(strKey, (RES_TYPE)m_vecEvent[i].wParam, (CRes*)m_vecEvent[i].lParam);
			pRes->Release();
		}
		break;

		case EVENT_TYPE::DELETE_RES:
		{
			// wParam : RES_TYPE, lParam : Resource Adress
			CRes* pRes = (CRes*)m_vecEvent[i].lParam;
			if (!CResMgr::GetInst()->DeleteRes((RES_TYPE)m_vecEvent[i].wParam, pRes->GetKey()))
			{
				MessageBox(nullptr, L"���ҽ� ���� ����", L"����", MB_OK);
			}
			pRes->Release();
		}
		break;

		case EVENT_TYPE::CHANGE_LEVEL_STATE:
		{
			// wParam : Level State
			m_bLevelChanged = true;
			//CLevelMgr::GetInst()->init();
			CLevelMgr::GetInst()->ChangeLevelState((LEVEL_STATE)m_vecEvent[i].wParam);
			
		}
		break;

		case EVENT_TYPE::CHANGE_LEVEL:
		{
			CSoundMgr::GetInst()->Stop();
			CLevelMgr::GetInst()->ChangeLevel((CLevel*)m_vecEvent[i].wParam);
			CSaveLoadMgr::GetInst()->ChangeLevel();
			CLevelMgr::GetInst()->init();
			m_bLevelChanged = true;
		}
		break;
		case EVENT_TYPE::DELETE_COMPONENT:
		{
			// wParam : Object Adress, IParam : COMPONETN_TYPE
			CGameObject* pObject = (CGameObject*)m_vecEvent[i].wParam;
			COMPONENT_TYPE Tpye = (COMPONENT_TYPE)m_vecEvent[i].lParam;

			m_bLevelChanged = true;
		}
		break;
		case EVENT_TYPE::CHANGE_LAYER_IDX:
		{
			// wParam : Object Adress, IParam : LayerIdx
			CGameObject* pObject = (CGameObject*)m_vecEvent[i].wParam;
			int LayerIdx = (int)m_vecEvent[i].lParam;

			
			CGameObject* pObj = pObject->Clone();
			if (pObj != nullptr)
			{
				Instantiate(pObj, pObject->Transform()->GetRelativePos(), LayerIdx);
				pObject->Destroy();
			}
			

			m_bLevelChanged = true;
		}
		break;

		case EVENT_TYPE::END:
			break;
		default:
			break;
		}
	}
	// wParam : Object Adress, IParam : COMPONETN_TYPE
	// wParam : Object Adress, IParam : LayerIdx	
	m_vecEvent.clear();
}