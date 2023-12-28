#include "pch.h"
#include "CMeshData.h"


#include "CPathMgr.h"
#include "CResMgr.h"

#include "CGameObject.h"
#include "CTransform.h"
#include "CMeshRender.h"
#include "CAnimator3D.h"

#include "CFBXLoader.h"




CMeshData::CMeshData(bool _bEngine)
	: CRes(RES_TYPE::MESHDATA, _bEngine)
{
}

CMeshData::~CMeshData()
{
}


CGameObject* CMeshData::Instantiate()
{
	CGameObject* pNewObj = new CGameObject;
	pNewObj->AddComponent(new CTransform);
	pNewObj->AddComponent(new CMeshRender);

	pNewObj->MeshRender()->SetMesh(m_pMesh);

	for (UINT i = 0; i < m_vecMtrl.size(); ++i)
	{
		pNewObj->MeshRender()->SetSharedMaterial(m_vecMtrl[i], i);
	}


	// Animation ��Ʈ �߰�
	if (false == m_pMesh->IsAnimMesh())
		return pNewObj;

	CAnimator3D* pAnimator = new CAnimator3D;
	pNewObj->AddComponent(pAnimator);
	pAnimator->SetResMeshData(GetRelativePath());
	pAnimator->SetBones(m_pMesh->GetBones());
	pAnimator->SetAnimClip(m_pMesh->GetAnimClip());


	
	return pNewObj;
}




//vector<CMeshData*> CMeshData::LoadFromFBX(const wstring& _strPath)
//{
//	wstring strFullPath = CPathMgr::GetInst()->GetContentPath();
//	strFullPath += _strPath;
//
//	CFBXLoader loader;
//	loader.init();
//	loader.LoadFbx(strFullPath);
//
//	// �޽� ��������
//	CMesh* pMesh = nullptr;
//	vector<Ptr<CMaterial>> vecMtrl;
//	vector<CMeshData*> data;
//	CMeshData* pMeshData = new CMeshData(true);
//	for (UINT i = 0; i < loader.GetContainerCount(); ++i)
//	{
//		pMesh = CMesh::CreateFromContainer(loader, i);
//
//		// ResMgr �� �޽� ���
//		if (nullptr != pMesh)
//		{
//			wstring strMeshKey = L"mesh\\";
//			strMeshKey += path(strFullPath).stem();
//			strMeshKey += std::to_wstring(i);
//			strMeshKey += L".mesh";
//			CResMgr::GetInst()->AddRes<CMesh>(strMeshKey, pMesh);
//
//			// �޽ø� ���� ���Ϸ� ����
//			pMesh->Save(strMeshKey);
//		}
//		for (UINT j = 0; j < loader.GetContainer(i).vecMtrl.size(); ++j)
//		{
//			// ����ó�� (material �̸��� �Է� �ȵǾ����� ���� �ִ�.)
//			Ptr<CMaterial> pMtrl = CResMgr::GetInst()->FindRes<CMaterial>(loader.GetContainer(i).vecMtrl[j].strMtrlName);
//			assert(pMtrl.Get());
//
//			vecMtrl.push_back(pMtrl);
//		}
//		pMeshData->m_pMesh = pMesh;
//		pMeshData->m_vecMtrl = vecMtrl;
//		data.push_back(pMeshData);
//		vecMtrl.clear();
//	}
//
//	
//
//	// ���׸��� ��������
//	//for (UINT i = 0; i < loader.GetContainerCount(); ++i)
//	//{
//	//	for (UINT j = 0; j < loader.GetContainer(i).vecMtrl.size(); ++j)
//	//	{
//	//		// ����ó�� (material �̸��� �Է� �ȵǾ����� ���� �ִ�.)
//	//		Ptr<CMaterial> pMtrl = CResMgr::GetInst()->FindRes<CMaterial>(loader.GetContainer(0).vecMtrl[j].strMtrlName);
//	//		assert(pMtrl.Get());
//
//	//		vecMtrl.push_back(pMtrl);
//	//	}
//	//}
//	//for (UINT i = 0; i < loader.GetContainer(i).vecMtrl.size(); ++i)
//	//{
//	//	// ����ó�� (material �̸��� �Է� �ȵǾ����� ���� �ִ�.)
//	//	Ptr<CMaterial> pMtrl = CResMgr::GetInst()->FindRes<CMaterial>(loader.GetContainer(0).vecMtrl[i].strMtrlName);
//	//	assert(pMtrl.Get());
//
//	//	vecMtrl.push_back(pMtrl);
//	//}
//
//	
//
//
//	return data;
//}


