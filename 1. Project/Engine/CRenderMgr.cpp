#include "pch.h"
#include "CRenderMgr.h"

#include "GlobalComponent.h"

#include "CDevice.h"
#include "CConstBuffer.h"
#include "CStructuredBuffer.h"
#include "CLevel.h"

#include "CResMgr.h"
#include "CLevelMgr.h"
#include "CKeyMgr.h"
#include "CMRT.h"

#include "CHDR.h"
#include "CBloom.h"

CRenderMgr::CRenderMgr()
	: m_arrMRT{}
	, m_pLight2DBuffer(nullptr)
	, m_pLight3DBuffer(nullptr)
	, m_EditorCam(nullptr)
{
	m_pLight2DBuffer = new CStructuredBuffer();
	m_pLight2DBuffer->Create(sizeof(tLightInfo), 2, SB_TYPE::SRV_ONLY, nullptr, true);

	m_pLight3DBuffer = new CStructuredBuffer();
	m_pLight3DBuffer->Create(sizeof(tLightInfo), 2, SB_TYPE::SRV_ONLY, nullptr, true);

	m_HDR = (CHDR*)CResMgr::GetInst()->FindRes<CComputeShader>(L"HDRFirstPassshader").Get();
}

CRenderMgr::~CRenderMgr()
{
	if (nullptr != m_pLight2DBuffer)
		delete m_pLight2DBuffer;

	if (nullptr != m_pLight3DBuffer)
		delete m_pLight3DBuffer;

	Safe_Del_Array(m_arrMRT);
}


void CRenderMgr::tick()
{
	m_vecCam.clear();
	m_vecLight2D.clear();
	m_vecLight3D.clear();
}

void CRenderMgr::render()
{
	// 렌더타겟 지정
	//m_arrMRT[(UINT)MRT_TYPE::SWAPCHAIN]->OMSet();

	// 타켓 클리어 
	// 배경 색 지정해주는 부분
	//CDevice::GetInst()->TargetClear(Vec4(0.f, 0.f, 0.f, 0.f));


	// MRT 타겟 클리어
	ClearMRT();

	// NoiseTexture Update
	UpdateNoiseTexture();

	// 2D 광원 업데이트
	UpdateLight2D();

	// 3D 광원 업데이트
	UpdateLight3D();

	// GlobalData 업데이트
	static CConstBuffer* pGlobalCB = CDevice::GetInst()->GetConstBuffer(CB_TYPE::GLOBAL);
	pGlobalCB->SetData(&g_global);
	pGlobalCB->UpdateData(PIPELINE_STAGE::ALL_STAGE);
	pGlobalCB->UpdateData_CS();

	// 렌더링의 기준을 카메라로 설정
	CLevel* pCurLevel = CLevelMgr::GetInst()->GetCurLevel();

	//UpdateHLOOM();

	if (LEVEL_STATE::PLAY == pCurLevel->GetState())
	{
		render_game();
	}
	else
	{
		render_editor();
	}

	// Post Process


	// HDRrender
	UpdateHDR();
	
}

void CRenderMgr::render_game()
{
	// Dir 광원 시점에서 Shadow맵핑을 위한 DepthMap 생성
	render_dynamic_shadowdepth();

	// 렌더링의 기준을 카메라로 설정
	for (size_t i = 0; i < m_vecCam.size(); ++i)
	{
		m_vecCam[i]->render();

	}
}

void CRenderMgr::render_editor()
{
	assert(m_EditorCam);

	// Dir 광원 시점에서 Shadow맵핑을 위한 DepthMap 생성
	render_dynamic_shadowdepth();

	m_EditorCam->render();
}

void CRenderMgr::render_dynamic_shadowdepth()
{
	m_arrMRT[(UINT)MRT_TYPE::SHADOW]->OMSet();

	for (size_t i = 0; i < m_vecLight3D.size(); ++i)
	{
		if (LIGHT_TYPE::DIRECTIONAL == m_vecLight3D[i]->GetLightType())
			m_vecLight3D[i]->render_depthmap();
	}
}

void CRenderMgr::UpdateNoiseTexture()
{
	Ptr<CTexture> NoiseTex = CResMgr::GetInst()->FindRes<CTexture>(L"texture\\noise\\noise_03.jpg");
	NoiseTex->UpdateData(15, PIPELINE_STAGE::ALL_STAGE);
	NoiseTex->UpdateData_CS(15, true);

	g_global.vNoiseResolution = Vec2(NoiseTex->GetWidth(), NoiseTex->GetHeight());
}

