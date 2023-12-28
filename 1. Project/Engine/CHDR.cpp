#include "pch.h"
#include "CHDR.h"

#include "CStructuredBuffer.h"
#include "CDevice.h"

CHDR::CHDR()
	: CComputeShader(1024, 1, 1)
{
	m_HDR = new CStructuredBuffer;
	m_HDR->Create(sizeof(float),1024,SB_TYPE::UAV_INC,nullptr, true);

	m_firstHDR = new CStructuredBuffer;
	m_firstHDR->Create(sizeof(float), 1024, SB_TYPE::UAV_INC, nullptr, true);
	m_pTargetTex = CResMgr::GetInst()->FindRes<CTexture>(L"UAV_HDRTex");
}

CHDR::~CHDR()
{
	delete(m_HDR);
	delete(m_firstHDR);
}

void CHDR::UpdateData()
{
	
	m_firstHDR->UpdateData_CS(0, false);
	// 구조화버퍼를 u1에 등록
	m_HDR->UpdateData_CS(1, false);

	static Ptr<CTexture> RTTex = CResMgr::GetInst()->FindRes<CTexture>(L"RenderTargetTex");

	// 바인딩 해제
	ID3D11ShaderResourceView* SRV = nullptr;
	CONTEXT->CSSetShaderResources(61, 1, &SRV);

	// 렌더타겟 복사받기
	CONTEXT->CopyResource(m_pTargetTex->GetTex2D().Get(), RTTex->GetTex2D().Get());
	
	m_pTargetTex->UpdateData_CS(61, true);
	m_pTargetTex->UpdateData_CS(2, true);



	// 그룹 수 계산
	m_iGroupX = 1024;
	m_iGroupY = 1;
	m_iGroupZ = 1;
}

void CHDR::Clear()
{
	
	//m_HDR->Clear();
}




