#include "pch.h"
#include "CResMgr.h"

CResMgr::CResMgr()
	: m_iLayoutOffset_0(0)
	, m_iLayoutOffset_1(0)
	, m_bChanged(false)
{

}

CResMgr::~CResMgr()
{
	for (UINT i = 0; i < (UINT)RES_TYPE::END; ++i)
	{
		map<wstring, Ptr<CRes>>::iterator iter = m_arrRes[i].begin();

		for (; iter != m_arrRes[i].end(); ++iter)
		{
			iter->second = nullptr;
		}
	}

	CSound::g_pFMOD->release();
}


Ptr<CTexture> CResMgr::CreateTexture(const wstring& _strKey, UINT _iWidth, UINT _iHeight, DXGI_FORMAT _eFormat, UINT _iBindFlag, bool _Cpu)
{
	m_bChanged = true;
	Ptr<CTexture> pTex = FindRes<CTexture>(_strKey);
	assert(!pTex.Get());

	pTex = new CTexture(true);
	if (_Cpu)
	{
		pTex->Create(_iWidth, _iHeight, _eFormat, _iBindFlag, true);
	}
	else
	{
		pTex->Create(_iWidth, _iHeight, _eFormat, _iBindFlag);
	}
	

	AddRes<CTexture>(_strKey, pTex.Get());

	return pTex;
}

Ptr<CTexture> CResMgr::CreateTexture(const wstring& _strKey, ComPtr<ID3D11Texture2D> _Tex2D)
{
	m_bChanged = true;
	Ptr<CTexture> pTex = FindRes<CTexture>(_strKey);
	assert(!pTex.Get());

	pTex = new CTexture(true);
	pTex->Create(_Tex2D);

	AddRes<CTexture>(_strKey, pTex.Get());

	return pTex;
}



void CResMgr::AddRes(const wstring& _strKey, RES_TYPE _type, CRes* _pRes)
{
	Ptr<CRes> pRes = FindRes(_strKey, _type);

	assert(!pRes.Get());

	_pRes->SetKey(_strKey);
	m_arrRes[(UINT)_type].insert(make_pair(_strKey, _pRes));
	m_bChanged = true;
}

Ptr<CRes> CResMgr::FindRes(const wstring& _strKey, RES_TYPE _type)
{
	map<wstring, Ptr<CRes>>::iterator iter = m_arrRes[(UINT)_type].find(_strKey);

	if (iter == m_arrRes[(UINT)_type].end())
	{
		return nullptr;
	}

	return iter->second;
}



Ptr<CTexture> CResMgr::LoadTexture(const wstring& _strKey, const wstring& _strRelativePath, int _iMapLevel)
{
	CTexture* pRes = FindRes<CTexture>(_strKey).Get();
	if (nullptr != pRes)
	{
		return pRes;
	}

	pRes = new CTexture;

	wstring strFilePath = CPathMgr::GetInst()->GetContentPath();
	strFilePath += _strRelativePath;

	if (FAILED(pRes->Load(strFilePath, _iMapLevel)))
	{
		return nullptr;
	}

	pRes->SetKey(_strKey);
	pRes->SetRelativePath(_strRelativePath);

	m_arrRes[(UINT)RES_TYPE::TEXTURE].insert(make_pair(_strKey, pRes));

	return pRes;
}

//Ptr<CMeshData> CResMgr::LoadFBX(const wstring& _strPath)
//{
//	wstring strFileName = path(_strPath).stem();
//
//	wstring strName = L"meshdata\\";
//	strName += strFileName; //+ L".mdat";
//
//	Ptr<CMeshData> pMeshData = FindRes<CMeshData>(strName);
//
//	if (nullptr != pMeshData)
//		return pMeshData;
//	vector<CMeshData*> data;
//	data = CMeshData::LoadFromFBX(_strPath);
//	for (UINT i = 0; i < data.size(); ++i)
//	{
//		pMeshData = data[i];
//		pMeshData->SetKey(strName);
//		pMeshData->SetRelativePath(strName);
//		pMeshData->m_bEngineRes = true;
//		wstring name;
//		name = strName;
//		name += std::to_wstring(i) + L".mdat";
//		m_arrRes[(UINT)RES_TYPE::MESHDATA].insert(make_pair(name, pMeshData.Get()));
//
//		// meshdata 를 실제파일로 저장
//		pMeshData->Save(name);
//	}
//	pMeshData = data[0];
//
//	return pMeshData;
//}

Ptr<CMeshData> CResMgr::LoadFBX(const wstring& _strPath)
{
	wstring strFileName = path(_strPath).stem();

	wstring strName = L"meshdata\\";
	strName += strFileName + L".mdat";

	Ptr<CMeshData> pMeshData = FindRes<CMeshData>(strName);

	if (nullptr != pMeshData)
		return pMeshData;

	pMeshData = CMeshData::LoadFromFBX(_strPath);
	pMeshData->SetKey(strName);
	pMeshData->SetRelativePath(strName);
	pMeshData->m_bEngineRes = true;
	m_arrRes[(UINT)RES_TYPE::MESHDATA].insert(make_pair(strName, pMeshData.Get()));

	// meshdata 를 실제파일로 저장
	pMeshData->Save(strName);

	return pMeshData;
}
void CResMgr::MeshDataSave(vector<CMeshData*> _meshdata, const wstring& _strPath)
{
	wstring strFileName = path(_strPath).stem();
	for (int i = 0; i < _meshdata.size(); ++i)
	{
		
		wstring strName = L"meshdata\\";
		wstring strcopyFileName = strFileName + L"_" + std::to_wstring(i);
		strName += strcopyFileName + L".mdat";
		Ptr<CMeshData> pMeshData = FindRes<CMeshData>(strName);
		if (nullptr != pMeshData)
			return;
		_meshdata[i]->SetKey(strName);
		_meshdata[i]->SetRelativePath(strName);
		_meshdata[i]->m_bEngineRes = true;
		m_arrRes[(UINT)RES_TYPE::MESHDATA].insert(make_pair(strName, _meshdata[i]));

		// meshdata 를 실제파일로 저장
		_meshdata[i]->Save(strName);
	}

}