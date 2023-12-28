#include "pch.h"
#include "CLevel.h"

#include "CLevelMgr.h"
#include "CGameObject.h"
#include "CTransform.h"

CLevel::CLevel()	
{
	m_arrLayer[0].SetName(L"Default");

	for (int i = 0; i < MAX_LAYER; ++i)
	{
		m_arrLayer[i].m_iLayerIdx = i;
	}
}

CLevel::~CLevel()
{

}


void CLevel::begin()
{
	for (UINT i = 0; i < MAX_LAYER; ++i)
	{
		m_arrLayer[i].begin();
	}
}

void CLevel::tick()
{
	

	/*CLevelMgr::GetInst()->ChangeLevel();
	CLevel* Level = CSaveLoadMgr::GetInst()->LoadLevel(L"level\\Test.lv");
	CLevel* Level = CSaveLoadMgr::GetInst()->LoadLevel(L"level\\Test.lv");
	CLevel* Level = CSaveLoadMgr::GetInst()->LoadLevel(L"level\\Test.lv");
	CLevel* Level = CSaveLoadMgr::GetInst()->LoadLevel(L"level\\Test.lv");*/



	for (UINT i = 0; i < MAX_LAYER; ++i)
	{
		m_arrLayer[i].clear();
	}


	for (UINT i = 0; i < MAX_LAYER; ++i)
	{
		m_arrLayer[i].tick();
	}
}

void CLevel::finaltick()
{
	for (UINT i = 0; i < MAX_LAYER; ++i)
	{
		m_arrLayer[i].finaltick();
	}
}

void CLevel::ClearLayer()
{
	for (UINT i = 0; i < MAX_LAYER; ++i)
	{
		m_arrLayer[i].clear();
	}
}

CLayer* CLevel::GetLayer(const wstring& _strLayerName)
{
	for (int i = 0; i < MAX_LAYER; ++i)
	{
		if (_strLayerName == m_arrLayer[i].GetName())
			return &m_arrLayer[i];
	}
	
	return nullptr;
}

void CLevel::GetGameObject(vector<vector<CGameObject*>>& _out)
{
	for (UINT i = 0; i < MAX_LAYER; ++i)
	{
		_out.push_back(m_arrLayer[i].GetObjects());
	}
}

void CLevel::AddGameObject(CGameObject* _pObject, UINT _iLayerIdx)
{
	assert(0 <= _iLayerIdx && _iLayerIdx < MAX_LAYER);

	m_arrLayer[_iLayerIdx].AddGameObject(_pObject);
}

void CLevel::AddGameObject(CGameObject* _pObject, const wstring& _strLayerName)
{
	CLayer* pLayer = GetLayer(_strLayerName);

	assert(pLayer);

	pLayer->AddGameObject(_pObject);
}

CGameObject* CLevel::FindObjectByName(const wstring& _Name)
{
	for (UINT i = 0; i < MAX_LAYER; ++i)
	{
		const vector<CGameObject*>& vecObjects = m_arrLayer[i].GetObjects();

		for (size_t j = 0; j < vecObjects.size(); ++j)
		{
			if (_Name == vecObjects[j]->GetName())
				return vecObjects[j];
		}
	}

	return nullptr;
}

CGameObject* CLevel::FindObjectByID(int _ID)
{
	for (UINT i = 0; i < MAX_LAYER; ++i)
	{
		const vector<CGameObject*>& vecObjects = m_arrLayer[i].GetObjects();

		for (size_t j = 0; j < vecObjects.size(); ++j)
		{
			if (_ID == vecObjects[j]->GetID())
				return vecObjects[j];
		}
	}
	return nullptr;
}

void CLevel::FindObjectByName(const wstring& _Name, vector<CGameObject*>& _out)
{
	for (UINT i = 0; i < MAX_LAYER; ++i)
	{
		const vector<CGameObject*>& vecObjects = m_arrLayer[i].GetObjects();

		for (size_t j = 0; j < vecObjects.size(); ++j)
		{
			if (_Name == vecObjects[j]->GetName())
				_out.push_back(vecObjects[j]);
		}
	}
}

void CLevel::FindObjectByWord(const wstring& _Name, vector<CGameObject*>& _out)
{
	for (UINT i = 0; i < MAX_LAYER; ++i)
	{
		const vector<CGameObject*>& vecObjects = m_arrLayer[i].GetObjects();

		for (size_t j = 0; j < vecObjects.size(); ++j)
		{
			if (_Name[0] == vecObjects[j]->GetName()[0] && _Name[1] == vecObjects[j]->GetName()[1])
				_out.push_back(vecObjects[j]);
		}
	}
}

void CLevel::FindObjectByPos(CGameObject* _Pos, vector<CGameObject*>& _out)
{
	Vec3 FindPos_LeftTop;
	Vec3 FindPos_Slice;
	Vec3 FindPos_LayerLeftTop;
	Vec3 FindPos_LayerSlice;
	if (_Pos->Transform() != nullptr) 
	{
		_Pos->Transform()->GetRelativePos();
		Vec3 Scale = _Pos->Transform()->GetRelativeScale() / 2;
		FindPos_LeftTop = _Pos->Transform()->GetRelativePos() - Scale;
		FindPos_Slice = _Pos->Transform()->GetRelativePos() + Scale;
	}
	for (UINT i = 0; i < MAX_LAYER; ++i)
	{
		const vector<CGameObject*>& vecObjects = m_arrLayer[i].GetParentObjects();

		for (size_t j = 0; j < vecObjects.size(); ++j)
		{
			if (vecObjects[j]->Transform() != nullptr)
			{
				vecObjects[j]->Transform()->GetRelativePos();
				Vec3 Scale = vecObjects[j]->Transform()->GetRelativeScale() / 2;
				FindPos_LayerLeftTop = vecObjects[j]->Transform()->GetRelativePos() - Scale;
				FindPos_LayerSlice = vecObjects[j]->Transform()->GetRelativePos() + Scale;
			}

			if (FindPos_LeftTop.x > FindPos_LayerLeftTop.x &&
				FindPos_LeftTop.y > FindPos_LayerLeftTop.y)
			{
				if(FindPos_Slice.x < FindPos_LayerSlice.x &&
					FindPos_Slice.y < FindPos_LayerSlice.y)
				_out.push_back(vecObjects[j]);
			}
		}
	}
}