void CRenderMgr::UpdateLight2D()
{
	// 버퍼 공간이 모자랄 경우 재할당
	if (m_pLight2DBuffer->GetElementCount() < (UINT)m_vecLight2D.size())
	{
		m_pLight2DBuffer->Create(m_pLight2DBuffer->GetElementSize(), (UINT)m_vecLight2D.size(), SB_TYPE::SRV_ONLY, nullptr, true);
	}

	// 광원정보를 구조화버퍼로 옮김
	static vector<tLightInfo> vecLight2DInfo;
	vecLight2DInfo.clear();

	for (size_t i = 0; i < m_vecLight2D.size(); ++i)
	{
		vecLight2DInfo.push_back(m_vecLight2D[i]->GetLightInfo());
	}

	m_pLight2DBuffer->SetData(vecLight2DInfo.data(), (UINT)vecLight2DInfo.size());

	// 구조화 버퍼를 t13 레지스터에 바인딩
	m_pLight2DBuffer->UpdateData(13, ALL_STAGE); 	

	// 광원 개수 글로벌 데이터에 업데이트
	g_global.iLight2DCount = (UINT)m_vecLight2D.size();
}

void CRenderMgr::UpdateLight3D()
{

	// 버퍼 공간이 모자랄 경우 재할당
	if (m_pLight3DBuffer->GetElementCount() < (UINT)m_vecLight3D.size())
	{
		m_pLight3DBuffer->Create(m_pLight3DBuffer->GetElementSize(), (UINT)m_vecLight3D.size(), SB_TYPE::SRV_ONLY, nullptr, true);
	}

	// 광원정보를 구조화버퍼로 옮김
	static vector<tLightInfo> vecLight3DInfo;
	vecLight3DInfo.clear();

	for (size_t i = 0; i < m_vecLight3D.size(); ++i)
	{
		vecLight3DInfo.push_back(m_vecLight3D[i]->GetLightInfo());
	}

	m_pLight3DBuffer->SetData(vecLight3DInfo.data(), (UINT)vecLight3DInfo.size());


	// 구조화 버퍼를 t14 레지스터에 바인딩
	m_pLight3DBuffer->UpdateData(14, PIPELINE_STAGE::ALL_STAGE);

	// 광원 개수 글로벌 데이터에 업데이트
	g_global.iLight3DCount = (UINT)m_vecLight3D.size();
}

void CRenderMgr::UpdateHDR()
{
	m_HDR = (CHDR*)CResMgr::GetInst()->FindRes<CComputeShader>(L"HDRFirstPassshader").Get();
	m_HDR->Execute();
	vector<float> a;
	a.resize(1024);

	m_HDR->GetStructuredBuffer()->GetData(a.data());

	

	m_HDR = (CHDR*)CResMgr::GetInst()->FindRes<CComputeShader>(L"HDRSecondPassshader").Get();
	//m_HDR->GetStructuredFirstBuffer()->SetData(a.data(), a.size());
	m_HDR->GetStructuredBuffer()->SetData(a.data(), a.size());
	//m_HDR->SetScalarParam();
	m_HDR->Execute();
	m_HDR->GetStructuredBuffer()->GetData(a.data());
	
	m_fLuminance = a[0];

}

void CRenderMgr::UpdateHLOOM()
{
	m_Bloom = (CBloom*)CResMgr::GetInst()->FindRes<CComputeShader>(L"HDRBloom").Get();

	m_Bloom->SetRenderTex(m_HDR->GetTexture());

	m_Bloom->Execute();

	int c = 0;
}


void CRenderMgr::CopyRenderTarget()
{
	static Ptr<CTexture> RTTex = CResMgr::GetInst()->FindRes<CTexture>(L"RenderTargetTex");

	// 바인딩 해제
	ID3D11ShaderResourceView* SRV = nullptr;
	CONTEXT->PSSetShaderResources(60, 1, &SRV);

	// 렌더타겟 복사받기
	CONTEXT->CopyResource(m_RTCopyTex->GetTex2D().Get(), RTTex->GetTex2D().Get());
	//m_RTCopyTex->GenerateMip(8);
	// 60 레지스터 바인딩
	m_RTCopyTex->UpdateData(60, PIPELINE_STAGE::PS);
	
}

CCamera* CRenderMgr::GetMainCam()
{
	if (LEVEL_STATE::PLAY == CLevelMgr::GetInst()->GetLevelState())
	{
		if (m_vecCam.empty())
			return nullptr;

		return m_vecCam[0];
	}
	else
	{
		return m_EditorCam;
	}
}