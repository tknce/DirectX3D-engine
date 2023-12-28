#include "pch.h"
#include "CSaveLoadMgr.h"


#include <Engine\CPathMgr.h>
#include <Engine\CLevel.h>
#include <Engine\CLayer.h>
#include <Engine\CGameObject.h>
#include <Engine\GlobalComponent.h>
#include <Engine\CCollisionMgr.h>
#include <Engine\CPrefab.h>
#include <Engine\CLevelMgr.h>
#include <Engine\CEventMgr.h>
#include <Engine\CKeyMgr.h>
#include <Engine/CAnimator2D.h>
#include <Engine/CAnimator3D.h>
#include <Engine/CAnimation2D.h>
#include <Engine/CPlayerMgr.h>
#include <Engine\CRigidBody3D.h>

#include <Script\CScriptMgr.h>
#include <Engine\CScript.h>


CSaveLoadMgr::CSaveLoadMgr()
	:m_iSaveIdx(0)
	, m_fAcctime(0)
	, m_iCurIdx(0)
	, m_bChangeLevel(false)
{
	wstring strFilePath = CPathMgr::GetInst()->GetContentPath();
	strFilePath += L"turnbacktime\\";
	filesystem::v1::create_directories(strFilePath);

	// ���
	wstring strFilePath1 = CPathMgr::GetInst()->GetContentPath();

	wstring Name;
	Name += L"turnbacktime\\";
	++m_iSaveIdx;
	Name += std::to_wstring(m_iSaveIdx);
	Name += L".a";

	m_vSave = Name;
	strFilePath1 += Name;
	// ���� ����
	m_pFile = nullptr;
	_wfopen_s(&m_pFile, strFilePath1.c_str(), L"wb");

	m_Timestate.recording = true;
	m_Timestate.play = false;
	m_Timestate.Rerecording = false;
}

CSaveLoadMgr::~CSaveLoadMgr()
{
	if(m_pFile != nullptr)
		fclose(m_pFile);

	wstring strFilePath = CPathMgr::GetInst()->GetContentPath();
	strFilePath += L"turnbacktime\\";
	filesystem::v1::path currentPath = strFilePath;
	filesystem::v1::remove_all(currentPath);
}


void CSaveLoadMgr::init()
{
	CPrefab::Save_GameObject_Func = &CSaveLoadMgr::SaveGameObject;
	CPrefab::Load_GameObject_Func = &CSaveLoadMgr::LoadGameObject;
	
}