CMeshData* CMeshData::LoadFromFBX(const wstring& _strPath)
{
	wstring strFullPath = CPathMgr::GetInst()->GetContentPath();
	strFullPath += _strPath;

	CFBXLoader loader;
	loader.init();
	loader.LoadFbx(strFullPath);

	// �޽� ��������
	CMesh* pMesh = nullptr;
	CMeshData* pMeshData = nullptr;
	vector<Ptr<CMaterial>> vecMtrl;
	vector<CMeshData*> data;
	for (int i = 0; i < loader.GetContainerCount(); ++i)
	{
		pMesh = CMesh::CreateFromContainer(loader, i);
		// ResMgr �� �޽� ���
		if (nullptr != pMesh)
		{
			wstring strMeshKey = L"mesh\\";
			strMeshKey += path(strFullPath).stem();
			if(i != 0)
			strMeshKey += L"_" + std::to_wstring(i);
			strMeshKey += L".mesh";
			CResMgr::GetInst()->AddRes<CMesh>(strMeshKey, pMesh);

			// �޽ø� ���� ���Ϸ� ����
			pMesh->Save(strMeshKey);
		}
		// ���׸��� ��������
		for (UINT j = 0; j < loader.GetContainer(i).vecMtrl.size(); ++j)
		{
			// ����ó�� (material �̸��� �Է� �ȵǾ����� ���� �ִ�.)
			Ptr<CMaterial> pMtrl = CResMgr::GetInst()->FindRes<CMaterial>(loader.GetContainer(i).vecMtrl[j].strMtrlName);
			assert(pMtrl.Get());

			vecMtrl.push_back(pMtrl);
		}

		pMeshData = new CMeshData(true);
		pMeshData->m_pMesh = pMesh;
		pMeshData->m_vecMtrl = vecMtrl;
		data.push_back(pMeshData);
		vecMtrl.clear();
		pMesh = nullptr;
	}
	//wstring strMeshKey = L"mesh\\";
	//strMeshKey += path(strFullPath).stem();
	//strMeshKey += L".mesh";
	//Ptr<CMesh> meshptr;
	//meshptr = CResMgr::GetInst()->FindRes<CMesh>(strMeshKey);

	//pMesh = meshptr.Get();
	

	CResMgr::GetInst()->MeshDataSave(data, _strPath);

	return pMeshData;
}

void CMeshData::Save(const wstring& _strRelativePath)
{
	SetRelativePath(_strRelativePath);

	wstring strFilePath = CPathMgr::GetInst()->GetContentPath() + _strRelativePath;

	FILE* pFile = nullptr;
	errno_t err = _wfopen_s(&pFile, strFilePath.c_str(), L"wb");
	assert(pFile);

	// Mesh Key ����	
	// Mesh Data ����
	SaveResourceRef<CMesh>(m_pMesh, pFile);

	// material ���� ����
	UINT iMtrlCount = (UINT)m_vecMtrl.size();
	fwrite(&iMtrlCount, sizeof(UINT), 1, pFile);

	UINT i = 0;
	wstring strMtrlPath = CPathMgr::GetInst()->GetContentPath();
	strMtrlPath += L"material\\";

	for (; i < iMtrlCount; ++i)
	{
		if (nullptr == m_vecMtrl[i])
			continue;

		// Material �ε���, Key, Path ����
		fwrite(&i, sizeof(UINT), 1, pFile);
		SaveResourceRef<CMaterial>(m_vecMtrl[i], pFile);
	}

	i = -1; // ���� ��
	fwrite(&i, sizeof(UINT), 1, pFile);

	fclose(pFile);
}

int CMeshData::Load(const wstring& _strFilePath)
{
	FILE* pFile = NULL;
	_wfopen_s(&pFile, _strFilePath.c_str(), L"rb");

	assert(pFile);

	// Mesh Load
	LoadResourceRef<CMesh>(m_pMesh, pFile);
	assert(m_pMesh.Get());

	// material ���� �б�
	UINT iMtrlCount = 0;
	fread(&iMtrlCount, sizeof(UINT), 1, pFile);

	m_vecMtrl.resize(iMtrlCount);

	for (UINT i = 0; i < iMtrlCount; ++i)
	{
		UINT idx = -1;
		fread(&idx, 4, 1, pFile);
		if (idx == -1)
			break;

		wstring strKey, strPath;

		Ptr<CMaterial> pMtrl;
		LoadResourceRef<CMaterial>(pMtrl, pFile);

		m_vecMtrl[i] = pMtrl;
	}

	fclose(pFile);

	return S_OK;
}

