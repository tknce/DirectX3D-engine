#include "pch.h"
#include "CLight2D.h"


#include "CRenderMgr.h"
#include "CTransform.h"
#include "CMeshRender.h"

CLight2D::CLight2D()
	: CComponent(COMPONENT_TYPE::LIGHT2D)
{
	m_Info.iLightType = LIGHT_TYPE::POINT;
}

CLight2D::CLight2D(const CLight2D& _origin)
	:CComponent(COMPONENT_TYPE::LIGHT2D)
{
	tLightInfo m_Info = _origin.m_Info;
}

CLight2D::~CLight2D()
{
}


void CLight2D::finaltick()
{




	m_Info.vWorldPos = Transform()->GetWorldPos();
	m_Info.vWorldDir = Transform()->GetWorldDir(DIR::RIGHT);

	CRenderMgr::GetInst()->RegisterLight2D(this);
}

void CLight2D::SaveToFile(FILE* _File)
{
	COMPONENT_TYPE type = GetType();
	fwrite(&type, sizeof(UINT), 1, _File);

	fwrite(&m_Info, sizeof(tLightInfo), 1, _File);
}

void CLight2D::LoadFromFile(FILE* _File)
{
	fread(&m_Info, sizeof(tLightInfo), 1, _File);
}
