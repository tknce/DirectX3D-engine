#include "pch.h"
#include "CCamera.h"

#include "CLevelMgr.h"
#include "CLevel.h"

#include "CRenderMgr.h"
#include "CMRT.h"

#include "CKeyMgr.h"
#include "CTimeMgr.h"

#include "CDevice.h"
#include "CRenderMgr.h"
#include "CTransform.h"
#include "CRenderComponent.h"
#include "CMaterial.h"
#include "CStructuredBuffer.h"

#include "CGameObject.h"

#include "CLight3D.h"
#include "CAnimation3D.h"

#include "CKeyMgr.h"
#include "CInstancingBuffer.h"

CCamera::CCamera()
	: CComponent(COMPONENT_TYPE::CAMERA)
	, m_Frustum(this)
	, m_eProjType(PROJ_TYPE::PERSPECTIVE)
	, m_fWidth(0.f)
	, m_fAspectRatio(1.f)
	, m_FOV(XM_PI / 2.f)
	, m_fFar(5000.f)
	, m_fScale(1.f)
	, m_ray{}
	, m_iLayerMask(0)
	, m_iCamIdx(0)
{
	Vec2 vRenderResolution = CDevice::GetInst()->GetRenderResolution();
	m_fWidth = CDevice::GetInst()->GetRenderResolution().x;
	m_fAspectRatio = vRenderResolution.x / vRenderResolution.y;
	



}

CCamera::~CCamera()
{
	//m_HDRShader = nullptr;
}

void CCamera::finaltick()
{
	// 뷰행렬 계산
	CalcViewMat();

	// 투영행렬 계산
	CalcProjMat();

	// 타켓과 마우스 움직임 계산
	if (m_pTarget != nullptr)
	{
		Vec3 Pos = m_pTarget->Transform()->GetRelativePos();
		Vec3 MousePos = Vec3(CKeyMgr::GetInst()->GetCameraMousePos().x, CKeyMgr::GetInst()->GetCameraMousePos().y, 0.f);
		Vec3 CameraPos = Transform()->GetRelativePos();

		Vec3 Dir = Pos - MousePos;
		Dir.Normalize();
		
		CameraPos = Pos;
		CameraPos -= Dir * 80;
		if ((Pos + 200) < CameraPos)
		{
			Dir = Vec3(0, 0, 0);
			CameraPos = Pos;
		}		
		if ((Dir + -200) > CameraPos)
		{
			Dir = Vec3(0, 0, 0);
			CameraPos = Pos;
		}			
		Transform()->SetRelativePos(CameraPos);
	}
	// 마우스방향 직선 계산
	CalRay();

	// Frustum 구성
	m_Frustum.finaltick();


	// 카메라 등록
	CRenderMgr::GetInst()->RegisterCamera(this);
}

void CCamera::finaltick_module()
{
	// 뷰행렬 계산
	CalcViewMat();

	// 투영행렬 계산
	CalcProjMat();

	// Frustum 구성
	m_Frustum.finaltick();

	// 마우스방향 직선 계산
	CalRay();
}

void CCamera::render()
{
	// 행렬 세팅
	g_transform.matView = m_matView;
	g_transform.matViewInv = m_matViewInv;
	g_transform.matProj = m_matProj;
	g_transform.matProjInv = m_matProjInv;

	// Shader Domain 에 따른 물체 분류
	SortObject();

	// Domain 분류에 따른 렌더링
	// Deferred Rendering
	CRenderMgr::GetInst()->GetMRT(MRT_TYPE::DEFERRED)->OMSet();
	render_deferred();
	
	// Deferred Object 광원처리
	CRenderMgr::GetInst()->GetMRT(MRT_TYPE::LIGHT)->OMSet();
	const vector<CLight3D*>& vecLight3D = CRenderMgr::GetInst()->GetLight3D();

	for (size_t i = 0; i < vecLight3D.size(); ++i)
	{
		vecLight3D[i]->render();
	}


	CRenderMgr::GetInst()->GetMRT(MRT_TYPE::SWAPCHAIN)->OMSet();
	
	Adaptation();



	
	// SwapChain MTR 로 변경

	// Domain 분류에 따른 렌더링
	// Forward Rendering
	//render_opaque();
	//render_mask();
	render_forward();

	render_decal();
	render_transparent();

	CRenderMgr::GetInst()->UpdateHLOOM();
	
	// bloom
	Ptr<CMaterial>pMergeMtrl = CResMgr::GetInst()->FindRes<CMaterial>(L"BloomMtrl");
	Ptr<CMesh>pRectMesh = CResMgr::GetInst()->FindRes<CMesh>(L"RectMesh");

	

	CRenderMgr::GetInst()->CopyRenderTarget();
	pMergeMtrl->UpdateData();
	pRectMesh->render(0);

	render_postprocess();
}


