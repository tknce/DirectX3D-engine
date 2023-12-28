#include "pch.h"

#include "CAnimator3D.h"
#include "CTransform.h"

#include "CTimeMgr.h"
#include "CMeshRender.h"
#include "CStructuredBuffer.h"
#include "CResMgr.h"

#include "CAnimation3D.h"

#include "CAnimation3DShader.h"

#include "CKeyMgr.h"


CAnimator3D::CAnimator3D()
	: m_pVecBones(nullptr)
	, m_pVecClip(nullptr)
	, m_iCurClip(0)
	, m_dCurTime(0.)
	, m_iFrameCount(30)
	, m_pBoneFinalMatBuffer(nullptr)
	, m_pBoneOriginMatBuffer(nullptr)
	, m_bFinalMatUpdate(false)
	, m_iFrameIdx(0)
	, m_iNextFrameIdx(0)
	, m_fRatio(0.f)
	, CComponent(COMPONENT_TYPE::ANIMATOR3D)
{
	m_pBoneFinalMatBuffer = new CStructuredBuffer;
	m_pBoneOriginMatBuffer = new CStructuredBuffer;
}

CAnimator3D::CAnimator3D(const CAnimator3D& _origin)
	: m_vecClipUpdateTime(_origin.m_vecClipUpdateTime)
	, m_pVecBones(_origin.m_pVecBones)
	, m_pVecClip(_origin.m_pVecClip)
	, m_iCurClip(_origin.m_iCurClip)
	, m_dCurTime(_origin.m_dCurTime)
	, m_iFrameCount(_origin.m_iFrameCount)
	, m_pBoneFinalMatBuffer(nullptr)
	, m_pBoneOriginMatBuffer(nullptr)
	, m_bFinalMatUpdate(false)
	, m_iFrameIdx(_origin.m_iFrameIdx)
	, m_iNextFrameIdx(_origin.m_iNextFrameIdx)
	, m_fRatio(_origin.m_fRatio)
	, m_bStop(false)
	, CComponent(COMPONENT_TYPE::ANIMATOR3D)
{
	m_pBoneFinalMatBuffer = new CStructuredBuffer;
	m_pBoneOriginMatBuffer = new CStructuredBuffer;
}

CAnimator3D::~CAnimator3D()
{
	SAFE_DELETE(m_pBoneFinalMatBuffer);
	SAFE_DELETE(m_pBoneOriginMatBuffer);
	Safe_Del_Map(m_mapAnim);
}


void CAnimator3D::finaltick()
{
	m_dCurTime = 0.f;

	// 현재 재생중인 Clip 의 시간을 진행한다.
	if (!m_bStop)
		m_vecClipUpdateTime[m_iCurClip] += DT;


	if (m_pCurAnim != nullptr)
	{
		// 시간 리셋
		if (m_vecClipUpdateTime[m_iCurClip] >= m_pCurAnim->GetAnim3D()[0].dTimeLength)
		{
			m_vecClipUpdateTime[m_iCurClip] = 0.f;
		}
	}
	else
	{
		// 시간 리셋
		if (m_vecClipUpdateTime[m_iCurClip] >= m_pVecClip->at(m_iCurClip).dTimeLength)
		{
			m_vecClipUpdateTime[m_iCurClip] = 0.f;
		}
	}



	// 등록된 애니메이션이 있을 경우
	if (m_pCurAnim != nullptr)
	{
		m_dCurTime = m_pCurAnim->GetAnim3D()[0].dStartTime + m_vecClipUpdateTime[m_iCurClip];
	}
	else
	{
		m_dCurTime = m_pVecClip->at(m_iCurClip).dStartTime + m_vecClipUpdateTime[m_iCurClip];
	}


	// 현재 프레임 인덱스 구하기
	double dFrameIdx = m_dCurTime * (double)m_iFrameCount;
	m_iFrameIdx = (int)(dFrameIdx);

	// 다음 프레임 인덱스
	if (m_iFrameIdx >= m_pVecClip->at(0).iFrameLength - 1)
		m_iNextFrameIdx = m_iFrameIdx;	// 끝이면 현재 인덱스를 유지
	else
		m_iNextFrameIdx = m_iFrameIdx + 1;

	// 프레임간의 시간에 따른 비율을 구해준다.
	m_fRatio = (float)(dFrameIdx - (double)m_iFrameIdx);

	// 컴퓨트 쉐이더 연산여부
	m_bFinalMatUpdate = false;
}

void CAnimator3D::UpdateData()
{


	if (!m_bFinalMatUpdate)
	{
		// Animation3D Update Compute Shader
		CAnimation3DShader* pUpdateShader = (CAnimation3DShader*)CResMgr::GetInst()->FindRes<CComputeShader>(L"Animation3DUpdateShader").Get();

		// Bone Data
		Ptr<CMesh> pMesh = MeshRender()->GetMesh();
		check_mesh(pMesh);
		
		pUpdateShader->SetFrameDataBuffer(pMesh->GetBoneFrameDataBuffer());
		pUpdateShader->SetOffsetMatBuffer(pMesh->GetBoneOffsetBuffer());
		pUpdateShader->SetOutputBuffer(m_pBoneFinalMatBuffer);
		pUpdateShader->SetOutputOriginBuffer(m_pBoneOriginMatBuffer);


		UINT iBoneCount = (UINT)m_pVecBones->size();
		pUpdateShader->SetBoneCount(iBoneCount);
		pUpdateShader->SetFrameIndex(m_iFrameIdx);
		pUpdateShader->SetNextFrameIdx(m_iNextFrameIdx);
		pUpdateShader->SetFrameRatio(m_fRatio);

		// 업데이트 쉐이더 실행
		pUpdateShader->Execute();



		m_bFinalMatUpdate = true;
	}
	//static vector<CGameObject*> v;
	//static bool b = false;
	//if (KEY_TAP(KEY::H))
	//	b = true;
	//if (b)
	//	v.push_back(GetOwner());
	// t30 레지스터에 최종행렬 데이터(구조버퍼) 바인딩		
	m_pBoneFinalMatBuffer->UpdateData(30, PIPELINE_STAGE::VS);
	//m_pBoneOriginMatBuffer->UpdateData(31, PIPELINE_STAGE::VS);
}

