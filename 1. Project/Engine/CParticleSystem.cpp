#include "pch.h"
#include "CParticleSystem.h"

#include "CTimeMgr.h"

#include "CResMgr.h"

#include "CDevice.h"
#include "CConstBuffer.h"
#include "CStructuredBuffer.h"
#include "CKeyMgr.h"

#include "CTransform.h"

CParticleSystem::CParticleSystem()
	: CRenderComponent(COMPONENT_TYPE::PARTICLESYSTEM)
	, m_iMaxCount(3000)
	, m_iAliveCount(300)
	, m_vStartScale(Vec4(50.f, 50.f, 1.f, 0.f))
	, m_vEndScale(Vec4(10.f, 10.f, 1.f, 0.f))
	, m_vStartColor(Vec4(0.24f, 0.28f, 0.8f, 1.f ))
	, m_vEndColor(Vec4(0.78f, 0.74f, 0.9f, 1.f))
	, m_vMinMaxSpeed(Vec2(100.f, 300.f))
	, m_vMinMaxLifeTime(Vec2(0.2f, 1.f))
	, m_fSpawnRange(200.f)
	, m_Frequency(5.f)
	, m_fAccTime(0.f)
	, m_ParticleBuffer(nullptr)
	, m_ParticleShare(nullptr)
	, m_WorldSpawn(1)
	, m_vSeta(Vec4(0,0,3.14/3,0))
	, m_vForce(0.f,400.f)
{
	SetMesh(CResMgr::GetInst()->FindRes<CMesh>(L"PointMesh"));
	SetSharedMaterial(CResMgr::GetInst()->FindRes<CMaterial>(L"ParticleRenderMtrl"),0);
	m_UpdateCS = (CParticleUpdateShader*)CResMgr::GetInst()->FindRes<CComputeShader>(L"ParticleUpdateShader").Get();

	GetCurMaterial(0)->SetTexParam(TEX_PARAM::TEX_0, CResMgr::GetInst()->FindRes<CTexture>(L"texture\\particle\\Sparks.png"));
	//GetCurMaterial()->SetTexParam(TEX_PARAM::TEX_0, CResMgr::GetInst()->FindRes<CTexture>(L"CartoonSmoke"));	
	//GetCurMaterial()->SetTexParam(TEX_PARAM::TEX_0, CResMgr::GetInst()->FindRes<CTexture>(L"SmallParticle"));
	

	// 구조화버퍼 생성
	m_ParticleBuffer = new CStructuredBuffer;
	m_ParticleBuffer->Create(sizeof(tParticle), m_iMaxCount, SB_TYPE::UAV_INC, nullptr);

	m_ParticleShare = new CStructuredBuffer;
	m_ParticleShare->Create(sizeof(tParticleShare), 1, SB_TYPE::UAV_INC, nullptr, true);
}

CParticleSystem::CParticleSystem(const CParticleSystem& _origin)
	: CRenderComponent(_origin)
	, m_iMaxCount(_origin.m_iMaxCount)
	, m_iAliveCount(_origin.m_iAliveCount)
	, m_vStartScale		(_origin.m_vStartScale)
	, m_vEndScale		(_origin.m_vEndScale)		
	, m_vStartColor		(_origin.m_vStartColor)		
	, m_vEndColor		(_origin.m_vEndColor)		
	, m_vMinMaxSpeed	(_origin.m_vMinMaxSpeed)	
	, m_vMinMaxLifeTime	(_origin.m_vMinMaxLifeTime)	
	, m_fSpawnRange		(_origin.m_fSpawnRange)		
	, m_Frequency		(_origin.m_Frequency)		
	, m_fAccTime		(0)		
	, m_ParticleBuffer(nullptr)
	, m_ParticleShare(nullptr)
	, m_WorldSpawn(1)
	, m_vSeta(_origin.m_vSeta)
{
	// 구조화버퍼 생성
	m_ParticleBuffer = new CStructuredBuffer;
	m_ParticleBuffer->Create(sizeof(tParticle), m_iMaxCount, SB_TYPE::UAV_INC, nullptr);

	m_ParticleShare = new CStructuredBuffer;
	m_ParticleShare->Create(sizeof(tParticleShare), 1, SB_TYPE::UAV_INC, nullptr, true);

	

	//SetMesh(GetMesh());
	//SetSharedMaterial(CResMgr::GetInst()->FindRes<CMaterial>(GetSharedMaterial()->GetKey()));
	//GetCurMaterial()->SetTexParam(TEX_0, GetCurMaterial()->GetTexture());

	m_UpdateCS = (CParticleUpdateShader*)CResMgr::GetInst()->FindRes<CComputeShader>(_origin.m_UpdateCS.Get()->GetKey()).Get();
	m_UpdateCS->SetParticleBuffer(m_ParticleBuffer);
	m_UpdateCS->SetParticleShareData(m_ParticleShare);
}

CParticleSystem::~CParticleSystem()
{
	if (nullptr != m_ParticleBuffer)
		delete m_ParticleBuffer;

	if (nullptr != m_ParticleShare)
		delete m_ParticleShare;
}