void CCamera::SetLayerMask(const wstring& _strLayerName)
{
	CLevel* pCurLevel = CLevelMgr::GetInst()->GetCurLevel();
	CLayer* pLayer = pCurLevel->GetLayer(_strLayerName);
	assert(pLayer);

	SetLayerMask(pLayer->GetLayerIdx());
}

void CCamera::SetLayerMask(int _iLayerIdx)
{
	if (m_iLayerMask & (1 << _iLayerIdx))
	{
		m_iLayerMask &= ~(1 << _iLayerIdx);
	}
	else
	{
		m_iLayerMask |= (1 << _iLayerIdx);
	}
}

void CCamera::Adaptation()
{
	static float PreLuminance = 0;
	static float Time = 0;
	static float fAdptNorm = 0;
	Time += DT;
	// Deferred -> Swapchain 병합
	static Ptr<CMaterial> pMergeMtrl = CResMgr::GetInst()->FindRes<CMaterial>(L"MergeMtrl");
	static Ptr<CMesh> pRectMesh = CResMgr::GetInst()->FindRes<CMesh>(L"RectMesh");
	float fLum = CRenderMgr::GetInst()->GetLuminance();

	{
		if (Time > 3.f)
			Time = 0;
		// 평균구하기
		fAdptNorm = (1 - (Time * 0.15f) * PreLuminance) + ((Time * 0.15f) * fLum);
		//fAdptNorm = min(3 < 0.0001f ? 3 : Time / 3, 3 - 0.0001f);


	}
	Vec4 adpt = Vec4(fAdptNorm, PreLuminance, 0, 0);
	float a = PreLuminance + fAdptNorm * (fLum - PreLuminance);


	int b = 0;
	pMergeMtrl->SetScalarParam(FLOAT_2, &fLum);
	pMergeMtrl->SetScalarParam(VEC4_0, &adpt);
	pMergeMtrl->SetScalarParam(INT_2, &a);
	PreLuminance = fLum;

	if (KEY_TAP(KEY::LBTN))
	{
		b = 1;
		pMergeMtrl->SetScalarParam(INT_2, &b);
	}

	pMergeMtrl->UpdateData();
	pRectMesh->render(0);
}

void CCamera::CalcViewMat()
{

		// =============
		// View 행렬 계산
		// =============
		// View 이동행렬 ( 카메라를 원점으로 이동하는 만큼 )
	
		Vec3 vPos = Vec3(Transform()->GetWorldMat()._41, Transform()->GetWorldMat()._42, Transform()->GetWorldMat()._43);		
		Matrix matViewTrans = XMMatrixTranslation(-vPos.x, -vPos.y, -vPos.z);
		
		// View 회전 행렬 ( 카메라가 보는 전방 방향을 z 축을 보도록 돌리는 만큼 )
		Vec3 vRight = Transform()->GetRelativeDir(DIR::RIGHT);
		Vec3 vUp = Transform()->GetRelativeDir(DIR::UP);
		Vec3 vFront = Transform()->GetRelativeDir(DIR::FRONT);



		Matrix matViewRot = XMMatrixIdentity();
		matViewRot._11 = vRight.x; matViewRot._12 = vUp.x; matViewRot._13 = vFront.x;
		matViewRot._21 = vRight.y; matViewRot._22 = vUp.y; matViewRot._23 = vFront.y;
		matViewRot._31 = vRight.z; matViewRot._32 = vUp.z; matViewRot._33 = vFront.z;

		//matViewRot = XMMatrixRotationX(Transform()->GetRelativeRotation().x);
		//matViewRot *= XMMatrixRotationY(Transform()->GetRelativeRotation().y);
		//matViewRot *= XMMatrixRotationZ(Transform()->GetRelativeRotation().z);

		m_matView = matViewTrans * matViewRot;
		// 역행렬 구하는 다이렉트 함수
		m_matViewInv = XMMatrixInverse(nullptr, m_matView);
}

