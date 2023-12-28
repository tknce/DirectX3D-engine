#include "pch.h"
#include "CBloom.h"

#include "CDevice.h"

CBloom::CBloom()
	: CComputeShader(1024, 1, 1)
{
	m_HDRBloomTex = CResMgr::GetInst()->FindRes<CTexture>(L"UAV_HDRBloomTex");
	
}

CBloom::~CBloom()
{
}

void CBloom::UpdateData()
{
	static Ptr<CTexture> RTTex = CResMgr::GetInst()->FindRes<CTexture>(L"RenderTargetTex");

	// ���ε� ����
	//ID3D11ShaderResourceView* SRV = nullptr;
	//CONTEXT->CSSetShaderResources(61, 1, &SRV);
	//
	//// ����Ÿ�� ����ޱ�
	//CONTEXT->CopyResource(m_HDRBloomTex->GetTex2D().Get(), RTTex->GetTex2D().Get());

	m_HDRBloomTex->UpdateData_CS(1, true);
	m_HDRBloomTex->UpdateData(62, PIPELINE_STAGE::ALL_STAGE);

	// �׷� �� ���
	m_iGroupX = 1024;
	m_iGroupY = 1;
	m_iGroupZ = 1;
}

void CBloom::Clear()
{
	m_HDRBloomTex->Clear(62);
}