void CParticleSystem::finaltick()
{
	/*Vec3 vPos = Transform()->GetRelativePos();
	vPos.x += 500.f * DT;
	Transform()->SetRelativePos(vPos);*/
	m_vSeta = GetOwner()->Transform()->GetRelativeRotation();

	// 파티클 생성 시간
	float fAliveTime = 1.f / m_Frequency;

	// 누적시간
	m_fAccTime += DT;

	
	m_vForce -= Vec2(0.f, (100.f * DT));
	
	tParticleShare share = { 10, 0,m_vVelopcity,m_vForce };
	// 누적시간이 파티클 생성 시간을 넘어서면
	if (fAliveTime < m_fAccTime)
	{
		float f = (m_fAccTime / fAliveTime);
		UINT iAliveCount = (UINT)f;
		m_fAccTime = f - floorf(f);

		//tParticleShare share = { iAliveCount, };
		 share = { 10, 0,m_vVelopcity,m_vForce };

		m_ParticleShare->SetData(&share, 1);
	}
	else
	{
		share = { 0, 0,m_vVelopcity,m_vForce };
		m_ParticleShare->SetData(&share, 1);
	}
	
	if (m_ParticleBuffer->GetElementCount() < m_iMaxCount)
	{
		m_ParticleBuffer->Create(sizeof(tParticle), m_iMaxCount, SB_TYPE::UAV_INC, nullptr);
	}

	if (KEY_PRESSED(KEY::I))
	{
		m_vSeta.z += XM_PI * DT;
	}

	//m_UpdateCS->UpdateData

	m_UpdateCS->SetParticleBuffer(m_ParticleBuffer);
	m_UpdateCS->SetParticleShareData(m_ParticleShare);
	m_UpdateCS->SetWorldSpawn(m_WorldSpawn);
	m_UpdateCS->SetObjectWorldPos(Transform()->GetWorldPos());
	m_UpdateCS->SetSpawnRange(m_fSpawnRange);
	m_UpdateCS->SetMinMaxSpeed(m_vMinMaxSpeed);
	m_UpdateCS->SetMinMaxLifeTime(m_vMinMaxLifeTime);
	
	

	m_UpdateCS->Execute();
}

void CParticleSystem::render()
{
	// 위치정보 업데이트
	Transform()->UpdateData();
	m_ParticleBuffer->UpdateData(16, PIPELINE_STAGE::VS | PIPELINE_STAGE::GS | PIPELINE_STAGE::PS);
	
	// 재질 업데이트
	GetCurMaterial(0)->SetScalarParam(INT_1, &m_WorldSpawn);
	GetCurMaterial(0)->SetScalarParam(FLOAT_0, &m_vSeta.z);
	GetCurMaterial(0)->SetScalarParam(VEC4_0, &m_vStartScale);
	GetCurMaterial(0)->SetScalarParam(VEC4_1, &m_vEndScale);
	GetCurMaterial(0)->SetScalarParam(VEC4_2, &m_vStartColor);
	GetCurMaterial(0)->SetScalarParam(VEC4_3, &m_vEndColor);
	GetCurMaterial(0)->SetScalarParam(VEC2_3, &m_vVelopcity);
	GetCurMaterial(0)->UpdateData();

	// 렌더링
	GetMesh()->render_particle(m_iMaxCount);

	// 클리어
	m_ParticleBuffer->Clear();
}

void CParticleSystem::render(UINT _iSubset)
{
	render();
}

void CParticleSystem::SaveToFile(FILE* _File)
{
	CRenderComponent::SaveToFile(_File);

	// 컴퓨트 쉐이더 정보 저장
	bool bCS = !!m_UpdateCS.Get();

	fwrite(&bCS, sizeof(bool), 1, _File);

	if (bCS)
	{
		SaveWStringToFile(m_UpdateCS->GetKey(), _File);
	}

	// 파티클 옵션
	fwrite(&m_iMaxCount, sizeof(UINT), 1, _File);
	fwrite(&m_iAliveCount, sizeof(UINT), 1, _File);
	fwrite(&m_vStartScale, sizeof(Vec4), 1, _File);
	fwrite(&m_vEndScale, sizeof(Vec4), 1, _File);
	fwrite(&m_vStartColor, sizeof(Vec4), 1, _File);
	fwrite(&m_vEndColor, sizeof(Vec4), 1, _File);
	fwrite(&m_vMinMaxSpeed, sizeof(Vec2), 1, _File);
	fwrite(&m_vMinMaxLifeTime, sizeof(Vec2), 1, _File);
	fwrite(&m_fSpawnRange, sizeof(float), 1, _File);
	fwrite(&m_Frequency, sizeof(float), 1, _File);
	fwrite(&m_fAccTime, sizeof(float), 1, _File);
	fwrite(&m_WorldSpawn, sizeof(int), 1, _File);
}

void CParticleSystem::LoadFromFile(FILE* _File)
{
	CRenderComponent::LoadFromFile(_File);

	// 컴퓨트쉐이더 참조
	bool bCS = false;
	fread(&bCS, sizeof(bool), 1, _File);

	if (bCS)
	{
		wstring strKey;
		LoadWStringFromFile(strKey, _File);
		m_UpdateCS = dynamic_cast<CParticleUpdateShader*>(CResMgr::GetInst()->FindRes<CComputeShader>(strKey).Get());
	}

	// 파티클 옵션
	fread(&m_iMaxCount, sizeof(UINT), 1, _File);
	fread(&m_iAliveCount, sizeof(UINT), 1, _File);
	fread(&m_vStartScale, sizeof(Vec4), 1, _File);
	fread(&m_vEndScale, sizeof(Vec4), 1, _File);
	fread(&m_vStartColor, sizeof(Vec4), 1, _File);
	fread(&m_vEndColor, sizeof(Vec4), 1, _File);
	fread(&m_vMinMaxSpeed, sizeof(Vec2), 1, _File);
	fread(&m_vMinMaxLifeTime, sizeof(Vec2), 1, _File);
	fread(&m_fSpawnRange, sizeof(float), 1, _File);
	fread(&m_Frequency, sizeof(float), 1, _File);
	fread(&m_fAccTime, sizeof(float), 1, _File);
	fread(&m_WorldSpawn, sizeof(int), 1, _File);
}

