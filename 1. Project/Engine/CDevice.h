#pragma once

#include "Ptr.h"
#include "CTexture.h"

class CConstBuffer;

class CDevice
	: public CSingleton<CDevice>
{	
private:
	HWND							m_hWnd;					// 윈도우핸들
	
	ComPtr<ID3D11Device>			m_pDevice;				// GPU 메모리 관리
	ComPtr<ID3D11DeviceContext>		m_pDeviceContext;		// GPU Rendering

	Ptr<CTexture>					m_pRenderTargetTex;		// 그릴 곳
	Ptr<CTexture>					m_pDepthStencilTex;		// 깊이 판정

	ComPtr<IDXGISwapChain>			m_pSwapChain;			// RenderTarget(FrontBuffer, BackBuffer) 를 관리 및 역할 교체 지시
	D3D11_VIEWPORT					m_tViewPort;			// 백버퍼를 윈도우에 그릴 영역(위치, 크기) 지정

	Vec2							m_vRenderResolution;	// 렌더링 버퍼 해상도

	ComPtr<ID3D11SamplerState>		m_arrSampler[(UINT)SAMPLER_TYPE::END]; // 쉐이더들에게 바인딩할 때 값을 넘겨주는 곳

	CConstBuffer*					m_arrCB[(UINT)CB_TYPE::END]; // 상수버퍼 쉐이더에서 쓸 값들을 항상 전달할 값들을 넣어주는 버퍼
	
	ComPtr<ID3D11RasterizerState>	m_arrRS[(UINT)RS_TYPE::END];	// 레스터라이즈 단계 생성 정점쉐이더에서 픽셀쉐이더로 갈 때 보간하는 단계
	ComPtr<ID3D11DepthStencilState>	m_arrDS[(UINT)DS_TYPE::END];	// 뎁스스탠실 단계 생성 깊이 판정 단계 여러 가지 활용 가능
	ComPtr<ID3D11BlendState>		m_arrBS[(UINT)BS_TYPE::END];	// 색 섞기 여부
	




public:
	int init(HWND _hWnd, Vec2 _vResolution);

	void Present();

private:
	int CreateSwapchain();
	int CreateTarget();
	int CreateSampler();
	int CreateConstBuffer();
	int CreateRasterizerState();
	int CreateDepthStencilState();
	int CreateBlendState();


public:
	ID3D11Device* GetDevice() { return m_pDevice.Get(); }
	ID3D11DeviceContext* GetContext() {return m_pDeviceContext.Get(); }
	CConstBuffer* GetConstBuffer(CB_TYPE _eType){return m_arrCB[(UINT)_eType];}

	ComPtr<ID3D11RasterizerState> GetRasterizerState(RS_TYPE _eType) { return m_arrRS[(UINT)_eType]; }
	ComPtr<ID3D11BlendState> GetBlendState(BS_TYPE _eType) { return m_arrBS[(UINT)_eType]; }
	ComPtr<ID3D11DepthStencilState> GetDepthStencilState(DS_TYPE _eType) { return m_arrDS[(UINT)_eType]; }

	Vec2 GetRenderResolution() { return m_vRenderResolution; }

	void RenderSizeChange();

	void Test();

public:
	CDevice();
	~CDevice();
};

