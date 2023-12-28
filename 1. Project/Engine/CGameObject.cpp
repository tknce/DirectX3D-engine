#include "pch.h"
#include "CGameObject.h"

#include <Script\CScriptMgr.h>


#include "CComponent.h"
#include "CRenderComponent.h"
#include "CMeshRender.h"
#include "CScript.h"

#include "CPlayerMgr.h"
#include "CLevelMgr.h"
#include "CLevel.h"

CGameObject::CGameObject()
	: m_pParent(nullptr)
	, m_arrCom{}
	, m_pRenderComponent(nullptr)
	, m_iLayerIdx(-1)
{

}

CGameObject::CGameObject(const CGameObject& _origin)
	: CEntity(_origin)
	, m_pParent(nullptr)
	, m_arrCom{}
	, m_pRenderComponent(nullptr)
	, m_iLayerIdx(-1)
{
	for (UINT i = 0; i < (UINT)COMPONENT_TYPE::END; ++i)
	{
		if (nullptr == _origin.m_arrCom[i])
			continue;

		AddComponent(_origin.m_arrCom[i]->Clone());		
	}
	
	for (size_t i = 0; i < _origin.m_vecScripts.size(); ++i)
	{
		AddComponent(_origin.m_vecScripts[i]->Clone());
	}

	for (size_t i = 0; i < _origin.m_vecChild.size(); ++i)
	{
		AddChild(_origin.m_vecChild[i]->Clone());
	}	
}

CGameObject::~CGameObject()
{
	Safe_Del_Array(m_arrCom);
	Safe_Del_Vec(m_vecScripts);
	Safe_Del_Vec(m_vecChild);
}

void CGameObject::begin()
{
	// Components
	for (UINT i = 0; i < (UINT)COMPONENT_TYPE::END; ++i)
	{
		if (nullptr != m_arrCom[i])
			m_arrCom[i]->begin();
	}

	// Scripts
	for (size_t i = 0; i < m_vecScripts.size(); ++i)
	{
		m_vecScripts[i]->begin();
	}

	// Child Object
	for (size_t i = 0; i < m_vecChild.size(); ++i)
	{
		m_vecChild[i]->begin();
	}
}

void CGameObject::tick()
{

	// Comopnent
	for (UINT i = 0; i < (UINT)COMPONENT_TYPE::END; ++i)
	{
		if(nullptr != m_arrCom[i])
			m_arrCom[i]->tick();
	}


		for (size_t i = 0; i < m_vecScripts.size(); ++i)
		{
			m_vecScripts[i]->tick();
		}
	
	

	// Child Object
	for (size_t i = 0; i < m_vecChild.size(); ++i)
	{
		m_vecChild[i]->tick();
	}
}

void CGameObject::finaltick()
{
	// Component
	for (UINT i = 0; i < (UINT)COMPONENT_TYPE::END; ++i)
	{
		if (nullptr != m_arrCom[i])
			m_arrCom[i]->finaltick();
	}

	// Child Object
	vector<CGameObject*>::iterator iter = m_vecChild.begin();
	for (; iter != m_vecChild.end();)
	{
		(*iter)->finaltick();

		if ((*iter)->IsDead())
		{
			//delete (*iter);
			iter = m_vecChild.erase(iter);
		}
		else
		{
			++iter;
		}
	}

	// Register Layer
	CLevel* pCurLevel = CLevelMgr::GetInst()->GetCurLevel();
	CLayer* pLayer = pCurLevel->GetLayer(m_iLayerIdx);
	pLayer->RegisterObject(this);
}

void CGameObject::finaltick_module()
{
	// Component
	for (UINT i = 0; i < (UINT)COMPONENT_TYPE::END; ++i)
	{
		if (nullptr != m_arrCom[i])
			m_arrCom[i]->finaltick_module();
	}

	// Child Object
	vector<CGameObject*>::iterator iter = m_vecChild.begin();
	for (; iter != m_vecChild.end();)
	{
		(*iter)->finaltick_module();

		if ((*iter)->IsDead())
		{
			iter = m_vecChild.erase(iter);
		}
		else
		{
			++iter;
		}
	}
}

void CGameObject::render()
{
	if (nullptr == m_pRenderComponent)
		return;

	m_pRenderComponent->render();
}

void CGameObject::AddComponent(CComponent* _pComponent)
{
	COMPONENT_TYPE eComType = _pComponent->GetType();

	// 스크립트가 아닌 경우
	if (COMPONENT_TYPE::SCRIPT != eComType)
	{
		//assert(!m_arrCom[(UINT)eComType]);

		// 입력된 Component 가 RenderComponent 라면
		CRenderComponent* pRenderCom = dynamic_cast<CRenderComponent*>(_pComponent);
		if (nullptr != pRenderCom )
		{
			assert(!m_pRenderComponent); // render 기능 컴포넌트는 한개만 가질 수 있다.
			m_pRenderComponent = pRenderCom;
		}

		// GameObject 와 Component 가 서로를 가리킴
		_pComponent->m_pOwnerObject = this;
		m_arrCom[(UINT)eComType] = _pComponent;
	}
	
	// Script 인 경우
	else
	{
		_pComponent->m_pOwnerObject = this;
		m_vecScripts.push_back((CScript*)_pComponent);		
	}
}



bool CGameObject::ComponentCheck(COMPONENT_TYPE _eComType)
{
	if (!IsValid(m_arrCom[(UINT)_eComType]))
		return false;

	return true;
}