void CSaveLoadMgr::tick()
{
	m_fAcctime += DT;
	float Time = 1 / 16;
	float skill = 1;

	if (m_Timestate.recording)
	{
		if(CLevelMgr::GetInst()->GetLevelState() == LEVEL_STATE::PLAY)
		{ 
			if (KEY_PRESSED(KEY::LSHIFT))
			{
				skill = 4;
			}
			if (m_fAcctime * skill > Time)
			{
				SaveTurnBackTime();
			}
		}
		
		m_fAcctime = 0;
	}
	

	if (m_Timestate.play)
	{
		if (CPlayerMgr::GetInst()->GetDead() == 1)
		{
			for (int i = 0; i < MAX_LAYER; ++i)
			{
				m_iSaveIdx -= 5;
				if (m_iSaveIdx >= 0)
				{
					vector<SAVEINSIDE> Ojbectinfo = m_vSavesave[m_iSaveIdx];
					for (size_t j = 0; j < Ojbectinfo.size(); ++j)
					{
						if (Ojbectinfo[j].Object->Animator2D() != nullptr && Ojbectinfo[j].Object->Animator2D()->GetCurAnim() != nullptr)
						{
							Ojbectinfo[j].Object->Transform()->SetRelativePos(Ojbectinfo[j].ObjectPos);
							Ojbectinfo[j].Object->Transform()->SetRelativeRotation(Ojbectinfo[j].ObjectDir);

							if (Ojbectinfo[j].ObjectIdx > Ojbectinfo[j].Object->Animator2D()->GetCurAnim()->GetAnimFrm().size())
							{
								Ojbectinfo[j].ObjectIdx = Ojbectinfo[j].Object->Animator2D()->GetCurAnim()->GetAnimFrm().size() - 1;
							}
							Ojbectinfo[j].Object->Animator2D()->Play_Load(Ojbectinfo[j].ObjectAniName, Ojbectinfo[j].ObjectIdx);
						}
					}
				}
				else
				{
					m_Timestate.play = false;
					m_Timestate.Rerecording = true;
					m_vSavesave.clear();
					CPlayerMgr::GetInst()->SetDead(0);
					CPlayerMgr::GetInst()->Setreturn(false);
				}
				m_fAcctime = 0;
			}
		}
		else
		{
			for (int i = 0; i < MAX_LAYER; ++i)
			{
				++m_iCurIdx;
				if (m_iCurIdx < m_iSaveIdx)
				{
					vector<SAVEINSIDE> Ojbectinfo = m_vSavesave[m_iCurIdx];
					for (size_t j = 0; j < Ojbectinfo.size(); ++j)
					{
						if (Ojbectinfo[j].Object->Animator2D() != nullptr && Ojbectinfo[j].Object->Animator2D()->GetCurAnim() != nullptr)
						{
							Ojbectinfo[j].Object->Transform()->SetRelativePos(Ojbectinfo[j].ObjectPos);
							Ojbectinfo[j].Object->Transform()->SetRelativeRotation(Ojbectinfo[j].ObjectDir);

							if (Ojbectinfo[j].ObjectIdx > Ojbectinfo[j].Object->Animator2D()->GetCurAnim()->GetAnimFrm().size())
							{
								Ojbectinfo[j].ObjectIdx = Ojbectinfo[j].Object->Animator2D()->GetCurAnim()->GetAnimFrm().size() - 1;
							}
							Ojbectinfo[j].Object->Animator2D()->Play_Load(Ojbectinfo[j].ObjectAniName, Ojbectinfo[j].ObjectIdx);
						}
					}
				}
				else
				{
					m_Timestate.play = false;
					m_Timestate.Rerecording = true;
					m_iCurIdx = 0;
					m_iSaveIdx = 0;
					m_vSavesave.clear();
					CPlayerMgr::GetInst()->SetDead(0);
					CPlayerMgr::GetInst()->Setreturn(false);
				}
				m_fAcctime = 0;
			}
		}
		
	}
	else
	{
		m_fAcctime = 0;
	}



	if (m_Timestate.Rerecording)
	{
		wstring strFilePath = CPathMgr::GetInst()->GetContentPath();
		strFilePath += L"turnbacktime\\";
		filesystem::v1::path currentPath = strFilePath;
		filesystem::v1::remove_all(currentPath);

		strFilePath = CPathMgr::GetInst()->GetContentPath();
		strFilePath += L"turnbacktime\\";
		filesystem::v1::create_directories(strFilePath);

		// ���
		strFilePath = CPathMgr::GetInst()->GetContentPath();

		wstring Name;
		Name += L"turnbacktime\\";
		++m_iSaveIdx;
		Name += std::to_wstring(m_iSaveIdx);
		Name += L".a";

		m_vSave = Name;
		strFilePath += Name;
		// ���� ����
		m_pFile = nullptr;
		_wfopen_s(&m_pFile, strFilePath.c_str(), L"wb");
		m_Timestate.Rerecording = false;
		m_Timestate.recording = true;
	}

		
}







