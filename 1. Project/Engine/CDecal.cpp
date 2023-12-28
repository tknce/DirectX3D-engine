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

	// DeferredDecal �� ���� -> ������ �����ؾ߉�
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
	// ť��޽�
	DebugDrawCube(Vec4(0.2f, 0.8f, 0.2f, 1.f), Transform()->GetWorldPos(), Transform()->GetWorldScale(), Transform()->GetRelativeRotation());

	// ��ġ ��� ������Ʈ
	Transform()->UpdateData();

	// ���׸��� ������Ʈ
	GetCurMaterial(0)->SetTexParam(TEX_1, m_DecalTex);
	GetCurMaterial(0)->UpdateData();

	// ������
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
	// ��ī�̹ڽ� Ÿ��
	fwrite(&m_bLighting, sizeof(bool), 1, _File);

	// ���� ��Ʋ�� �ؽ���
	SaveResourceRef<CTexture>(m_DecalTex, _File);
}

void CDecal::LoadFromFile(FILE* _File)
{


	// ��ī�̹ڽ� Ÿ��
	fread(&m_bLighting, sizeof(bool), 1, _File);


	// ���� ��Ʋ�� �ؽ���
	LoadResourceRef<CTexture>(m_DecalTex, _File);
}