void CCamera::CalcProjMat()
{
	// ==============
	// 투영 행렬 계산
	// ==============
	Vec2 vRenderResolution = CDevice::GetInst()->GetRenderResolution();

	if (PERSPECTIVE == m_eProjType)
	{
		// 원근투영
		m_matProj = XMMatrixPerspectiveFovLH(m_FOV, m_fAspectRatio, 1.f, m_fFar);
	}
	else
	{
		// 직교투영
		float fHeight = m_fWidth / m_fAspectRatio;
		m_matProj = XMMatrixOrthographicLH(m_fWidth * m_fScale, fHeight * m_fScale, 1.f, m_fFar);

	}

	m_matProjInv = XMMatrixInverse(nullptr, m_matProj);
}
void CCamera::CalRay()
{
	// 마우스 방향을 향하는 Ray 구하기
	// SwapChain 타겟의 ViewPort 정보
	CMRT* pMRT = CRenderMgr::GetInst()->GetMRT(MRT_TYPE::SWAPCHAIN);
	D3D11_VIEWPORT tVP = pMRT->GetViewPort();

	//  현재 마우스 좌표
	Vec2 vMousePos = CKeyMgr::GetInst()->GetMousePos();

	// 직선은 카메라의 좌표를 반드시 지난다.
	m_ray.vStart = Transform()->GetWorldPos();

	// view space 에서의 방향
	m_ray.vDir.x = ((((vMousePos.x - tVP.TopLeftX) * 2.f / tVP.Width) - 1.f) - m_matProj._31) / m_matProj._11;
	m_ray.vDir.y = (-(((vMousePos.y - tVP.TopLeftY) * 2.f / tVP.Height) - 1.f) - m_matProj._32) / m_matProj._22;
	m_ray.vDir.z = 1.f;

	// world space 에서의 방향
	m_ray.vDir = XMVector3TransformNormal(m_ray.vDir, m_matViewInv);
	m_ray.vDir.Normalize();
}

