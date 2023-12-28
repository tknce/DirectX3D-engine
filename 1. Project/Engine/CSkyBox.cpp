#include "pch.h"
#include "CSkyBox.h"

#include "CTransform.h"


CSkyBox::CSkyBox()
	: CRenderComponent(COMPONENT_TYPE::SKYBOX)
	, m_Type(SKYBOX_TYPE::SPHERE)
{
	SetType(m_Type);
	SetSharedMaterial(CResMgr::GetInst()->FindRes<CMaterial>(L"SkyBoxMtrl"),0);	
}

CSkyBox::~CSkyBox()
{
}

void CSkyBox::finaltick()
{
	// MainCam 위치를 알아낸다.
	// SkyBox 의 위치값을 갱신

	// MainCam 의 Far 값을 가져온다
	// Transform Scale 로 적용
}

void CSkyBox::render()
{
	Transform()->UpdateData();

	GetCurMaterial(0)->SetScalarParam(INT_0, (void*)&m_Type);

	if (SKYBOX_TYPE::SPHERE == m_Type)
	{
		GetCurMaterial(0)->SetTexParam(TEX_0, m_SkyBoxTex);
	}
	else
	{
		GetCurMaterial(0)->SetTexParam(TEX_CUBE_0, m_SkyBoxTex);
	}

	GetCurMaterial(0)->UpdateData();

	GetMesh()->render(0);
}

void CSkyBox::render(UINT _iSubset)
{
	render();
}



void CSkyBox::SetType(SKYBOX_TYPE _type)
{
	m_Type = _type;

	if (SKYBOX_TYPE::CUBE == _type)
	{
		SetMesh(CResMgr::GetInst()->FindRes<CMesh>(L"CubeMesh"));
		SetSharedMaterial(CResMgr::GetInst()->FindRes<CMaterial>(L"SkyBoxMtrl"), 0);
	}

	else if (SKYBOX_TYPE::SPHERE == _type)
	{
		SetMesh(CResMgr::GetInst()->FindRes<CMesh>(L"SphereMesh"));
		SetSharedMaterial(CResMgr::GetInst()->FindRes<CMaterial>(L"SkyBoxMtrl"), 0);
	}
}

void CSkyBox::SetType(UINT _type)
{
	if (SKYBOX_TYPE::CUBE == (SKYBOX_TYPE)_type || SKYBOX_TYPE::SPHERE == (SKYBOX_TYPE)_type)
	{
		m_Type = (SKYBOX_TYPE)_type;

		SetType(m_Type);
	}
}

void CSkyBox::SetSkyBoxTex(Ptr<CTexture> _tex)
{
	m_SkyBoxTex = _tex;
}


void CSkyBox::SaveToFile(FILE* _File)
{
	COMPONENT_TYPE type = CComponent::GetType();
	fwrite(&type, sizeof(UINT), 1, _File);

	// 스카이박스 타입
	fwrite(&m_Type, sizeof(SKYBOX_TYPE), 1, _File);

	// 참조 아틀라스 텍스쳐
	SaveResourceRef<CTexture>(m_SkyBoxTex, _File);

}

void CSkyBox::LoadFromFile(FILE* _File)
{


	// 스카이박스 타입
	fread(&m_Type, sizeof(SKYBOX_TYPE), 1, _File);


	// 참조 아틀라스 텍스쳐
	LoadResourceRef<CTexture>(m_SkyBoxTex, _File);
}