void CSaveLoadMgr::SaveLevel(CLevel* _Level, wstring _strRelativePath)
{
	assert(_Level);

	// ���
	wstring strFilePath = CPathMgr::GetInst()->GetContentPath();
	strFilePath += _strRelativePath;

	// ���� ����
	FILE* pFile = nullptr;
	_wfopen_s(&pFile, strFilePath.c_str(), L"wb");

	// ���� �̸� ����
	_Level->SaveToFile(pFile);

	WORD m_matWord[MAX_LAYER];
	int MAX = MAX_LAYER;
	for (int iRow = 0; iRow < MAX_LAYER; ++iRow)
	{
		m_matWord[iRow] = CCollisionMgr::GetInst()->GetCollisionLayer(iRow);
		fwrite(&iRow, sizeof(int), 1, pFile);
		for (int iCol = iRow; iCol < MAX_LAYER; ++iCol)
		{
			
			if (m_matWord[iRow] & (1 << iCol))
			{
				fwrite(&iCol, sizeof(int), 1, pFile);
			}
		}
		fwrite(&MAX, sizeof(int), 1, pFile);
	}
	++MAX;
	fwrite(&MAX, sizeof(int), 1, pFile);
	// ������ ���̾� ����
	for (UINT i = 0; i < MAX_LAYER; ++i)
	{
		// ���̾� �̸� ����
		CLayer* pLayer = _Level->GetLayer(i);
		pLayer->SaveToFile(pFile);

		// ���̾ �ִ� �θ� ������Ʈ ����
		const vector<CGameObject*>& vecParentObj = pLayer->GetParentObjects();

		size_t iObjectCount = vecParentObj.size();
		fwrite(&iObjectCount, sizeof(size_t), 1, pFile);

		for (size_t j = 0; j < vecParentObj.size(); ++j)
		{
			SaveGameObject(vecParentObj[j], pFile);
		}
	}

	// �浹 �Ŵ��� ���� ��
	//CCollisionMgr::GetInst()->SaveToFile(pFile);
	fclose(pFile);



}

void CSaveLoadMgr::SaveGameObject(CGameObject* _Object, FILE* _File)
{
	// ������Ʈ �̸� ����
	_Object->SaveToFile(_File);
	int LayerIdx = _Object->GetLayerIdx();
	fwrite(&LayerIdx, sizeof(int), 1, _File);

	// ������Ʈ ���� ������Ʈ ����
	for (UINT i = 0; i < (UINT)COMPONENT_TYPE::END; ++i)
	{
		CComponent* pComponent = _Object->GetComponent((COMPONENT_TYPE)i);

		if (nullptr != pComponent)
		{
			pComponent->SaveToFile(_File);
		}
	}

	COMPONENT_TYPE ComponentEnd = COMPONENT_TYPE::END;
	fwrite(&ComponentEnd, sizeof(UINT), 1, _File);


	// Script ����, �� Script �̸� ����
	const vector<CScript*>& vecScript = _Object->GetScripts();
	size_t iScriptCount = vecScript.size();
	fwrite(&iScriptCount, sizeof(size_t), 1, _File);

	for (size_t i = 0; i < vecScript.size(); ++i)
	{
		SaveWStringToFile(CScriptMgr::GetScriptName(vecScript[i]), _File);
		vecScript[i]->SaveToFile(_File);
	}


	// �ڽ� ������Ʈ
	const vector<CGameObject*> vecChild = _Object->GetChildObject();

	size_t iChildCount = vecChild.size();
	fwrite(&iChildCount, sizeof(size_t), 1, _File);

	for (size_t i = 0; i < iChildCount; ++i)
	{
		SaveGameObject(vecChild[i], _File);
	}
}




CLevel* CSaveLoadMgr::LoadLevel(wstring _strRelativePath)
{
	// ���
	wstring strFilePath = CPathMgr::GetInst()->GetContentPath();
	strFilePath += _strRelativePath;

	// ���� �б�
	FILE* pFile = nullptr;
	_wfopen_s(&pFile, strFilePath.c_str(), L"rb");

	// ���� �̸�
	CLevel* pLevel = new CLevel;
	pLevel->LoadFromFile(pFile);

	while (true)
	{
		int iRow = 0;
		fread(&iRow, sizeof(int), 1, pFile);
		if (iRow == (MAX_LAYER + 1))
			break;

		for (int iCol = 0; iCol < MAX_LAYER; ++iCol)
		{
			fread(&iCol, sizeof(int), 1, pFile);
			if (iCol == MAX_LAYER)
			{
				break;
			}
			CCollisionMgr::GetInst()->CollisionLayerCheck(iRow, iCol);
		}
	}
	//CLevelMgr::GetInst()->SetCurLevel(pLevel);
	// ������ ���̾�
	for (UINT i = 0; i < MAX_LAYER; ++i)
	{
		// ���̾� �̸�
		CLayer* pLayer = pLevel->GetLayer(i);
		pLayer->LoadFromFile(pFile);

		// ���̾ �ִ� �θ� ������Ʈ 
		size_t iObjectCount = 0;
		fread(&iObjectCount, sizeof(size_t), 1, pFile);

		for (size_t j = 0; j < iObjectCount; ++j)
		{
			CGameObject* pObject = LoadGameObject(pFile);
			pLayer->AddGameObject(pObject);
		}
	}

	// �浹 �Ŵ��� ���� ��
	//CCollisionMgr::GetInst()->SaveToFile(pFile);


	fclose(pFile);

	pLevel->SetName(_strRelativePath);

	CEventMgr::GetInst()->IsLevelChanged();

	return pLevel;
}