void CCamera::SortObject()
{
	for (auto& pair : m_mapInstGroup_D)
		pair.second.clear();
	for (auto& pair : m_mapInstGroup_F)
		pair.second.clear();

	m_vecDecal.clear();
	m_vecTransparent.clear();
	m_vecPostProcess.clear();

	CLevel* pCurLevel = CLevelMgr::GetInst()->GetCurLevel();

	for (UINT i = 0; i < MAX_LAYER; ++i)
	{
		// Layer 확인
		if (m_iLayerMask & (1 << i))
		{
			// 해당 레이어에 속한 게임오브젝트를 가져온다.
			CLayer* pLayer = pCurLevel->GetLayer(i);


			// 오브젝트들을 쉐이더 도메인에 따라 분류한다.
			const vector<CGameObject*>& vecObj = pLayer->GetObjects();
			for (size_t j = 0; j < vecObj.size(); ++j)
			{
				CRenderComponent* pRenderCom = vecObj[j]->GetRenderComponent();

				// 렌더 컴포넌트가 없거나, 메쉬참조가 없다면 제외
				if (nullptr == pRenderCom || nullptr == pRenderCom->GetMesh())
				{
					continue;
				}

				// frustum check 실패 시
				//if (!m_Frustum.CheckFrustumRadius(vecObj[j]->Transform()->GetWorldPos(), vecObj[j]->Transform()->GetWorldScale().x / 2.f ))
				/*if (vecObj[j]->GetRenderComponent()->IsUseFrustumCulling() && !m_Frustum.CheckFrustumRadius(vecObj[j]->Transform()->GetWorldPos()))
					continue;*/

					// 메테리얼 개수만큼 반복
				UINT iMtrlCount = pRenderCom->GetMtrlCount();

				for (UINT iMtrl = 0; iMtrl < iMtrlCount; ++iMtrl)
				{
					if (nullptr != pRenderCom->GetCurMaterial(iMtrl) || nullptr != pRenderCom->GetCurMaterial(iMtrl)->GetShader())
					{
						Ptr<CGraphicsShader> pShader = pRenderCom->GetCurMaterial(iMtrl)->GetShader();

						SHADER_DOMAIN eDomain = pShader->GetDomain();

						switch (eDomain)
						{
						case SHADER_DOMAIN::DOMAIN_DEFERRED_OPAQUE:
						case SHADER_DOMAIN::DOMAIN_DEFERRED_MASK:
						case SHADER_DOMAIN::DOMAIN_OPAQUE:
						case SHADER_DOMAIN::DOMAIN_MASK:
						{
							// Shader 의 POV 에 따라서 인스턴싱 그룹을 분류한다.
							map<ULONG64, vector<tInstObj>>* pMap = NULL;
							Ptr<CMaterial> pMtrl = pRenderCom->GetCurMaterial(iMtrl);

							if (pShader->GetDomain() == SHADER_DOMAIN::DOMAIN_DEFERRED_OPAQUE
								|| pShader->GetDomain() == SHADER_DOMAIN::DOMAIN_DEFERRED_MASK)
							{
								pMap = &m_mapInstGroup_D;
							}
							else if (pShader->GetDomain() == SHADER_DOMAIN::DOMAIN_OPAQUE
								|| pShader->GetDomain() == SHADER_DOMAIN::DOMAIN_MASK)
							{
								pMap = &m_mapInstGroup_F;
							}
							else
							{
								assert(nullptr);
								continue;
							}

							uInstID uID = {};
							uID.llID = pRenderCom->GetInstID(iMtrl);

							// ID 가 0 다 ==> Mesh 나 Material 이 셋팅되지 않았다.
							if (0 == uID.llID)
								continue;

							map<ULONG64, vector<tInstObj>>::iterator iter = pMap->find(uID.llID);
							if (iter == pMap->end())
							{
								pMap->insert(make_pair(uID.llID, vector<tInstObj>{tInstObj{ vecObj[j], iMtrl }}));
							}
							else
							{
								iter->second.push_back(tInstObj{ vecObj[j], iMtrl });
							}
						}
						break;
						case SHADER_DOMAIN::DOMAIN_DECAL:
							m_vecDecal.push_back(vecObj[j]);
							break;
						case SHADER_DOMAIN::DOMAIN_TRANSPARENT:
							m_vecTransparent.push_back(vecObj[j]);
							break;
						case SHADER_DOMAIN::DOMAIN_POST_PROCESS:
							m_vecPostProcess.push_back(vecObj[j]);
							break;
						}
					}
				}
			}
		}
	}
}

void CCamera::SortShadowObject()
{
	m_vecDynamicShadow.clear();

	CLevel* pCurLevel = CLevelMgr::GetInst()->GetCurLevel();

	for (UINT i = 0; i < MAX_LAYER; ++i)
	{
		CLayer* pLayer = pCurLevel->GetLayer(i);
		const vector<CGameObject*>& vecObj = pLayer->GetObjects();

		for (size_t j = 0; j < vecObj.size(); ++j)
		{
			CRenderComponent* pRenderCom = vecObj[j]->GetRenderComponent();

			if (pRenderCom && pRenderCom->IsDynamicShadow())
			{
				m_vecDynamicShadow.push_back(vecObj[j]);
			}
		}
	}
}

