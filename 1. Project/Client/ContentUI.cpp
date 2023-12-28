#include "pch.h"
#include "ContentUI.h"

#include <Engine/CResMgr.h>
#include <Engine/CKeyMgr.h>
#include <Engine/CPathMgr.h>
#include <Engine/CTimeMgr.h>
#include <Engine/CEventMgr.h>
#include <Engine/CSound.h>
#include <Engine/CLevelMgr.h>
#include <Engine/CLevel.h>
#include <Engine/CGameObject.h>
#include <Engine/CTransform.h>

#include "TreeUI.h"
#include "CImGuiMgr.h"
#include "InspectorUI.h"
#include "CSaveLoadMgr.h"

ContentUI::ContentUI()
	: UI("Content")
	, m_Tree(nullptr)
	, m_Ins(false)
{
	m_Tree = new TreeUI("##ContentTree");
	AddChild(m_Tree);

	//Ʈ�� ����
	m_Tree->ShowSeperator(false);
	m_Tree->SetDummyRoot(true);

	// Tree�� Delegate ���
	m_Tree->AddDynamic_Selected(this, (FUNC_1)&ContentUI::SetResourcetoInspector);
	
	m_Tree->AddDynamic_Instiate(this, (FUNC_1_2)&ContentUI::InstantiatePrefab);

	// ���� ���ҽ� ���¸� ����
	ResetContent();
}

ContentUI::~ContentUI()
{
}

void ContentUI::update()
{
	/*if (KEY_TAP(KEY::I))
	{
		CSaveLoadMgr::GetInst()->SaveTurnBackTime();
	}*/


	if (CResMgr::GetInst()->IsChanged())
	{
		ResetContent();
	}

	if (KEY_TAP(KEY::LBTN))
	{
		if (m_Prefab != nullptr)
		{
			if (m_Ins)
			{
				Vec2 ObjectPos = CKeyMgr::GetInst()->GetMousePos();
				Instantiate(m_Prefab, m_Prefab->Transform()->GetRelativePos(), 4);
				m_Ins = false;
			}
			
		}
	}



	if (KEY_PRESSED(KEY::LCTRL) && KEY_TAP(KEY::S))
	{
		TreeNode* pSelectedNode = m_Tree->GetSelectedNode();
		if (nullptr != pSelectedNode)
		{
			RES_TYPE type = ((CRes*)pSelectedNode->GetData())->GetResType();

			wstring strRelativePath;
			wstring Path;
			if (RES_TYPE::MATERIAL == type)
			{
				Ptr<CMaterial> pRes = (CMaterial*)pSelectedNode->GetData();
				
				//strRelativePath = L"material//";
				//Path = strRelativePath;
				Path += pRes.Get()->GetName();
				pRes.Get()->SetName(Path);
				
				strRelativePath += pRes->GetKey();
				//strRelativePath += L".mtrl";
				pRes->Save(strRelativePath);
			}
			if (RES_TYPE::PREFAB == type)
			{
				Ptr<CPrefab> pRes = (CPrefab*)pSelectedNode->GetData();

				strRelativePath = L"prefab//";
				strRelativePath += pRes->GetKey();
				strRelativePath += L".pref";
				pRes->Save(strRelativePath);
			}
		}

	}

	UI::update();
}

void ContentUI::render_update()
{
}

void ContentUI::ResetContent()
{
	m_Tree->Clear();

	TreeNode* pRootNode = m_Tree->AddItem(nullptr, "Content", 0);

	for (UINT i = 0; i < (UINT)RES_TYPE::END; ++i)
	{
		TreeNode* pResNode = m_Tree->AddItem(pRootNode, ToString((RES_TYPE)i), 0, true);

		const map<wstring, Ptr<CRes>>& mapRes = CResMgr::GetInst()->GetResource((RES_TYPE)i);

		map<wstring, Ptr<CRes>>::const_iterator iter = mapRes.begin();
		for (; iter != mapRes.end(); ++iter)
		{
			wstring strName = iter->first;
			m_Tree->AddItem(pResNode, string(strName.begin(), strName.end()), (DWORD_PTR)iter->second.Get());
		}
	}



}

