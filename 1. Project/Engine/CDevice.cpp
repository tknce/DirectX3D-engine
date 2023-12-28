#include "pch.h"
#include "CDevice.h"

#include "CResMgr.h"
#include "CConstBuffer.h"

void CDevice::RenderSizeChange()
{
	// 랜더타겟 비워둠
	CONTEXT->OMSetRenderTargets(0, nullptr, nullptr);
	
	m_pSwapChain.Get()->ResizeBuffers(1, // 백버퍼수
		0,// 0일 경우 스왑체인과 연결되어있는 윈도우 클라이언트 너비
		0, // 0일 경우 스왑체인과 연결되어있는 윈도우 클라이언트 높이
		DXGI_FORMAT_R8G8B8A8_UNORM, // 백버퍼의 포맷 rgba
		0);
}

// 공부용
void CDevice::Test()
{
	// 어뎁터에 접근할 index 변수
	UINT i = 0;
	// Adapter를 담아둘 포인터 변수
	IDXGIAdapter* adapter = nullptr;

	ComPtr<IDXGIDevice> pDXGIDevice = nullptr;
	ComPtr<IDXGIAdapter> pAdapter =nullptr;
	ComPtr<IDXGIFactory> pFactory = nullptr;

	m_pDevice->QueryInterface(__uuidof(IDXGIDevice), (void**)pDXGIDevice.GetAddressOf());


	pDXGIDevice->GetParent(__uuidof(IDXGIAdapter), (void**)pAdapter.GetAddressOf());


	pAdapter->GetParent(__uuidof(IDXGIFactory), (void**)pFactory.GetAddressOf());



	// IDXGIFactory 객체를 통해 어뎁터들을 열거한다. i를 index로 사용한다.
	// 만약 index가 시스템에 존재하는 어뎁터의 갯수와 같거나 더 크다면 while문을 종료한다.
	while (pFactory->EnumAdapters(i, &adapter) != DXGI_ERROR_NOT_FOUND)
	{
		// 어뎁터 정보 구조체
		DXGI_ADAPTER_DESC desc;
		adapter->GetDesc(&desc);

		// 어뎁터의 이름을 출력한다. (GPU 정보)
		wstring text = L"***Adapter: ";
		text += desc.Description;
		text += L"\n";

		MessageBox(nullptr, text.c_str(), L"그래픽 카드" , MB_OK);
		

		++i;
	}
}

CDevice::CDevice()
	: m_hWnd(nullptr)	
	, m_tViewPort{}
	, m_arrCB{}
	, m_arrRS{}
	, m_arrDS{}
	, m_arrBS{}
{
}

CDevice::~CDevice()
{
	for (UINT i = 0; i < (UINT)CB_TYPE::END; ++i)
	{
		if (nullptr != m_arrCB[i])
			delete m_arrCB[i];
	}
}