void CAnimator3D::SetAnimClip(const vector<tMTAnimClip>* _vecAnimClip)
{
	m_pVecClip = _vecAnimClip;
	m_vecClipUpdateTime.resize(m_pVecClip->size());

	static float fTime = 0.f;
	fTime += 1.f;
	m_vecClipUpdateTime[0] = fTime;
}

void CAnimator3D::SetMeshData()
{
	Ptr<CMeshData> meshdata = CResMgr::GetInst()->FindRes<CMeshData>(m_strMeshData);
	GetOwner()->MeshRender()->SetMesh(meshdata->GetMesh());

	for (UINT i = 0; i < meshdata->GetMaterial().size(); ++i)
	{
		GetOwner()->MeshRender()->SetSharedMaterial(meshdata->GetMaterial()[i], i);
	}
	// Animation 파트 추가
	SetBones(meshdata->GetMesh()->GetBones());
	SetAnimClip(meshdata->GetMesh()->GetAnimClip());
}


void CAnimator3D::ClearData()
{
	m_pBoneFinalMatBuffer->Clear();
	//m_pBoneOriginMatBuffer->Clear();
	UINT iMtrlCount = MeshRender()->GetMtrlCount();
	Ptr<CMaterial> pMtrl = nullptr;
	for (UINT i = 0; i < iMtrlCount; ++i)
	{
		pMtrl = MeshRender()->GetSharedMaterial(i);
		if (nullptr == pMtrl)
			continue;

		pMtrl->SetAnim3D(false); // Animation Mesh 알리기
		pMtrl->SetBoneCount(0);
	}
}

void CAnimator3D::SetAnimation3D(wstring _strKey, tMTAnimClip information)
{
	CAnimation3D* pAnim = FindAnimation3D(_strKey);
	assert(!pAnim);

	pAnim = new CAnimation3D;
	pAnim->SetAnim3D(information);
	pAnim->SetName(_strKey);

	pAnim->SetOwner(this);
	m_mapAnim.insert(make_pair(pAnim->GetName(), pAnim));
}

CAnimation3D* CAnimator3D::FindAnimation3D(wstring _strKey)
{
	map<wstring, CAnimation3D*>::iterator iter = m_mapAnim.find(_strKey);

	if (iter == m_mapAnim.end())
		return nullptr;

	return iter->second;
}

void CAnimator3D::Play(wstring _strKey)
{
	CAnimation3D* pAnim = FindAnimation3D(_strKey);

	if (IsValid(pAnim))
	{
		m_pCurAnim = pAnim;
	}
}






void CAnimator3D::check_mesh(Ptr<CMesh> _pMesh)
{
	UINT iBoneCount = _pMesh->GetBoneCount();
	if (m_pBoneFinalMatBuffer->GetElementCount() != iBoneCount)
	{
		m_pBoneFinalMatBuffer->Create(sizeof(Matrix), iBoneCount, SB_TYPE::UAV_INC, nullptr, false);
		m_pBoneOriginMatBuffer->Create(sizeof(Matrix), iBoneCount, SB_TYPE::UAV_INC, nullptr, false);
	}
}

void CAnimator3D::SaveToFile(FILE* _pFile)
{
	COMPONENT_TYPE type = GetType();
	fwrite(&type, sizeof(UINT), 1, _pFile);

	// meshdata불러오기
	size_t iLen = m_strMeshData.length();
	fwrite(&iLen, sizeof(size_t), 1, _pFile);
	fwrite(m_strMeshData.data(), sizeof(wchar_t), iLen, _pFile);

	// Animation 저장
	size_t iAnimCount = m_mapAnim.size();
	fwrite(&iAnimCount, sizeof(size_t), 1, _pFile);

	for (const auto& pair : m_mapAnim)
	{
		pair.second->SaveToFile(_pFile);
	}
}

void CAnimator3D::LoadFromFile(FILE* _pFile)
{
	size_t iLen = 0;
	fread(&iLen, sizeof(size_t), 1, _pFile);
	wchar_t szBuffer[256] = {};
	fread(szBuffer, sizeof(wchar_t), iLen, _pFile);

	// Animation
	size_t iAnimCount = 0;
	fread(&iAnimCount, sizeof(size_t), 1, _pFile);


	m_strMeshData = szBuffer;

	for (size_t i = 0; i < iAnimCount; ++i)
	{
		CAnimation3D* pAnim = new CAnimation3D;
		pAnim->LoadFromFile(_pFile);

		pAnim->m_pOwner = this;
		m_mapAnim.insert(make_pair(pAnim->GetName(), pAnim));
	}
}