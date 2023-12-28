#include "pch.h"
#include "CEditor.h"

#include "imgui.h"
#include "CGameObjectEx.h"
#include "CEditorCam.h"
#include "CCameraScript.h"
#include "CGrid2DScript.h"

#include <Engine\Ctransform.h>
#include <Engine\CMeshRender.h>
#include <Engine\CCamera.h>
#include <Engine\CRenderMgr.h>

#include <Engine\CLevelMgr.h>
#include <Engine\CLevel.h>


CEditor::CEditor()
	: m_DebugDrawObject{}
{

}

CEditor::~CEditor()
{
	Safe_Del_Vec(m_vecEditorObj);
	Safe_Del_Array(m_DebugDrawObject);
}

void CEditor::init()
{
	CreateDebugDrawObject();

	// Editor 용도 Grid Object 추가
	//CGameObjectEx* pGridObj = new CGameObjectEx;
	//pGridObj->SetName(L"Grid Object");

	//pGridObj->AddComponent(new CTransform);
	//pGridObj->AddComponent(new CMeshRender);
	//pGridObj->AddComponent(new CGrid2DScript);

	//pGridObj->MeshRender()->SetMesh(CResMgr::GetInst()->FindRes<CMesh>(L"RectMesh"));
	//pGridObj->MeshRender()->SetSharedMaterial(CResMgr::GetInst()->FindRes<CMaterial>(L"GridMtrl"));

	//pGridObj->GetScript<CGrid2DScript>()->SetGridColor(Vec4(0.2f, 0.9f, 0.2f, 1.f));
	//pGridObj->GetScript<CGrid2DScript>()->SetGridInterval(100.f);
	//pGridObj->GetScript<CGrid2DScript>()->SetThickness(2.f);

	//m_vecEditorObj.push_back(pGridObj);


	// Editor 용도 Camera Object 추가
	CGameObjectEx* pEditorCam = new CGameObjectEx;
	pEditorCam->SetName(L"Editor Camera");

	pEditorCam->AddComponent(new CTransform);
	pEditorCam->AddComponent(new CEditorCam);
	pEditorCam->AddComponent(new CCameraScript);

	pEditorCam->Camera()->SetProjType(PROJ_TYPE::PERSPECTIVE);
	pEditorCam->Camera()->SetFar(100000.f);
	pEditorCam->Camera()->SetLayerMaskAll();
	pEditorCam->Camera()->SetLayerMask(31);

	m_vecEditorObj.push_back(pEditorCam);
	CRenderMgr::GetInst()->RegisterEditorCamera(pEditorCam->Camera());


}

void CEditor::progress()
{
	if (LEVEL_STATE::PLAY != CLevelMgr::GetInst()->GetCurLevel()->GetState())
	{
		tick();
		render();
	}

	// Debug Shape
	render_debug();
}

void CEditor::tick()
{
	for (size_t i = 0; i < m_vecEditorObj.size(); ++i)
	{
		m_vecEditorObj[i]->tick();
	}

	for (size_t i = 0; i < m_vecEditorObj.size(); ++i)
	{
		m_vecEditorObj[i]->finaltick();
	}
}

void CEditor::render()
{
	for (size_t i = 0; i < m_vecEditorObj.size(); ++i)
	{
		m_vecEditorObj[i]->render();
	}
}


void CEditor::render_debug()
{
	// DebugDrawRender
	// 일정 시간동안 렌더링 되는 Shape 
	list<tDebugShapeInfo>::iterator iter = m_DebugDrawList.begin();
	for (; iter != m_DebugDrawList.end(); )
	{
		iter->fCurTime += DT;
		if (iter->fDuration < iter->fCurTime)
		{
			iter = m_DebugDrawList.erase(iter);
		}
		else
		{
			DebugDraw(*iter);
			++iter;
		}
	}

	// 새로 추가된 DebugShape 확인
	vector<tDebugShapeInfo>& vecInfo = CRenderMgr::GetInst()->GetDebutgDrawInfo();

	for (size_t i = 0; i < vecInfo.size(); ++i)
	{
		DebugDraw(vecInfo[i]);

		if (0.f < vecInfo[i].fDuration)
		{
			m_DebugDrawList.push_back(vecInfo[i]);
		}
	}
	vecInfo.clear();
}