void CGameObject::AddChild(CGameObject* _pChild)
{
	// 자식으로 들어오는 오브젝트가 최상위 부모 오브젝트다

	if (nullptr == _pChild->GetParent())
	{
		if (-1 == _pChild->GetLayerIdx())
		{
			_pChild->m_iLayerIdx = m_iLayerIdx;
		}
		else
		{
			CLayer* pLayer = CLevelMgr::GetInst()->GetCurLevel()->GetLayer(_pChild->m_iLayerIdx);
			pLayer->DeregisterObject(_pChild);
		}
	}

	// 자식으로 들어오는 오브젝트가 다른 오브젝트의 자식 오브젝트였다면
	else
	{
		_pChild->DisconnectFromParent();
	}

	_pChild->m_pParent = this;
	m_vecChild.push_back(_pChild);
}

void CGameObject::AddChild_LOAD(CGameObject* _pChild)
{
	_pChild->m_pParent = this;
	m_vecChild.push_back(_pChild);
}

void CGameObject::DisconnectFromParent()
{
	vector<CGameObject*>::iterator iter = m_pParent->m_vecChild.begin();

	for (; iter != m_pParent->m_vecChild.end(); ++iter)
	{
		if (*iter == this)
		{
			m_pParent->m_vecChild.erase(iter);
			m_pParent = nullptr;

			return;
		}
	}

	assert(nullptr);
}

void CGameObject::DeleteComponent(COMPONENT_TYPE _eComType)
{
	switch (_eComType)
	{
	case COMPONENT_TYPE::TRANSFORM:
		delete (CTransform*)m_arrCom[(UINT)_eComType];
		m_arrCom[(UINT)_eComType] = nullptr;
		break;
	case COMPONENT_TYPE::CAMERA:
		delete (CCamera*)m_arrCom[(UINT)_eComType];
		m_arrCom[(UINT)_eComType] = nullptr;
		break;
	case COMPONENT_TYPE::COLLIDER2D:
		delete (CCollider2D*)m_arrCom[(UINT)_eComType];
		m_arrCom[(UINT)_eComType] = nullptr;
		break;
	case COMPONENT_TYPE::COLLIDER3D:
		break;
	case COMPONENT_TYPE::ANIMATOR2D:
		delete (CAnimator2D*)m_arrCom[(UINT)_eComType];
		m_arrCom[(UINT)_eComType] = nullptr;
		break;
	case COMPONENT_TYPE::ANIMATOR3D:
		break;
	case COMPONENT_TYPE::LIGHT2D:
		delete (CLight2D*)m_arrCom[(UINT)_eComType];
		m_arrCom[(UINT)_eComType] = nullptr;
		break;
	case COMPONENT_TYPE::LIGHT3D:
		break;
	case COMPONENT_TYPE::RIGIDBODY2D:
		delete (CRigidBody2D*)m_arrCom[(UINT)_eComType];
		m_arrCom[(UINT)_eComType] = nullptr;
		break;
	case COMPONENT_TYPE::RIGIDBODY3D:
		delete (CRigidBody2D*)m_arrCom[(UINT)_eComType];
		m_arrCom[(UINT)_eComType] = nullptr;
		break;
	case COMPONENT_TYPE::MESHRENDER:
		delete (CMeshRender*)m_arrCom[(UINT)_eComType];
		m_arrCom[(UINT)_eComType] = nullptr;
		break;
	case COMPONENT_TYPE::TILEMAP:
		delete (CTileMap*)m_arrCom[(UINT)_eComType];
		m_arrCom[(UINT)_eComType] = nullptr;
		break;
	case COMPONENT_TYPE::PARTICLESYSTEM:
		delete (CParticleSystem*)m_arrCom[(UINT)_eComType];
		m_arrCom[(UINT)_eComType] = nullptr;
		break;
	case COMPONENT_TYPE::SKYBOX:
		break;
	case COMPONENT_TYPE::DECAL:
		break;
	case COMPONENT_TYPE::LANDSCAPE:
		break;
	case COMPONENT_TYPE::END:
		break;
	case COMPONENT_TYPE::SCRIPT:
		break;
	case COMPONENT_TYPE::AI:
		delete (AI*)m_arrCom[(UINT)_eComType];
		m_arrCom[(UINT)_eComType] = nullptr;
		break;
	default:
		break;
	}
}

void CGameObject::DeleteScript(int _scriptidx)
{
	vector<CScript*> vecscript = m_vecScripts;
	m_vecScripts.clear();
	for (size_t i = 0; i < vecscript.size(); ++i)
	{
		if (_scriptidx == vecscript[i]->GetScriptType())
		{
			vecscript[i]->Delete();
			vecscript[i] = nullptr;
			continue;
		}
		m_vecScripts.push_back(vecscript[i]);
	}
}

CGameObject* CGameObject::GetChildwstring(wstring _ChildName)
{
	for (size_t i = 0; i < m_vecChild.size(); ++i)
	{
		if (m_vecChild[i]->GetName() == _ChildName)
		{
			return m_vecChild[i];
		}
	}

	return nullptr;
}



#include "CEventMgr.h"

void CGameObject::Destroy()
{
	tEvent eve = {};
	eve.eType = EVENT_TYPE::DELETE_OBJECT;
	eve.wParam = (DWORD_PTR)this;

	CEventMgr::GetInst()->AddEvent(eve);
}