#include "pch.h"
#include "CMRT.h"
#include "CDevice.h"
#include "CTexture.h"

CMRT::CMRT()
	: m_arrTarget{}
	, m_RTCount(0)
	, m_tViewPort{}
{
}

CMRT::~CMRT()
{
}

void CMRT::Create(Ptr<CTexture>(&_arrTex)[8], Vec4(&arrClear)[8], Ptr<CTexture> _pDesTex)
{
	for (int i = 0; i < 8; ++i)
	{
		if (nullptr == _arrTex[i].Get())
		{
			m_RTCount = i;
			break;
		}

		m_arrTarget[i] = _arrTex[i];
	}

	for (int i = 0; i < m_RTCount; ++i)
	{
		m_arrClearColor[i] = arrClear[i];
	}

	m_DSTex = _pDesTex;

	// ViewPort ����	
	m_tViewPort.TopLeftX = 0;
	m_tViewPort.TopLeftY = 0;

	m_tViewPort.Width = m_arrTarget[0]->GetWidth();
	m_tViewPort.Height = m_arrTarget[0]->GetHeight();

	m_tViewPort.MinDepth = 0;
	m_tViewPort.MaxDepth = 1;
}

void CMRT::Clear()
{
	for (int i = 0; i < m_RTCount; ++i)
	{
		CONTEXT->ClearRenderTargetView(m_arrTarget[i]->GetRTV().Get(), m_arrClearColor[i]);
	}

	if (m_DSTex.Get())
		CONTEXT->ClearDepthStencilView(m_DSTex->GetDSV().Get(), D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, 1.f, 0.f);
}

void CMRT::OMSet()
{
	CMaterial::Clear();
	ID3D11RenderTargetView* arrRTV[8] = {};
	for (int i = 0; i < m_RTCount; ++i)
	{
		arrRTV[i] = m_arrTarget[i]->GetRTV().Get();
	}

	if (nullptr != m_DSTex.Get())
		CONTEXT->OMSetRenderTargets(m_RTCount, arrRTV, m_DSTex->GetDSV().Get());
	else
		CONTEXT->OMSetRenderTargets(m_RTCount, arrRTV, nullptr);

	// ViewPort ����
	CONTEXT->RSSetViewports(1, &m_tViewPort);
}