void CEditor::CreateDebugDrawObject()
{
	CGameObjectEx* pDebugObj = nullptr;

	// DEBUG_SHAPE::RECT
	pDebugObj = new CGameObjectEx;

	pDebugObj->AddComponent(new CTransform);
	pDebugObj->AddComponent(new CMeshRender);

	pDebugObj->MeshRender()->SetMesh(CResMgr::GetInst()->FindRes<CMesh>(L"RectMesh_Debug"));
	pDebugObj->MeshRender()->SetSharedMaterial(CResMgr::GetInst()->FindRes<CMaterial>(L"DebugDrawMtrl"),0);

	m_DebugDrawObject[(UINT)DEBUG_SHAPE::RECT] = pDebugObj;

	// DEBUG_SHAPE::CIRCLE
	pDebugObj = new CGameObjectEx;

	pDebugObj->AddComponent(new CTransform);
	pDebugObj->AddComponent(new CMeshRender);

	pDebugObj->MeshRender()->SetMesh(CResMgr::GetInst()->FindRes<CMesh>(L"CircleMesh_Debug"));
	pDebugObj->MeshRender()->SetSharedMaterial(CResMgr::GetInst()->FindRes<CMaterial>(L"DebugDrawMtrl"),0);

	m_DebugDrawObject[(UINT)DEBUG_SHAPE::CIRCLE] = pDebugObj;

	pDebugObj = new CGameObjectEx;

	pDebugObj->AddComponent(new CTransform);
	pDebugObj->AddComponent(new CMeshRender);

	pDebugObj->MeshRender()->SetMesh(CResMgr::GetInst()->FindRes<CMesh>(L"CubeMesh"));
	pDebugObj->MeshRender()->SetSharedMaterial(CResMgr::GetInst()->FindRes<CMaterial>(L"DebugDrawMtrl"),0);

	m_DebugDrawObject[(UINT)DEBUG_SHAPE::CUBE] = pDebugObj;

	pDebugObj = new CGameObjectEx;

	pDebugObj->AddComponent(new CTransform);
	pDebugObj->AddComponent(new CMeshRender);

	pDebugObj->MeshRender()->SetMesh(CResMgr::GetInst()->FindRes<CMesh>(L"SphereMesh"));
	pDebugObj->MeshRender()->SetSharedMaterial(CResMgr::GetInst()->FindRes<CMaterial>(L"DebugDrawMtrl"),0);

	m_DebugDrawObject[(UINT)DEBUG_SHAPE::SPHERE] = pDebugObj;

	pDebugObj = new CGameObjectEx;

	pDebugObj->AddComponent(new CTransform);
	pDebugObj->AddComponent(new CMeshRender);

	pDebugObj->MeshRender()->SetMesh(CResMgr::GetInst()->FindRes<CMesh>(L"ConeMesh"));
	pDebugObj->MeshRender()->SetSharedMaterial(CResMgr::GetInst()->FindRes<CMaterial>(L"DebugDrawMtrl"),0);

	m_DebugDrawObject[(UINT)DEBUG_SHAPE::CONE] = pDebugObj;
}

void CEditor::DebugDraw(tDebugShapeInfo& _info)
{
	CGameObjectEx* pDebugObj = m_DebugDrawObject[(UINT)_info.eShape];

	pDebugObj->Transform()->SetRelativePos(_info.vPosition);
	pDebugObj->Transform()->SetRelativeRotation(_info.vRot);

	if (DEBUG_SHAPE::CIRCLE == _info.eShape || DEBUG_SHAPE::SPHERE == _info.eShape || DEBUG_SHAPE::CONE == _info.eShape)
	{
		pDebugObj->Transform()->SetRelativeScale(Vec3(_info.fRadius * 2.f, _info.fRadius * 2.f, _info.fRadius * 2.f));
	}
	else
	{
		pDebugObj->Transform()->SetRelativeScale(_info.vScale);
	}

	pDebugObj->MeshRender()->GetCurMaterial(0)->SetScalarParam(SCALAR_PARAM::VEC4_0, &_info.vColor);


	CCamera* pMainCam = CRenderMgr::GetInst()->GetMainCam();

	pDebugObj->Transform()->finaltick();

	g_transform.matWorld = pDebugObj->Transform()->GetWorldMat();
	g_transform.matView = pMainCam->GetViewMat();
	g_transform.matProj = pMainCam->GetProjMat();

	pDebugObj->render();
}


void Vector3::ToRadian()
{
	x = (x / 180.f) * XM_PI;
	y = (y / 180.f) * XM_PI;
	z = (z / 180.f) * XM_PI;
}

void Vector3::ToDegree()
{
	x = (x / XM_PI) * 180.f;
	y = (y / XM_PI) * 180.f;
	z = (z / XM_PI) * 180.f;
}

Vec2::operator ImVec2() const { return ImVec2(x, y); }