void CSaveLoadMgr::SaveTurnBackTime()
{
	++m_iSaveIdx;

	for (size_t i = 0; i < MAX_LAYER; ++i)
	{
		// ��� ������Ʈ�� �����´�
		vector<CGameObject*> Objects = CLevelMgr::GetInst()->GetCurLevel()->GetLayer(i)->GetParentObjects();
		size_t Size = Objects.size();
		// ������Ʈ ���� ��ŭ �����Ѵ�
		fwrite(&Size, sizeof(size_t), 1, m_pFile);
		int isAni = 0;
		for (size_t j = 0; j < Objects.size(); ++j)
		{
			// ������Ʈ �̸� ����

			SaveWStringToFile(Objects[j]->GetName(), m_pFile);
			// ������Ʈ ��ġ�� ����
			Objects[j]->Transform()->SaveTurnBackTime(m_pFile);
			// ������Ʈ �ִϸ��̼��� �ִٸ� �ִϸ��̼� ����
			if (Objects[j]->Animator2D() != nullptr && Objects[j]->Animator2D()->GetCurAnim() != nullptr)
			{
				isAni = 1;
				fwrite(&isAni, sizeof(int), 1, m_pFile);
				Objects[j]->Animator2D()->TurnbacktimeSave(m_pFile);
			}
			else
			{
				isAni = 0;
				fwrite(&isAni, sizeof(int), 1, m_pFile);
			}
			
		}

	}
	if (m_Timestate.play)
	{
		m_Timestate.recording = false;
		fclose(m_pFile);
		LoadTurnBackTime();
	}
	if (m_bChangeLevel)
	{
		fclose(m_pFile);
		wstring strFilePath = CPathMgr::GetInst()->GetContentPath();
		strFilePath += L"turnbacktime\\";
		filesystem::v1::create_directories(strFilePath);

		// ���
		wstring strFilePath1 = CPathMgr::GetInst()->GetContentPath();

		wstring Name;
		Name += L"turnbacktime\\";
		++m_iSaveIdx;
		Name += std::to_wstring(m_iSaveIdx);
		Name += L".a";

		m_vSave = Name;
		strFilePath1 += Name;

		m_bChangeLevel = false;

		m_pFile = nullptr;
		_wfopen_s(&m_pFile, strFilePath1.c_str(), L"wb");
	}
}

void CSaveLoadMgr::LoadTurnBackTime()
{

	// ���
	wstring strFilePath = CPathMgr::GetInst()->GetContentPath();
	strFilePath += m_vSave;

	// ���� �б�
	FILE* pFile = nullptr;
	_wfopen_s(&pFile, strFilePath.c_str(), L"rb");
	wstring ObjectName;
	vector<SAVEINSIDE> SaveInside;
	while (true)
	{
		--m_iSaveIdx;
		if (m_iSaveIdx == 0)
			break;
		for (size_t i = 0; i < MAX_LAYER; ++i)
		{
			size_t Size;
			SaveInside.clear();
			fread(&Size, sizeof(size_t), 1, pFile);
			for (size_t j = 0; j < Size; ++j)
			{
				LoadWStringFromFile(ObjectName, pFile);
				SAVEINSIDE Inside;

				Inside.Object = CLevelMgr::GetInst()->FindObjectByName(ObjectName);
				Vec3 Pos;
				fread(&Pos, sizeof(Vec3), 1, pFile);
				Inside.ObjectPos = Pos;
				fread(&Pos, sizeof(Vec3), 1, pFile);
				Inside.ObjectDir = Pos;
				int isAni;
				fread(&isAni, sizeof(int), 1, m_pFile);
				if (isAni == 1)
				{
					LoadWStringFromFile(Inside.ObjectAniName, m_pFile);
					fread(&Inside.ObjectIdx, sizeof(int), 1, m_pFile);
				}
				else
				{

				}
				

				SaveInside.push_back(Inside);

			}
			m_vSavesave.push_back(SaveInside);
		}
		
	}
	m_iSaveIdx = m_vSavesave.size();
	fclose(pFile);
	
}