void ContentUI::ReloadContent()
{
	// Content ������ �ִ� ��� ���ҽ����� �˻� �� �ε�
	wstring strFolderPath = CPathMgr::GetInst()->GetContentPath();
	FindContentFileName(strFolderPath);

	// ���ϸ��� Ȯ���ڸ� Ȯ�� �� �˸��� ���ҽ��� �������� �˾Ƴ���.
	for (size_t i = 0; i < m_vecContentName.size(); ++i)
	{
		RES_TYPE resType = GetResTypeByExt(m_vecContentName[i]);

		// ���ҽ� Ÿ�Կ� �ش��ϴ� ���ҽ��� ��ηκ��� �ε�
		if (RES_TYPE::END == resType)
			continue;

		switch (resType)
		{
		case RES_TYPE::PREFAB:
			CResMgr::GetInst()->Load<CPrefab>(m_vecContentName[i], m_vecContentName[i]);
			break;
		case RES_TYPE::MESHDATA:
			CResMgr::GetInst()->Load<CMeshData>(m_vecContentName[i], m_vecContentName[i]);
			break;
		case RES_TYPE::MATERIAL:
			CResMgr::GetInst()->Load<CMaterial>(m_vecContentName[i], m_vecContentName[i]);
			break;
		case RES_TYPE::MESH:
			CResMgr::GetInst()->Load<CMesh>(m_vecContentName[i], m_vecContentName[i]);
			break;
		case RES_TYPE::TEXTURE:
			CResMgr::GetInst()->Load<CTexture>(m_vecContentName[i], m_vecContentName[i]);
			break;
		case RES_TYPE::SOUND:
			CResMgr::GetInst()->Load<CSound>(m_vecContentName[i], m_vecContentName[i]);
			break;
		}
	}

	// �ε��� ���ҽ��� ���� ���Ϸ� �����ϴ� Ȯ��
	for (UINT i = 0; i < (UINT)RES_TYPE::END; ++i)
	{
		const map<wstring, Ptr<CRes>>& mapRes = CResMgr::GetInst()->GetResource((RES_TYPE)i);

		map<wstring, Ptr<CRes>>::const_iterator iter = mapRes.begin();
		for (iter; iter != mapRes.end(); ++iter)
		{
			// Engine Res Ÿ���̸� Ȯ������ �ʴ´�.
			if (iter->second->IsEngineRes())
			{
				continue;
			}

			wstring strRelativePath = iter->second->GetRelativePath();
			assert(!strRelativePath.empty());

			// �޸𸮿� �ε��� ���ҽ��� �ش��ϴ� ���� ������ �������� �ʴ� ���
			if (!filesystem::exists(strFolderPath + strRelativePath))
			{
				// ���ҽ� �Ŵ������� �ش� ���ҽ��� �����Ѵ�.
				if (iter->second->GetRefCount() <= 1)
				{
					// ���� ����
					tEvent evn = {  };
					evn.eType = EVENT_TYPE::DELETE_RES;
					evn.wParam = i;
					evn.lParam = (DWORD_PTR)(iter->second.Get());

					CEventMgr::GetInst()->AddEvent(evn);

					MessageBox(nullptr, L"���� ����", L"���ҽ� ���� Ȯ��", MB_OK);
				}
				else
				{
					// �ش� ���ҽ��� �����ϴ� ��ü�� ����	
					MessageBox(nullptr, L"��� �� �� ���ҽ�/n���ҽ� ���� ����", L"���ҽ� ���� Ȯ��", MB_OK);
				}
			}
		}
	}

}

void ContentUI::SetResourcetoInspector(DWORD_PTR _res)
{
	// _res : Ŭ���� ���
	TreeNode* pSelectedNode = (TreeNode*)_res;
	CRes* pRes = (CRes*)pSelectedNode->GetData();

	// InspectorUI �� Ŭ���� Resouce �� �˷��ش�.
	InspectorUI* Inspector = (InspectorUI*)CImGuiMgr::GetInst()->FindUI("Inspector");

	Inspector->SetTargetResource(pRes);
}