int CDevice::init(HWND _hWnd, Vec2 _vResolution)
{
	m_hWnd = _hWnd;
	m_vRenderResolution = _vResolution;
	g_global.vRenderResolution = m_vRenderResolution;

	UINT iDeviceFlag = D3D11_CREATE_DEVICE_DEBUG // 디버깅을 지원하는 장치 생성 알림
						| D3D11_CREATE_DEVICE_BGRA_SUPPORT; // BGRA형식으로 텍스쳐를 구성할 것이라는 것을 알림
	D3D_FEATURE_LEVEL FeatureLevel = (D3D_FEATURE_LEVEL)0; // 필요한 기능들 어디까지 쓰는지에 대해 0 이면 모든 기능 다 씀


	ID3D11Device* pDevice = nullptr;
	HRESULT hr = D3D11CreateDevice(nullptr, D3D_DRIVER_TYPE_HARDWARE, nullptr
									, iDeviceFlag, nullptr, 0
									, D3D11_SDK_VERSION
									, m_pDevice.GetAddressOf()
									, &FeatureLevel
									, m_pDeviceContext.GetAddressOf());
	
	UINT iQuality = 0;
	DEVICE->CheckMultisampleQualityLevels(DXGI_FORMAT_R8G8B8A8_UNORM, 4, &iQuality);


	// 스왚체인 만들기
	if (FAILED(CreateSwapchain()))
	{
		MessageBox(nullptr, L"스왚체인 생성 실패", L"Device 초기화 실패", MB_OK);
		return E_FAIL;
	}

	// RenderTarget, DepthStencilTarget 만들기
	if (FAILED(CreateTarget()))
	{
		MessageBox(nullptr, L"타겟 생성 실패", L"Device 초기화 실패", MB_OK);
		return E_FAIL;
	}


	// ViewPort
	m_tViewPort.TopLeftX = 0; // 왼쪽위 시작 위치
	m_tViewPort.TopLeftY = 0;
	m_tViewPort.Width = _vResolution.x;
	m_tViewPort.Height = _vResolution.y;
	m_tViewPort.MinDepth = 0.f; // 깊이 값 설정
	m_tViewPort.MaxDepth = 1.f;

	m_pDeviceContext->RSSetViewports(1, &m_tViewPort);


	// 샘플러 생성 및 바인딩
	if (FAILED(CreateSampler()))
	{
		MessageBox(nullptr, L"샘플러 생성 실패", L"Device 초기화 실패", MB_OK);
		return E_FAIL;
	}


	// 상수버퍼 생성
	if (FAILED(CreateConstBuffer()))
	{
		MessageBox(nullptr, L"상수버퍼 생성 실패", L"Device 초기화 실패", MB_OK);
		return E_FAIL;
	}

	// 레스터라이저 스테이트 생성
	if (FAILED(CreateRasterizerState()))
	{
		MessageBox(nullptr, L"레스터라이저 스테이트 생성 실패", L"Device 초기화 실패", MB_OK);
		return E_FAIL;
	}

	// DepthStencil 스테이트 생성
	if (FAILED(CreateDepthStencilState()))
	{
		MessageBox(nullptr, L"뎊스 스텐실 스테이트 생성 실패", L"Device 초기화 실패", MB_OK);
		return E_FAIL;
	}


	// 블렌드 스테이트 생성
	if (FAILED(CreateBlendState()))
	{
		MessageBox(nullptr, L"블렌드 스테이트 생성 실패", L"Device 초기화 실패", MB_OK);
		return E_FAIL;
	}

	return S_OK;
}


int CDevice::CreateSwapchain()
{
	DXGI_SWAP_CHAIN_DESC desc = {};

	desc.OutputWindow = m_hWnd;	// Front Buffer 를 출력시킬 윈도우 핸들
	desc.Windowed = true;		// 윈도우, 전체화면 모드
	desc.BufferCount = 1;		// 스왑체인 버퍼 수
	desc.SwapEffect = DXGI_SWAP_EFFECT_DISCARD; // 이전 프레임 장면을 유지하지 않는다.

	desc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT; // 백 버퍼의 표면 사용량 및 CPU 액세스 옵션을 설명 - 표면 또는 리소스를 출력 렌더링 대상으로 사용합니다.
	desc.BufferDesc.Width = (UINT)m_vRenderResolution.x;
	desc.BufferDesc.Height = (UINT)m_vRenderResolution.y;
	desc.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM; // 픽셀 당 포멧	
	desc.BufferDesc.RefreshRate.Numerator = 60; // 화면 주사율
	desc.BufferDesc.RefreshRate.Denominator = 1; // 60 / 1 초당 60프레임
	desc.BufferDesc.Scaling = DXGI_MODE_SCALING_UNSPECIFIED; // 지정되지 않는 조정
	desc.BufferDesc.ScanlineOrdering = DXGI_MODE_SCANLINE_ORDER_UNSPECIFIED;// 순서 정리 - 순서가 지정되지 않음

	desc.SampleDesc.Count = 1; // 픽셀 당 멀티 샘플의 수
	desc.SampleDesc.Quality = 0; // 품질 수준 1이 가까워질수록 성능이 낮아짐

	ComPtr<IDXGIDevice> pDXGIDevice = nullptr;
	ComPtr<IDXGIAdapter> pAdapter = nullptr;
	ComPtr<IDXGIFactory> pFactory = nullptr;
	

	if (FAILED(m_pDevice->QueryInterface(__uuidof(IDXGIDevice), (void**)pDXGIDevice.GetAddressOf())))
	{
		return E_FAIL;
	}
	
	if (FAILED(pDXGIDevice->GetParent(__uuidof(IDXGIAdapter), (void**)pAdapter.GetAddressOf())))
	{
		return E_FAIL;
	}

	if (FAILED(pAdapter->GetParent(__uuidof(IDXGIFactory), (void**)pFactory.GetAddressOf())))
	{
		return E_FAIL;
	}

	if (FAILED(pFactory->CreateSwapChain(m_pDevice.Get(), &desc, m_pSwapChain.GetAddressOf())))
	{
		return E_FAIL;
	}

	return S_OK;
}