CGameObject* CSaveLoadMgr::LoadGameObject(FILE* _File)
{
	CGameObject* pObject = new CGameObject;

	// ������Ʈ �̸�
	pObject->LoadFromFile(_File);

	int LayerIdx = 0;
	fread(&LayerIdx, sizeof(int), 1, _File);
	pObject->SetLayerIdx(LayerIdx);

	// ������Ʈ ���� ������Ʈ
	bool bProgress = true;
	while (true)
	{
		// Component Ÿ�� Ȯ��
		COMPONENT_TYPE type = COMPONENT_TYPE::END;
		fread(&type, sizeof(UINT), 1, _File);

		if (type == COMPONENT_TYPE::AI)
			type = COMPONENT_TYPE::END;


		
		// Ÿ�Կ� �ش��ϴ� ������Ʈ �ε� �� �߰�
		CComponent* pComponent = nullptr;

		switch (type)
		{
		case COMPONENT_TYPE::TRANSFORM:
			pComponent = new CTransform;
			break;

		case COMPONENT_TYPE::CAMERA:
			pComponent = new CCamera;
			break;

		case COMPONENT_TYPE::COLLIDER2D:
			pComponent = new CCollider2D;
			break;

		case COMPONENT_TYPE::ANIMATOR2D:
			pComponent = new CAnimator2D;
			break;

		case COMPONENT_TYPE::LIGHT2D:
			pComponent = new CLight2D;
			break;

		case COMPONENT_TYPE::RIGIDBODY2D:
			pComponent = new CRigidBody2D;
			break;

		case COMPONENT_TYPE::RIGIDBODY3D:
			pComponent = new CRigidBody3D;
			break;

		case COMPONENT_TYPE::MESHRENDER:
			pComponent = new CMeshRender;
			break;

		case COMPONENT_TYPE::TILEMAP:
			pComponent = new CTileMap;
			break;

		case COMPONENT_TYPE::PARTICLESYSTEM:
			pComponent = new CParticleSystem;
			break;

		case COMPONENT_TYPE::LIGHT3D:
			pComponent = new CLight3D;
			break;
		case COMPONENT_TYPE::COLLIDER3D:
			break;
		case COMPONENT_TYPE::ANIMATOR3D:
			pComponent = new CAnimator3D;
			break;
		case COMPONENT_TYPE::SKYBOX:
			pComponent = new CSkyBox;
			break;
		case COMPONENT_TYPE::DECAL:
			pComponent = new CDecal;
			break;
		case COMPONENT_TYPE::LANDSCAPE:
			pComponent = new CLandScape;
			break;
		case COMPONENT_TYPE::AI:
			pComponent = new AI;
			break;
		case COMPONENT_TYPE::END:
			bProgress = false;
			break;
		}

		if (!bProgress)
			break;

		if (pComponent != nullptr)
		{
			pComponent->LoadFromFile(_File);
			pObject->AddComponent(pComponent);
		}
	}


	// Script
	size_t iScriptCount = 0;
	fread(&iScriptCount, sizeof(size_t), 1, _File);

	for (size_t i = 0; i < iScriptCount; ++i)
	{
		wstring strScriptName;
		LoadWStringFromFile(strScriptName, _File);
		CScript* pNewScript = CScriptMgr::GetScript(strScriptName);
		pNewScript->LoadFromFile(_File);

		pObject->AddComponent(pNewScript);
	}


	// �ڽ� ������Ʈ
	size_t iChildCount = 0;
	fread(&iChildCount, sizeof(size_t), 1, _File);

	for (size_t i = 0; i < iChildCount; ++i)
	{
		CGameObject* pChild = LoadGameObject(_File);
		pObject->AddChild_LOAD(pChild);
	}

	if (pObject->Animator3D() != nullptr)
		pObject->Animator3D()->SetMeshData();


	return pObject;
}