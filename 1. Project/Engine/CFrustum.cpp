#include "pch.h"
#include "CFrustum.h"

#include "CCamera.h"
#include "CMesh.h"
#include "CMeshRender.h"

CFrustum::CFrustum(CCamera* _Owner)
	: m_Owner(_Owner)
{
	// 투영공간 좌표
	//     4 ------ 5
	//     |        |  Far
	//   / |        |
	//	/  7 ------ 6	
	// /      /
	// 0 -- 1     /
	// |    |    / Near
	// 3 -- 2
	m_ProjPos[0] = Vec3(-1.f, 1.f, 0.f);
	m_ProjPos[1] = Vec3(1.f, 1.f, 0.f);
	m_ProjPos[2] = Vec3(1.f, -1.f, 0.f);
	m_ProjPos[3] = Vec3(-1.f, -1.f, 0.f);

	m_ProjPos[4] = Vec3(-1.f, 1.f, 1.f);
	m_ProjPos[5] = Vec3(1.f, 1.f, 1.f);
	m_ProjPos[6] = Vec3(1.f, -1.f, 1.f);
	m_ProjPos[7] = Vec3(-1.f, -1.f, 1.f);
}

CFrustum::~CFrustum()
{
}

void CFrustum::finaltick()
{
	return;
	// 카메라 (투영행렬 역) * (뷰행렬 역)
	Matrix matInv = m_Owner->GetProjMatInv() * m_Owner->GetViewMatInv();

	

	Vec3 vWorldPos[8] = {};

	for (int i = 0; i < 8; ++i)
	{
		m_WorldPos[i] = XMVector3TransformCoord(m_ProjPos[i], matInv);
	}

	// 월드공간 좌표 vWorldPos
	//     4 ------ 5
	//     |        |  Far
	//   / |        |
	//	/  7 ------ 6	
	// /      /
	// 0 -- 1     /
	// |    |    / Near
	// 3 -- 2

	// 월드 정점을 이용해서 평면 6개 구성
	m_Plane[(UINT)FACE_TYPE::FT_NEAR] = XMPlaneFromPoints(m_WorldPos[0], m_WorldPos[1], m_WorldPos[2]);
	m_Plane[(UINT)FACE_TYPE::FT_FAR] = XMPlaneFromPoints(m_WorldPos[7], m_WorldPos[6], m_WorldPos[5]);

	m_Plane[(UINT)FACE_TYPE::FT_UP] = XMPlaneFromPoints(m_WorldPos[4], m_WorldPos[5], m_WorldPos[1]);
	m_Plane[(UINT)FACE_TYPE::FT_DOWN] = XMPlaneFromPoints(m_WorldPos[3], m_WorldPos[2], m_WorldPos[6]);

	m_Plane[(UINT)FACE_TYPE::FT_LEFT] = XMPlaneFromPoints(m_WorldPos[4], m_WorldPos[0], m_WorldPos[3]);
	m_Plane[(UINT)FACE_TYPE::FT_RIGHT] = XMPlaneFromPoints(m_WorldPos[1], m_WorldPos[5], m_WorldPos[6]);

	if (m_Owner->GetOwner()->GetName() == L"MainCamera")
	m_Owner->GetOwner()->MeshRender()->GetCurMaterial(0)->SetScalarParam(SCALAR_PARAM::MAT_0, &matInv);

	if (m_Owner->GetOwner()->GetName() == L"MainCamera")
	{
		// frustum Mesh
		static vector<Vtx> vecVtx;
		vecVtx.resize(5);

		Vtx arrfrustum[5] = {};

		arrfrustum[0].vPos = Vec3(0.f,0.f,0.f);
		arrfrustum[1].vPos = m_WorldPos[4];
		arrfrustum[2].vPos = m_WorldPos[5];
		arrfrustum[3].vPos = m_WorldPos[6];
		arrfrustum[4].vPos = m_WorldPos[7];
		
		for (size_t i = 0; i < 5; ++i)
		{
			vecVtx[i] = arrfrustum[i];
		}

		m_Owner->GetOwner()->MeshRender()->GetMesh()->Create(vecVtx.data(), vecVtx.size());
		vecVtx.clear();
	}
}

bool CFrustum::CheckFrustum(Vec3 _vPos)
{
	for (UINT i = 0; i < (UINT)FACE_TYPE::END; ++i)
	{
		float fDot = _vPos.Dot(m_Plane[i]);
		if (fDot + m_Plane[i].w > 0)
		{
			return false;
		}
	}

	return true;
}

bool CFrustum::CheckFrustumRadius(Vec3 _vPos, float _fRadius)
{
	for (UINT i = 0; i < (UINT)FACE_TYPE::END; ++i)
	{
		float fDot = _vPos.Dot(m_Plane[i]);
		if (fDot + m_Plane[i].w > _fRadius)
		{
			return false;
		}
	}

	return true;
}
