#include "pch.h"
#include "CDevice.h"

#include "CResMgr.h"
#include "CConstBuffer.h"

void CDevice::RenderSizeChange()
{
	// ����Ÿ�� �����
	CONTEXT->OMSetRenderTargets(0, nullptr, nullptr);
	
	m_pSwapChain.Get()->ResizeBuffers(1, // ����ۼ�
		0,// 0�� ��� ����ü�ΰ� ����Ǿ��ִ� ������ Ŭ���̾�Ʈ �ʺ�
		0, // 0�� ��� ����ü�ΰ� ����Ǿ��ִ� ������ Ŭ���̾�Ʈ ����
		DXGI_FORMAT_R8G8B8A8_UNORM, // ������� ���� rgba
		0);
}

// ���ο�
void CDevice::Test()
{
	// ��Ϳ� ������ index ����
	UINT i = 0;
	// Adapter�� ��Ƶ� ������ ����
	IDXGIAdapter* adapter = nullptr;

	ComPtr<IDXGIDevice> pDXGIDevice = nullptr;
	ComPtr<IDXGIAdapter> pAdapter =nullptr;
	ComPtr<IDXGIFactory> pFactory = nullptr;

	m_pDevice->QueryInterface(__uuidof(IDXGIDevice), (void**)pDXGIDevice.GetAddressOf());


	pDXGIDevice->GetParent(__uuidof(IDXGIAdapter), (void**)pAdapter.GetAddressOf());


	pAdapter->GetParent(__uuidof(IDXGIFactory), (void**)pFactory.GetAddressOf());



	// IDXGIFactory ��ü�� ���� ��͵��� �����Ѵ�. i�� index�� ����Ѵ�.
	// ���� index�� �ý��ۿ� �����ϴ� ����� ������ ���ų� �� ũ�ٸ� while���� �����Ѵ�.
	while (pFactory->EnumAdapters(i, &adapter) != DXGI_ERROR_NOT_FOUND)
	{
		// ��� ���� ����ü
		DXGI_ADAPTER_DESC desc;
		adapter->GetDesc(&desc);

		// ����� �̸��� ����Ѵ�. (GPU ����)
		wstring text = L"***Adapter: ";
		text += desc.Description;
		text += L"\n";

		MessageBox(nullptr, text.c_str(), L"�׷��� ī��" , MB_OK);
		

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

	UINT iDeviceFlag = D3D11_CREATE_DEVICE_DEBUG // ������� �����ϴ� ��ġ ���� �˸�
						| D3D11_CREATE_DEVICE_BGRA_SUPPORT; // BGRA�������� �ؽ��ĸ� ������ ���̶�� ���� �˸�
	D3D_FEATURE_LEVEL FeatureLevel = (D3D_FEATURE_LEVEL)0; // �ʿ��� ��ɵ� ������ �������� ���� 0 �̸� ��� ��� �� ��


	ID3D11Device* pDevice = nullptr;
	HRESULT hr = D3D11CreateDevice(nullptr, D3D_DRIVER_TYPE_HARDWARE, nullptr
									, iDeviceFlag, nullptr, 0
									, D3D11_SDK_VERSION
									, m_pDevice.GetAddressOf()
									, &FeatureLevel
									, m_pDeviceContext.GetAddressOf());
	
	UINT iQuality = 0;
	DEVICE->CheckMultisampleQualityLevels(DXGI_FORMAT_R8G8B8A8_UNORM, 4, &iQuality);


	// ����ü�� �����
	if (FAILED(CreateSwapchain()))
	{
		MessageBox(nullptr, L"����ü�� ���� ����", L"Device �ʱ�ȭ ����", MB_OK);
		return E_FAIL;
	}

	// RenderTarget, DepthStencilTarget �����
	if (FAILED(CreateTarget()))
	{
		MessageBox(nullptr, L"Ÿ�� ���� ����", L"Device �ʱ�ȭ ����", MB_OK);
		return E_FAIL;
	}


	// ViewPort
	m_tViewPort.TopLeftX = 0; // ������ ���� ��ġ
	m_tViewPort.TopLeftY = 0;
	m_tViewPort.Width = _vResolution.x;
	m_tViewPort.Height = _vResolution.y;
	m_tViewPort.MinDepth = 0.f; // ���� �� ����
	m_tViewPort.MaxDepth = 1.f;

	m_pDeviceContext->RSSetViewports(1, &m_tViewPort);


	// ���÷� ���� �� ���ε�
	if (FAILED(CreateSampler()))
	{
		MessageBox(nullptr, L"���÷� ���� ����", L"Device �ʱ�ȭ ����", MB_OK);
		return E_FAIL;
	}


	// ������� ����
	if (FAILED(CreateConstBuffer()))
	{
		MessageBox(nullptr, L"������� ���� ����", L"Device �ʱ�ȭ ����", MB_OK);
		return E_FAIL;
	}

	// �����Ͷ����� ������Ʈ ����
	if (FAILED(CreateRasterizerState()))
	{
		MessageBox(nullptr, L"�����Ͷ����� ������Ʈ ���� ����", L"Device �ʱ�ȭ ����", MB_OK);
		return E_FAIL;
	}

	// DepthStencil ������Ʈ ����
	if (FAILED(CreateDepthStencilState()))
	{
		MessageBox(nullptr, L"�X�� ���ٽ� ������Ʈ ���� ����", L"Device �ʱ�ȭ ����", MB_OK);
		return E_FAIL;
	}


	// ���� ������Ʈ ����
	if (FAILED(CreateBlendState()))
	{
		MessageBox(nullptr, L"���� ������Ʈ ���� ����", L"Device �ʱ�ȭ ����", MB_OK);
		return E_FAIL;
	}

	return S_OK;
}


int CDevice::CreateSwapchain()
{
	DXGI_SWAP_CHAIN_DESC desc = {};

	desc.OutputWindow = m_hWnd;	// Front Buffer �� ��½�ų ������ �ڵ�
	desc.Windowed = true;		// ������, ��üȭ�� ���
	desc.BufferCount = 1;		// ����ü�� ���� ��
	desc.SwapEffect = DXGI_SWAP_EFFECT_DISCARD; // ���� ������ ����� �������� �ʴ´�.

	desc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT; // �� ������ ǥ�� ��뷮 �� CPU �׼��� �ɼ��� ���� - ǥ�� �Ǵ� ���ҽ��� ��� ������ ������� ����մϴ�.
	desc.BufferDesc.Width = (UINT)m_vRenderResolution.x;
	desc.BufferDesc.Height = (UINT)m_vRenderResolution.y;
	desc.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM; // �ȼ� �� ����	
	desc.BufferDesc.RefreshRate.Numerator = 60; // ȭ�� �ֻ���
	desc.BufferDesc.RefreshRate.Denominator = 1; // 60 / 1 �ʴ� 60������
	desc.BufferDesc.Scaling = DXGI_MODE_SCALING_UNSPECIFIED; // �������� �ʴ� ����
	desc.BufferDesc.ScanlineOrdering = DXGI_MODE_SCANLINE_ORDER_UNSPECIFIED;// ���� ���� - ������ �������� ����

	desc.SampleDesc.Count = 1; // �ȼ� �� ��Ƽ ������ ��
	desc.SampleDesc.Quality = 0; // ǰ�� ���� 1�� ����������� ������ ������

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
	// RenderTargetTexture �� ����ü�����κ��� �����ϱ�
	ComPtr<ID3D11Texture2D> tex2D;
	hr = m_pSwapChain->GetBuffer(0, __uuidof(ID3D11Texture2D), (void**)tex2D.GetAddressOf());

	// RenderTargetTexture ���
	m_pRenderTargetTex = CResMgr::GetInst()->CreateTexture(L"RenderTargetTex", tex2D);


	// DepthStencilTexture ����
	m_pDepthStencilTex = CResMgr::GetInst()->CreateTexture(L"DepthStencilTex"
				, (UINT)m_vRenderResolution.x, (UINT)m_vRenderResolution.y
				, DXGI_FORMAT_D24_UNORM_S8_UINT, D3D11_BIND_DEPTH_STENCIL);
	
	return hr;
}

int CDevice::CreateSampler()
{
	HRESULT hr = S_OK;

	// ���÷� ������Ʈ ����
	D3D11_SAMPLER_DESC desc = {};

	desc.AddressU = D3D11_TEXTURE_ADDRESS_MODE::D3D11_TEXTURE_ADDRESS_WRAP;
	desc.AddressV = D3D11_TEXTURE_ADDRESS_MODE::D3D11_TEXTURE_ADDRESS_WRAP;
	desc.AddressW = D3D11_TEXTURE_ADDRESS_MODE::D3D11_TEXTURE_ADDRESS_WRAP;

	desc.Filter = D3D11_FILTER_ANISOTROPIC;
	desc.MaxLOD = D3D11_FLOAT32_MAX;

	hr = DEVICE->CreateSamplerState(&desc, m_arrSampler[(UINT)SAMPLER_TYPE::ANISOTROPIC].GetAddressOf());
	

	desc.Filter = D3D11_FILTER_MIN_MAG_MIP_POINT;
	hr = DEVICE->CreateSamplerState(&desc, m_arrSampler[(UINT)SAMPLER_TYPE::POINT].GetAddressOf());


	// ���÷� ���ε�
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
	//	BOOL BlendEnable;	// �⺻�� : FALSE.
	//  ȥ���� �� ���ΰ�
	//	D3D11_BLEND SrcBlend;	// �⺻�� : D3D11_BLEND_ONE.
	//  �ҽ� �ؽ�ó�� ������ ����ϴµ� ���
	//	D3D11_BLEND DestBlend;	// �⺻�� : D3D11_BLEND_ZERO.
	//  �ҽ� �ؽ�ó�� ������ ����ϴµ� ���
	//	D3D11_BLEND_OP BlendOp;	// �⺻�� : D3D11_BLEND_OP_ADD.
	//	�ռ� Src,DestBlend�� ���� �����ϴ� ��� ����
	//	D3D11_BLEND SrcBlendAlpha;	// �⺻�� : D3D11_BLEND_ONE.
	//	D3D11_BLEND DestBlendAlpha;	// �⺻�� : D3D11_BLEND_ZERO.
	//	D3D11_BLEND_OP BlendOpAlpha;	// �⺻�� : D3D11_BLEND_OP_ADD.
	//	UINT8 RenderTargetWriteMask;	// �⺻�� : D3D11_COLOR_WRITE_ENABLE_ALL.
	//} 	D3D11_RENDER_TARGET_BLEND_DESC;

	// Default ����	
	desc.AlphaToCoverageEnable = false;
	desc.IndependentBlendEnable = false;

	desc.RenderTarget[0].BlendEnable = true;
	desc.RenderTarget[0].BlendOp = D3D11_BLEND_OP::D3D11_BLEND_OP_ADD; // ���İ��� ���ϰڴ�.
	desc.RenderTarget[0].SrcBlend = D3D11_BLEND_ONE;	// ȥ�հ�� 0
	desc.RenderTarget[0].DestBlend = D3D11_BLEND_ZERO;	// ȥ�հ�� 1

	desc.RenderTarget[0].BlendOpAlpha = D3D11_BLEND_OP::D3D11_BLEND_OP_ADD; // ���ϰڴ�.
	desc.RenderTarget[0].SrcBlendAlpha = D3D11_BLEND_ONE;	// ȥ�հ�� 1
	desc.RenderTarget[0].DestBlendAlpha = D3D11_BLEND_ZERO;	// ȥ�հ�� 0

	desc.RenderTarget[0].RenderTargetWriteMask = D3D11_COLOR_WRITE_ENABLE_ALL;

	hr = DEVICE->CreateBlendState(&desc, m_arrBS[(UINT)BS_TYPE::DEFAULT].GetAddressOf());

	

	// Alpha Blend
	desc.AlphaToCoverageEnable = false;
	desc.IndependentBlendEnable = false;
	

	desc.RenderTarget[0].BlendEnable = true;
	desc.RenderTarget[0].BlendOp = D3D11_BLEND_OP::D3D11_BLEND_OP_ADD;
	desc.RenderTarget[0].SrcBlend = D3D11_BLEND_SRC_ALPHA;		// ���� �����͸� ����.
	desc.RenderTarget[0].DestBlend = D3D11_BLEND_INV_SRC_ALPHA;	// ȥ�� �� 1 - RGB�� �����Ѵ�.

	desc.RenderTarget[0].BlendOpAlpha = D3D11_BLEND_OP::D3D11_BLEND_OP_ADD;
	desc.RenderTarget[0].SrcBlendAlpha = D3D11_BLEND_ONE;		// ȥ�հ�� 1
	desc.RenderTarget[0].DestBlendAlpha = D3D11_BLEND_ZERO;		// ȥ�հ�� 0

	desc.RenderTarget[0].RenderTargetWriteMask = D3D11_COLOR_WRITE_ENABLE_ALL;




	hr = DEVICE->CreateBlendState(&desc, m_arrBS[(UINT)BS_TYPE::ALPHABLEND].GetAddressOf());
	

	// One One Blend
	desc.AlphaToCoverageEnable = false;
	desc.IndependentBlendEnable = false;

	desc.RenderTarget[0].BlendEnable = true;
	desc.RenderTarget[0].BlendOp = D3D11_BLEND_OP::D3D11_BLEND_OP_ADD;
	desc.RenderTarget[0].SrcBlend = D3D11_BLEND_ONE;	// ȥ�հ�� 1
	desc.RenderTarget[0].DestBlend = D3D11_BLEND_ONE;	// ȥ�հ�� 0
	desc.RenderTarget[0].RenderTargetWriteMask = D3D11_COLOR_WRITE_ENABLE_ALL;

	DEVICE->CreateBlendState(&desc, m_arrBS[(UINT)BS_TYPE::ONE_ONE].GetAddressOf());


	return hr;
}

void CDevice::Present()
{	
	m_pSwapChain->Present(0, 0);
}