void CCamera::render_deferred()
{
	for (auto& pair : m_mapSingleObj)
	{
		pair.second.clear();
	}

	// Deferred object render
	tInstancingData tInstData = {};

	for (auto& pair : m_mapInstGroup_D)
	{
		// 그룹 오브젝트가 없거나, 쉐이더가 없는 경우
		if (pair.second.empty())
			continue;

		// instancing 개수 조건 미만이거나
		// Animation2D 오브젝트거나(스프라이트 애니메이션 오브젝트)
		// Shader 가 Instancing 을 지원하지 않는경우
		if (pair.second.size() <= 1
			|| pair.second[0].pObj->Animator2D()
			|| pair.second[0].pObj->GetRenderComponent()->GetCurMaterial(pair.second[0].iMtrlIdx)->GetShader()->GetVSInst() == nullptr)
		{
			// 해당 물체들은 단일 랜더링으로 전환
			for (UINT i = 0; i < pair.second.size(); ++i)
			{
				map<INT_PTR, vector<tInstObj>>::iterator iter
					= m_mapSingleObj.find((INT_PTR)pair.second[i].pObj);

				if (iter != m_mapSingleObj.end())
					iter->second.push_back(pair.second[i]);
				else
				{
					m_mapSingleObj.insert(make_pair((INT_PTR)pair.second[i].pObj, vector<tInstObj>{pair.second[i]}));
				}
			}
			continue;
		}

		CGameObject* pObj = pair.second[0].pObj;
		Ptr<CMesh> pMesh = pObj->GetRenderComponent()->GetMesh();
		Ptr<CMaterial> pMtrl = pObj->GetRenderComponent()->GetCurMaterial(pair.second[0].iMtrlIdx);

		// Instancing 버퍼 클리어
		CInstancingBuffer::GetInst()->Clear();

		int iRowIdx = 0;
		bool bHasAnim3D = false;
		for (UINT i = 0; i < pair.second.size(); ++i)
		{
			tInstData.matWorld = pair.second[i].pObj->Transform()->GetWorldMat();
			tInstData.matWV = tInstData.matWorld * m_matView;
			tInstData.matWVP = tInstData.matWV * m_matProj;

			if (pair.second[i].pObj->Animator3D())
			{
				pair.second[i].pObj->Animator3D()->UpdateData();
				tInstData.iRowIdx = iRowIdx++;
				CInstancingBuffer::GetInst()->AddInstancingBoneMat(pair.second[i].pObj->Animator3D()->GetFinalBoneMat());
				bHasAnim3D = true;
			}
			else
				tInstData.iRowIdx = -1;

			CInstancingBuffer::GetInst()->AddInstancingData(tInstData);
		}

		// 인스턴싱에 필요한 데이터를 세팅(SysMem -> GPU Mem)
		CInstancingBuffer::GetInst()->SetData();

		if (bHasAnim3D)
		{
			pMtrl->SetAnim3D(true); // Animation Mesh 알리기
			pMtrl->SetBoneCount(pMesh->GetBoneCount());
		}

		pMtrl->UpdateData_Inst();
		pMesh->render_instancing(pair.second[0].iMtrlIdx);

		// 정리
		if (bHasAnim3D)
		{
			pMtrl->SetAnim3D(false); // Animation Mesh 알리기
			pMtrl->SetBoneCount(0);
		}
	}

	// 개별 랜더링
	for (auto& pair : m_mapSingleObj)
	{
		if (pair.second.empty())
			continue;

		pair.second[0].pObj->Transform()->UpdateData();

		for (auto& instObj : pair.second)
		{
			instObj.pObj->GetRenderComponent()->render(instObj.iMtrlIdx);
		}

		if (pair.second[0].pObj->Animator3D())
		{
			pair.second[0].pObj->Animator3D()->ClearData();
		}
	}
}