void ContentUI::InstantiatePrefab(DWORD_PTR _res)
{
	TreeNode* pSelectedNode = (TreeNode*)_res;
	
	string Name = pSelectedNode->GetName();
	wstring PrefabName = wstring(Name.begin(), Name.end());




	Ptr<CPrefab> Prefab = CResMgr::GetInst()->FindRes<CPrefab>(PrefabName);
	m_Ins = true;
	m_Prefab = Prefab->Instantiate();
}

void ContentUI::FindContentFileName(const wstring& _strFolderPath)
{

	// ��� ���ϸ��� �˾Ƴ���.
	wstring strFolderPath = _strFolderPath + L"*.*";

	HANDLE hFindHandle = nullptr;

	WIN32_FIND_DATA data = {};

	// Ž���� �ڵ� ���
	hFindHandle = FindFirstFile(strFolderPath.c_str(), &data);

	if (INVALID_HANDLE_VALUE == hFindHandle)
		return;

	// Ž�� �ڵ��� �̿��ؼ� ��� ������ �� Ȯ���� �� ���� �ݺ�
	while (FindNextFile(hFindHandle, &data))
	{
		if ((FILE_ATTRIBUTE_DIRECTORY & data.dwFileAttributes) && wcscmp(data.cFileName, L".."))
		{
			FindContentFileName(_strFolderPath + data.cFileName + L"\\");
		}

		else
		{
			wstring strRelative = GetRelativePath(CPathMgr::GetInst()->GetContentPath(), _strFolderPath + data.cFileName);
			m_vecContentName.push_back(strRelative);
		}
	}

	FindClose(hFindHandle);

	//// ��� ���ϸ��� �˾Ƴ���.
	//wstring strFolderPath = _strFolderPath + L"*.*";

	//HANDLE hFindHandle = nullptr;

	//WIN32_FIND_DATA data = {};

	//// Ž���� �ڵ� ���
	//hFindHandle = FindFirstFile(strFolderPath.c_str(), &data);

	//// ���� ��Ȯ�� ������ ã�Ƴ��� �ʹٸ� �޼����ڽ� ����
	//if (INVALID_HANDLE_VALUE == hFindHandle)
	//	return;

	//// Ž�� �ڵ��� �̿��ؼ� ��� ������ �� Ȯ���� �� ���� �ݺ�
	//while (FindNextFile(hFindHandle, &data))
	//{
	//	if (FILE_ATTRIBUTE_DIRECTORY == data.dwFileAttributes && wcscmp(data.cFileName, L".."))
	//	{
	//		FindContentFileName(_strFolderPath + data.cFileName + L"\\");
	//	}

	//	else
	//	{
	//		wstring strRelative = GetRelativePath(CPathMgr::GetInst()->GetContentPath(), _strFolderPath + data.cFileName);
	//		m_vecContentName.push_back(strRelative);
	//	}

	//	
	//}

	//CloseHandle(hFindHandle);
}

RES_TYPE ContentUI::GetResTypeByExt(wstring _filename)
{
	wstring strExt = path(_filename.c_str()).extension();

	if (strExt == L".pref")
		return RES_TYPE::PREFAB;
	else if (strExt == L".mdat")
		return RES_TYPE::MESHDATA;
	else if (strExt == L".mtrl")
		return RES_TYPE::MATERIAL;
	else if (strExt == L".mesh")
		return RES_TYPE::MESH;
	else if (strExt == L".dds" || strExt == L".DDS" || strExt == L".tga" || strExt == L".TGA"
		|| strExt == L".png" || strExt == L".jpg" || strExt == L".JPG" || strExt == L".jegp" || strExt == L".JPEG"
		|| strExt == L".bmp" || strExt == L".BMP")
		return RES_TYPE::TEXTURE;
	else if (strExt == L".mp3" || strExt == L".MP3" || strExt == L".wav" || strExt == L".WAV" || strExt == L".ogg" || strExt == L".OGG")
		return RES_TYPE::SOUND;
	else
		return RES_TYPE::END;
}





