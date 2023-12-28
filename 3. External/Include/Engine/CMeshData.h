#pragma once
#include "CRes.h"

#include "Ptr.h"
#include "CMaterial.h"
#include "CMesh.h"


class CMeshData :
	public CRes
{
	Ptr<CMesh>				m_pMesh;
	vector<Ptr<CMaterial>>	m_vecMtrl;

public:
	static CMeshData* LoadFromFBX(const wstring& _strFilePath);

	virtual void Save(const wstring& _strFilePath);
	virtual int Load(const wstring& _strFilePath);

	CGameObject* Instantiate();
	Ptr<CMesh> GetMesh() { return m_pMesh; }
	vector<Ptr<CMaterial>> GetMaterial() { return m_vecMtrl; }

	CLONE_ASSERT(CMeshData)
public:
	CMeshData(bool _bEngine = false);
	virtual ~CMeshData();
};