int CDevice::CreateTarget()
{
	HRESULT hr = S_OK;
	// RenderTargetTexture 를 스왚체인으로부터 참조하기
	ComPtr<ID3D11Texture2D> tex2D;
	hr = m_pSwapChain->GetBuffer(0, __uuidof(ID3D11Texture2D), (void**)tex2D.GetAddressOf());

	// RenderTargetTexture 등록
	m_pRenderTargetTex = CResMgr::GetInst()->CreateTexture(L"RenderTargetTex", tex2D);


	// DepthStencilTexture 생성
	m_pDepthStencilTex = CResMgr::GetInst()->CreateTexture(L"DepthStencilTex"
				, (UINT)m_vRenderResolution.x, (UINT)m_vRenderResolution.y
				, DXGI_FORMAT_D24_UNORM_S8_UINT, D3D11_BIND_DEPTH_STENCIL);
	
	return hr;
}

int CDevice::CreateSampler()
{
	HRESULT hr = S_OK;

	// 샘플러 스테이트 생성
	D3D11_SAMPLER_DESC desc = {};

	desc.AddressU = D3D11_TEXTURE_ADDRESS_MODE::D3D11_TEXTURE_ADDRESS_WRAP;
	desc.AddressV = D3D11_TEXTURE_ADDRESS_MODE::D3D11_TEXTURE_ADDRESS_WRAP;
	desc.AddressW = D3D11_TEXTURE_ADDRESS_MODE::D3D11_TEXTURE_ADDRESS_WRAP;

	desc.Filter = D3D11_FILTER_ANISOTROPIC;
	desc.MaxLOD = D3D11_FLOAT32_MAX;

	hr = DEVICE->CreateSamplerState(&desc, m_arrSampler[(UINT)SAMPLER_TYPE::ANISOTROPIC].GetAddressOf());
	

	desc.Filter = D3D11_FILTER_MIN_MAG_MIP_POINT;
	hr = DEVICE->CreateSamplerState(&desc, m_arrSampler[(UINT)SAMPLER_TYPE::POINT].GetAddressOf());


	// 샘플러 바인딩
	CONTEXT->VSSetSamplers((UINT)SAMPLER_TYPE::ANISOTROPIC, 1, m_arrSampler[(UINT)SAMPLER_TYPE::ANISOTROPIC].GetAddressOf());
	CONTEXT->HSSetSamplers((UINT)SAMPLER_TYPE::ANISOTROPIC, 1, m_arrSampler[(UINT)SAMPLER_TYPE::ANISOTROPIC].GetAddressOf());
	CONTEXT->DSSetSamplers((UINT)SAMPLER_TYPE::ANISOTROPIC, 1, m_arrSampler[(UINT)SAMPLER_TYPE::ANISOTROPIC].GetAddressOf());
	CONTEXT->GSSetSamplers((UINT)SAMPLER_TYPE::ANISOTROPIC, 1, m_arrSampler[(UINT)SAMPLER_TYPE::ANISOTROPIC].GetAddressOf());
	CONTEXT->PSSetSamplers((UINT)SAMPLER_TYPE::ANISOTROPIC, 1, m_arrSampler[(UINT)SAMPLER_TYPE::ANISOTROPIC].GetAddressOf());
	CONTEXT->CSSetSamplers((UINT)SAMPLER_TYPE::ANISOTROPIC, 1, m_arrSampler[(UINT)SAMPLER_TYPE::ANISOTROPIC].GetAddressOf());

	CONTEXT->VSSetSamplers((UINT)SAMPLER_TYPE::POINT, 1, m_arrSampler[(UINT)SAMPLER_TYPE::POINT].GetAddressOf());
	CONTEXT->HSSetSamplers((UINT)SAMPLER_TYPE::POINT, 1, m_arrSampler[(UINT)SAMPLER_TYPE::POINT].GetAddressOf());
	CONTEXT->DSSetSamplers((UINT)SAMPLER_TYPE::POINT, 1, m_arrSampler[(UINT)SAMPLER_TYPE::POINT].GetAddressOf());
	CONTEXT->GSSetSamplers((UINT)SAMPLER_TYPE::POINT, 1, m_arrSampler[(UINT)SAMPLER_TYPE::POINT].GetAddressOf());
	CONTEXT->PSSetSamplers((UINT)SAMPLER_TYPE::POINT, 1, m_arrSampler[(UINT)SAMPLER_TYPE::POINT].GetAddressOf());
	CONTEXT->CSSetSamplers((UINT)SAMPLER_TYPE::POINT, 1, m_arrSampler[(UINT)SAMPLER_TYPE::POINT].GetAddressOf());
	
	return hr;
}

