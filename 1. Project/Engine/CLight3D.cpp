#include "pch.h"
#include "CLight3D.h"

#include "CRenderMgr.h"
#include "CTransform.h"
#include "CCamera.h"

CLight3D::CLight3D()
	: CComponent(COMPONENT_TYPE::LIGHT3D)
	, m_iLightIdx(-1)
	, m_pLightCam(nullptr)
{
	m_pLightCam = new CGameObject;
	m_pLightCam->AddComponent(new CTransform);
	m_pLightCam->AddComponent(new CCamera);
}

CLight3D::CLight3D(const CLight3D& _origin)
	: CComponent(_origin)
	, m_Info(_origin.m_Info)
	, m_iLightIdx(-1)
	, m_pLightCam(nullptr)
{
	m_pLightCam = _origin.m_pLightCam->Clone();
}

CLight3D::~CLight3D()
{
	if (nullptr != m_pLightCam)
		delete m_pLightCam;
}


void CLight3D::finaltick()
{
	static int a = 0;
	// 디버그 랜더링용
	if (LIGHT_TYPE::POINT == m_Info.iLightType)
	{
		a = 0;
		m_pLightMtrl->SetScalarParam(SCALAR_PARAM::INT_0, &a);
		Transform()->SetRelativeScale(Vec3(m_Info.fRadius * 2.f, m_Info.fRadius * 2.f, m_Info.fRadius * 2.f));
	}
	if (LIGHT_TYPE::SPOT == m_Info.iLightType)
	{
		a = 1;
		m_pLightMtrl->SetScalarParam(SCALAR_PARAM::INT_0, &a);
		Transform()->SetRelativeScale(Vec3(m_Info.fRadius, m_Info.fRadius, m_Info.fRadius));
	}
	if (LIGHT_TYPE::DIRECTIONAL == m_Info.iLightType)
	{
		a = 2;
		m_pLightMtrl->SetScalarParam(SCALAR_PARAM::INT_0, &a);
	}

	m_Info.vWorldPos = Transform()->GetWorldPos();
	m_Info.vWorldDir = Transform()->GetWorldDir(DIR::FRONT);



	// RenderMgr 에 등록시킨 후, 인덱스를 리턴받는다.
	m_iLightIdx = CRenderMgr::GetInst()->RegisterLight3D(this);

	// 광원의 카메라도 광원과 동일한 Transform 이 되도록 업데이트 한다.
	m_pLightCam->Transform()->SetRelativePos(Transform()->GetWorldPos());
	m_pLightCam->Transform()->SetRelativeRotation(DecomposeRotMat(Transform()->GetWorldRotation()));
	m_pLightCam->finaltick_module();
}


void CLight3D::render()
{
	// 디버그 랜더링용
	if (LIGHT_TYPE::POINT == m_Info.iLightType)
		DebugDrawSphere(Vec4(0.2f, 0.8f, 0.2f, 1.f), Transform()->GetWorldPos(), Transform()->GetWorldScale().x * 0.5f);

	if (LIGHT_TYPE::SPOT == m_Info.iLightType)
		DebugDrawCone(Vec4(0.2f, 0.8f, 0.2f, 1.f), Transform()->GetWorldPos(), Transform()->GetRelativeRotation(), Transform()->GetWorldScale().x * 0.5f);



	if (nullptr == m_pLightMtrl)
		return;

	// Transform Update
	Transform()->UpdateData();

	// 재질을 통해서 본인의 라이트 인덱스를 전달
	m_pLightMtrl->SetScalarParam(INT_0, &m_iLightIdx);

	// 재질 바인딩(쉐이더, 상수, 텍스쳐(타겟) 등등)
	m_pLightMtrl->UpdateData();

	// 렌더링
	m_pVolumeMesh->render(0);

	// 방향성 광원인 경우 그림자 처리를 위해서 광원카메라로 투영시킬 수 있게 View * Proj 행렬을 전달
	if (LIGHT_TYPE::DIRECTIONAL == (LIGHT_TYPE)m_Info.iLightType)
	{
		Matrix matLightVP = m_pLightCam->Camera()->GetViewMat() * m_pLightCam->Camera()->GetProjMat();
		m_pLightMtrl->SetScalarParam(SCALAR_PARAM::MAT_0, &matLightVP);
		m_pLightMtrl->SetTexParam(TEX_PARAM::TEX_3, CResMgr::GetInst()->FindRes<CTexture>(L"DepthMapTex"));
	}
}

void CLight3D::render_depthmap()
{
	m_pLightCam->Camera()->SortShadowObject();
	m_pLightCam->Camera()->render_depthmap();
}

void CLight3D::SetLightDirection(Vec3 _vDir)
{
	m_Info.vWorldDir = _vDir;
	m_Info.vWorldDir.Normalize();

	Vec3 vFront = m_Info.vWorldDir;
	Vec3 vUp = Vec3(0.f, 1.f, 0.f);
	Vec3 vRight = XMVector3Cross(vUp, vFront);
	vRight.Normalize();
	vUp = XMVector3Cross(vFront, vRight);
	vUp.Normalize();

	Matrix matRot = XMMatrixIdentity();
	matRot._11 = vRight.x;	matRot._12 = vRight.y;	matRot._13 = vRight.z;
	matRot._21 = vUp.x;		matRot._22 = vUp.y;		matRot._23 = vUp.z;
	matRot._31 = vFront.x;	matRot._32 = vFront.y;	matRot._33 = vFront.z;

	Vec3 vRot = DecomposeRotMat(matRot);

	// 광원이 가리키는 방향이 Transform 의 Front 가 되도록 회전시켜준다.
	Transform()->SetRelativeRotation(vRot);
}

void CLight3D::SetLightType(LIGHT_TYPE _eType)
{
	m_Info.iLightType = _eType;

	if (LIGHT_TYPE::DIRECTIONAL == m_Info.iLightType)
	{
		// 태양이기때문에 모든 부분에 빛이 들어가야된다
		m_pVolumeMesh = CResMgr::GetInst()->FindRes<CMesh>(L"RectMesh");
		m_pLightMtrl = CResMgr::GetInst()->FindRes<CMaterial>(L"DirLightMtrl");

		m_pLightCam->Camera()->SetProjType(PROJ_TYPE::ORTHOGRAHPICS);
		m_pLightCam->Camera()->SetWidth(4096.f);
		m_pLightCam->Camera()->SetAspectRatio(1.f);
		m_pLightCam->Camera()->SetFar(100000.f);
	}

	else if (LIGHT_TYPE::POINT == m_Info.iLightType)
	{
		// 동그라미 부분에 들어가야된다. 맞닿는 부분만 랜더링 되야되기때문에 수식을 잡아줘야된다.
		m_pVolumeMesh = CResMgr::GetInst()->FindRes<CMesh>(L"SphereMesh");
		m_pLightMtrl = CResMgr::GetInst()->FindRes<CMaterial>(L"PointLightMtrl");
	}

	else
	{
		m_pVolumeMesh = CResMgr::GetInst()->FindRes<CMesh>(L"ConeMesh");
		m_pLightMtrl = CResMgr::GetInst()->FindRes<CMaterial>(L"PointLightMtrl");
	}
}

void CLight3D::SaveToFile(FILE* _File)
{
	COMPONENT_TYPE type = GetType();
	fwrite(&type, sizeof(UINT), 1, _File);

	fwrite(&m_Info, sizeof(tLightInfo), 1, _File);
}

void CLight3D::LoadFromFile(FILE* _File)
{
	fread(&m_Info, sizeof(tLightInfo), 1, _File);
	SetLightType(m_Info.iLightType);
}