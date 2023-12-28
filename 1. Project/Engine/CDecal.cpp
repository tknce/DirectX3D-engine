#include "pch.h"
#include "CDecal.h"

#include "CTransform.h"

CDecal::CDecal()
	: CRenderComponent(COMPONENT_TYPE::DECAL)
	, m_bLighting(false)
{
	SetMesh(CResMgr::GetInst()->FindRes<CMesh>(L"CubeMesh"));
	SetDefaultLit(m_bLighting);
}

CDecal::~CDecal()
{
}

void CDecal::SetDefaultLit(bool _bSet)
{
	m_bLighting = _bSet;

	// DeferredDecal 쪽 렌더 -> 빛적용 구현해야됌
	if (m_bLighting)
	{
		SetSharedMaterial(CResMgr::GetInst()->FindRes<CMaterial>(L"DeferredDecalMtrl"),0);
	}
	else
	{
		SetSharedMaterial(CResMgr::GetInst()->FindRes<CMaterial>(L"DecalMtrl"),0);
	}
}

void CDecal::finaltick()
{
}

void CDecal::render()
{
	// 큐브메쉬
	DebugDrawCube(Vec4(0.2f, 0.8f, 0.2f, 1.f), Transform()->GetWorldPos(), Transform()->GetWorldScale(), Transform()->GetRelativeRotation());

	// 위치 행렬 업데이트
	Transform()->UpdateData();

	// 머테리얼 업데이트
	GetCurMaterial(0)->SetTexParam(TEX_1, m_DecalTex);
	GetCurMaterial(0)->UpdateData();

	// 렌더링
	GetMesh()->render(0);
}

void CDecal::render(UINT _iSubset)
{
	render();
}

void CDecal::SaveToFile(FILE* _File)
{
	COMPONENT_TYPE type = GetType();
	fwrite(&type, sizeof(UINT), 1, _File);
	// 스카이박스 타입
	fwrite(&m_bLighting, sizeof(bool), 1, _File);

	// 참조 아틀라스 텍스쳐
	SaveResourceRef<CTexture>(m_DecalTex, _File);
}

void CDecal::LoadFromFile(FILE* _File)
{


	// 스카이박스 타입
	fread(&m_bLighting, sizeof(bool), 1, _File);


	// 참조 아틀라스 텍스쳐
	LoadResourceRef<CTexture>(m_DecalTex, _File);
}