int CDevice::CreateConstBuffer()
{
	m_arrCB[(UINT)CB_TYPE::TRANSFORM] = new CConstBuffer(CB_TYPE::TRANSFORM);
	m_arrCB[(UINT)CB_TYPE::TRANSFORM]->Create(sizeof(tTransform));

	m_arrCB[(UINT)CB_TYPE::MATERIAL] = new CConstBuffer(CB_TYPE::MATERIAL);
	m_arrCB[(UINT)CB_TYPE::MATERIAL]->Create(sizeof(tMtrlConst));

	m_arrCB[(UINT)CB_TYPE::GLOBAL] = new CConstBuffer(CB_TYPE::GLOBAL);
	m_arrCB[(UINT)CB_TYPE::GLOBAL]->Create(sizeof(tGlobalData));

	m_arrCB[(UINT)CB_TYPE::ANIMATION2D] = new CConstBuffer(CB_TYPE::ANIMATION2D);
	m_arrCB[(UINT)CB_TYPE::ANIMATION2D]->Create(sizeof(tAnim2DInfo));	
	
	return S_OK;
}

int CDevice::CreateRasterizerState()
{
	HRESULT hr = S_OK;

	m_arrRS[(UINT)RS_TYPE::CULL_BACK] = nullptr;
	

	D3D11_RASTERIZER_DESC desc = {};
	
	// Cull Front Mode
	desc.CullMode = D3D11_CULL_MODE::D3D11_CULL_FRONT;
	desc.FillMode = D3D11_FILL_MODE::D3D11_FILL_SOLID;

	hr = DEVICE->CreateRasterizerState(&desc, m_arrRS[(UINT)RS_TYPE::CULL_FRONT].GetAddressOf());


	// Cull None Mode
	desc.CullMode = D3D11_CULL_MODE::D3D11_CULL_NONE;
	desc.FillMode = D3D11_FILL_MODE::D3D11_FILL_SOLID;

	hr = DEVICE->CreateRasterizerState(&desc, m_arrRS[(UINT)RS_TYPE::CULL_NONE].GetAddressOf());


	// WireFrame Mode
	desc.CullMode = D3D11_CULL_MODE::D3D11_CULL_NONE;
	desc.FillMode = D3D11_FILL_MODE::D3D11_FILL_WIREFRAME;

	hr = DEVICE->CreateRasterizerState(&desc, m_arrRS[(UINT)RS_TYPE::WIRE_FRAME].GetAddressOf());


	return hr;
}

int CDevice::CreateDepthStencilState()
{
	HRESULT hr = S_OK;

	// Less
	m_arrDS[(UINT)DS_TYPE::LESS] = nullptr;


	D3D11_DEPTH_STENCIL_DESC desc = {};

	// Less Equal
	desc.DepthEnable = true;
	desc.DepthFunc = D3D11_COMPARISON_FUNC::D3D11_COMPARISON_LESS_EQUAL;
	desc.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ALL;
	desc.StencilEnable = false;

	DEVICE->CreateDepthStencilState(&desc, m_arrDS[(UINT)DS_TYPE::LESS_EQUAL].GetAddressOf());
	
	// Greater
	desc.DepthEnable = true;
	desc.DepthFunc = D3D11_COMPARISON_FUNC::D3D11_COMPARISON_GREATER;
	desc.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ALL;
	desc.StencilEnable = false;

	DEVICE->CreateDepthStencilState(&desc, m_arrDS[(UINT)DS_TYPE::GREATER].GetAddressOf());

	// NO_WRITE
	desc.DepthEnable = true;
	desc.DepthFunc = D3D11_COMPARISON_FUNC::D3D11_COMPARISON_LESS;
	desc.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ZERO;

	desc.StencilEnable = false;

	DEVICE->CreateDepthStencilState(&desc, m_arrDS[(UINT)DS_TYPE::NO_WRITE].GetAddressOf());

	// NO_TEST_NO_WRITE
	desc.DepthEnable = false;	
	desc.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ZERO;
	desc.StencilEnable = false;

	DEVICE->CreateDepthStencilState(&desc, m_arrDS[(UINT)DS_TYPE::NO_TEST_NO_WRITE].GetAddressOf());

	return hr;
}