void CCamera::render_forward()
{
	for (auto& pair : m_mapSingleObj)
	{
		pair.second.clear();
	}

	// Deferred object render
	tInstancingData tInstData = {};

	for (auto& pair : m_mapInstGroup_F)
	{
		// 그룹 오브젝트가 없거나, 쉐이더가 없는 경우
		if (pair.second.empty())
			continue;

		// instancing 개수 조건 미만이거나
		// Animation2D 오브젝트거나(스프라이트 애니메이션 오브젝트)
		// Shader 가 Instancing 을 지원하지 않는경우
		if (pair.second.size() <= 1
			|| pair.second[0].pObj->Animator2D()
			|| pair.second[0].pObj->GetRenderComponent()->GetCurMaterial(pair.second[0].iMtrlIdx)->GetShader()->GetVSInst() == nullptr)
		{
			// 해당 물체들은 단일 랜더링으로 전환
			for (UINT i = 0; i < pair.second.size(); ++i)
			{
				map<INT_PTR, vector<tInstObj>>::iterator iter
					= m_mapSingleObj.find((INT_PTR)pair.second[i].pObj);

				if (iter != m_mapSingleObj.end())
					iter->second.push_back(pair.second[i]);
				else
				{
					m_mapSingleObj.insert(make_pair((INT_PTR)pair.second[i].pObj, vector<tInstObj>{pair.second[i]}));
				}
			}
			continue;
		}

		CGameObject* pObj = pair.second[0].pObj;
		Ptr<CMesh> pMesh = pObj->GetRenderComponent()->GetMesh();
		Ptr<CMaterial> pMtrl = pObj->GetRenderComponent()->GetCurMaterial(pair.second[0].iMtrlIdx);

		// Instancing 버퍼 클리어
		CInstancingBuffer::GetInst()->Clear();

		int iRowIdx = 0;
		bool bHasAnim3D = false;
		for (UINT i = 0; i < pair.second.size(); ++i)
		{
			tInstData.matWorld = pair.second[i].pObj->Transform()->GetWorldMat();
			tInstData.matWV = tInstData.matWorld * m_matView;
			tInstData.matWVP = tInstData.matWV * m_matProj;

			if (pair.second[i].pObj->Animator3D())
			{
				pair.second[i].pObj->Animator3D()->UpdateData();
				tInstData.iRowIdx = iRowIdx++;
				CInstancingBuffer::GetInst()->AddInstancingBoneMat(pair.second[i].pObj->Animator3D()->GetFinalBoneMat());
				bHasAnim3D = true;
			}
			else
				tInstData.iRowIdx = -1;

			CInstancingBuffer::GetInst()->AddInstancingData(tInstData);
		}

		// 인스턴싱에 필요한 데이터를 세팅(SysMem -> GPU Mem)
		CInstancingBuffer::GetInst()->SetData();

		if (bHasAnim3D)
		{
			pMtrl->SetAnim3D(true); // Animation Mesh 알리기
			pMtrl->SetBoneCount(pMesh->GetBoneCount());
		}

		pMtrl->UpdateData_Inst();
		pMesh->render_instancing(pair.second[0].iMtrlIdx);

		// 정리
		if (bHasAnim3D)
		{
			pMtrl->SetAnim3D(false); // Animation Mesh 알리기
			pMtrl->SetBoneCount(0);
		}
	}

	// 개별 랜더링
	for (auto& pair : m_mapSingleObj)
	{
		if (pair.second.empty())
			continue;

		pair.second[0].pObj->Transform()->UpdateData();

		for (auto& instObj : pair.second)
		{
			instObj.pObj->GetRenderComponent()->render(instObj.iMtrlIdx);
		}

		if (pair.second[0].pObj->Animator3D())
		{
			pair.second[0].pObj->Animator3D()->ClearData();
		}
	}
}

void CCamera::render_decal()
{
	for (size_t i = 0; i < m_vecDecal.size(); ++i)
	{
		m_vecDecal[i]->render();
	}
}

void CCamera::render_transparent()
{
	for (size_t i = 0; i < m_vecTransparent.size(); ++i)
	{
		m_vecTransparent[i]->render();
	}
}

void CCamera::render_postprocess()
{
	for (size_t i = 0; i < m_vecPostProcess.size(); ++i)
	{
		CRenderMgr::GetInst()->CopyRenderTarget();
		m_vecPostProcess[i]->render();
	}
}

void CCamera::render_depthmap()
{
	// 광원 카메라의 View, Proj 세팅
	g_transform.matView = m_matView;
	g_transform.matViewInv = m_matViewInv;
	g_transform.matProj = m_matProj;

	for (size_t i = 0; i < m_vecDynamicShadow.size(); ++i)
	{
		m_vecDynamicShadow[i]->GetRenderComponent()->render_depthmap();
	}
}

void CCamera::SaveToFile(FILE* _File)
{
	COMPONENT_TYPE type = GetType();
	fwrite(&type, sizeof(UINT), 1, _File);

	fwrite(&m_eProjType, sizeof(PROJ_TYPE), 1, _File);
	fwrite(&m_fAspectRatio, sizeof(float), 1, _File);
	fwrite(&m_fFar, sizeof(float), 1, _File);
	fwrite(&m_fScale, sizeof(float), 1, _File);
	fwrite(&m_iLayerMask, sizeof(UINT), 1, _File);
	fwrite(&m_iCamIdx, sizeof(int), 1, _File);
}

void CCamera::LoadFromFile(FILE* _File)
{
	fread(&m_eProjType, sizeof(PROJ_TYPE), 1, _File);
	fread(&m_fAspectRatio, sizeof(float), 1, _File);
	fread(&m_fFar, sizeof(float), 1, _File);
	fread(&m_fScale, sizeof(float), 1, _File);
	fread(&m_iLayerMask, sizeof(UINT), 1, _File);
	fread(&m_iCamIdx, sizeof(int), 1, _File);
}