int CDevice::CreateBlendState()
{
	HRESULT hr = S_OK;
	D3D11_BLEND_DESC desc = {};


	//typedef struct D3D11_RENDER_TARGET_BLEND_DESC
	//{
	//	BOOL BlendEnable;	// 기본값 : FALSE.
	//  혼합을 할 것인가
	//	D3D11_BLEND SrcBlend;	// 기본값 : D3D11_BLEND_ONE.
	//  소스 텍스처의 색상을 계산하는데 사용
	//	D3D11_BLEND DestBlend;	// 기본값 : D3D11_BLEND_ZERO.
	//  소스 텍스처의 색상을 계산하는데 사용
	//	D3D11_BLEND_OP BlendOp;	// 기본값 : D3D11_BLEND_OP_ADD.
	//	앞선 Src,DestBlend에 대한 결합하는 방법 정의
	//	D3D11_BLEND SrcBlendAlpha;	// 기본값 : D3D11_BLEND_ONE.
	//	D3D11_BLEND DestBlendAlpha;	// 기본값 : D3D11_BLEND_ZERO.
	//	D3D11_BLEND_OP BlendOpAlpha;	// 기본값 : D3D11_BLEND_OP_ADD.
	//	UINT8 RenderTargetWriteMask;	// 기본값 : D3D11_COLOR_WRITE_ENABLE_ALL.
	//} 	D3D11_RENDER_TARGET_BLEND_DESC;

	// Default 블렌딩	
	desc.AlphaToCoverageEnable = false;
	desc.IndependentBlendEnable = false;

	desc.RenderTarget[0].BlendEnable = true;
	desc.RenderTarget[0].BlendOp = D3D11_BLEND_OP::D3D11_BLEND_OP_ADD; // 알파값을 더하겠다.
	desc.RenderTarget[0].SrcBlend = D3D11_BLEND_ONE;	// 혼합계수 0
	desc.RenderTarget[0].DestBlend = D3D11_BLEND_ZERO;	// 혼합계수 1

	desc.RenderTarget[0].BlendOpAlpha = D3D11_BLEND_OP::D3D11_BLEND_OP_ADD; // 더하겠다.
	desc.RenderTarget[0].SrcBlendAlpha = D3D11_BLEND_ONE;	// 혼합계수 1
	desc.RenderTarget[0].DestBlendAlpha = D3D11_BLEND_ZERO;	// 혼합계수 0

	desc.RenderTarget[0].RenderTargetWriteMask = D3D11_COLOR_WRITE_ENABLE_ALL;

	hr = DEVICE->CreateBlendState(&desc, m_arrBS[(UINT)BS_TYPE::DEFAULT].GetAddressOf());

	

	// Alpha Blend
	desc.AlphaToCoverageEnable = false;
	desc.IndependentBlendEnable = false;
	

	desc.RenderTarget[0].BlendEnable = true;
	desc.RenderTarget[0].BlendOp = D3D11_BLEND_OP::D3D11_BLEND_OP_ADD;
	desc.RenderTarget[0].SrcBlend = D3D11_BLEND_SRC_ALPHA;		// 알파 데이터를 쓴다.
	desc.RenderTarget[0].DestBlend = D3D11_BLEND_INV_SRC_ALPHA;	// 혼합 전 1 - RGB를 생성한다.

	desc.RenderTarget[0].BlendOpAlpha = D3D11_BLEND_OP::D3D11_BLEND_OP_ADD;
	desc.RenderTarget[0].SrcBlendAlpha = D3D11_BLEND_ONE;		// 혼합계수 1
	desc.RenderTarget[0].DestBlendAlpha = D3D11_BLEND_ZERO;		// 혼합계수 0

	desc.RenderTarget[0].RenderTargetWriteMask = D3D11_COLOR_WRITE_ENABLE_ALL;




	hr = DEVICE->CreateBlendState(&desc, m_arrBS[(UINT)BS_TYPE::ALPHABLEND].GetAddressOf());
	

	// One One Blend
	desc.AlphaToCoverageEnable = false;
	desc.IndependentBlendEnable = false;

	desc.RenderTarget[0].BlendEnable = true;
	desc.RenderTarget[0].BlendOp = D3D11_BLEND_OP::D3D11_BLEND_OP_ADD;
	desc.RenderTarget[0].SrcBlend = D3D11_BLEND_ONE;	// 혼합계수 1
	desc.RenderTarget[0].DestBlend = D3D11_BLEND_ONE;	// 혼합계수 0
	desc.RenderTarget[0].RenderTargetWriteMask = D3D11_COLOR_WRITE_ENABLE_ALL;

	DEVICE->CreateBlendState(&desc, m_arrBS[(UINT)BS_TYPE::ONE_ONE].GetAddressOf());


	return hr;
}

void CDevice::Present()
{	
	m_pSwapChain->Present(0, 0);
}