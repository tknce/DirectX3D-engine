#include "pch.h"
#include "CTexture.h"

#include "CDevice.h"


CTexture::CTexture(bool _bEngineRes)
    : CRes(RES_TYPE::TEXTURE, _bEngineRes)
    , m_Desc{}
{
}

CTexture::~CTexture()
{
}

void CTexture::Save(const wstring& _strRelativePath)
{
}

int CTexture::Load_oldver(const wstring& _strFilePath)
{
    HRESULT hr = E_FAIL;
    
    wchar_t szExt[50] = {};
    _wsplitpath_s(_strFilePath.c_str(), nullptr, 0, nullptr, 0, nullptr, 0, szExt, 50);

    wstring strExt = szExt;

    if (strExt == L".dds" || strExt == L".DDS")
    {
        hr = LoadFromDDSFile(_strFilePath.c_str(), DDS_FLAGS::DDS_FLAGS_NONE, nullptr, m_Image);
    }

    else if (strExt == L".tga" || strExt == L".TGA")
    {
        hr = LoadFromTGAFile(_strFilePath.c_str(), nullptr, m_Image);
    }

    else // WIC (png, jpg, jpeg, bmp )
    {
        hr = LoadFromWICFile(_strFilePath.c_str(), WIC_FLAGS::WIC_FLAGS_NONE, nullptr, m_Image);
    }

    if (FAILED(hr))
        return hr;


    // sysmem -> GPU
    hr = CreateShaderResourceView(DEVICE
        , m_Image.GetImages()
        , m_Image.GetImageCount()
        , m_Image.GetMetadata()
        , m_SRV.GetAddressOf());


    m_SRV->GetResource((ID3D11Resource**)m_Tex2D.GetAddressOf());
    m_Tex2D->GetDesc(&m_Desc);

   //vector<vector<tBGRA>> a ;
   //GetPixelVector(a);

   //WIdthSearch(a, Vec2(50,50));


    return hr;
}
int CTexture::Load(const wstring& _strFilePath)
{
    return Load(_strFilePath, 1);
}

int CTexture::Load(const wstring& _strFilePath, int _iMipLevel)
{
    wchar_t strBuff[50] = {};
    _wsplitpath_s(_strFilePath.c_str(), 0, 0, 0, 0, 0, 0, strBuff, 50);
    wstring strExt = strBuff;

    HRESULT hRet = S_OK;

    if (strExt == L".dds" || strExt == L".DDS")
    {
        // .dds .DDS
        hRet = LoadFromDDSFile(_strFilePath.c_str(), DDS_FLAGS_FORCE_RGB, nullptr, m_Image);
    }
    else if (strExt == L".tga" || strExt == L".TGA")
    {
        // .tga .TGA
        hRet = LoadFromTGAFile(_strFilePath.c_str(), nullptr, m_Image);
    }
    else
    {
        // .png .jpg .jpeg .bmp
        hRet = LoadFromWICFile(_strFilePath.c_str(), WIC_FLAGS_NONE, nullptr, m_Image);
    }

    if (FAILED(hRet))
    {
        wsprintf(strBuff, L"에러코드 : %d", hRet);
        MessageBox(nullptr, strBuff, L"텍스쳐 로딩 실패", MB_OK);
        return hRet;
    }

    // Texture2D 생성
    m_Desc.Format = m_Image.GetMetadata().format;

    if (m_Image.GetMetadata().IsCubemap())
    {
        m_Desc.MipLevels = 1;	// 0 ==> 가능한 모든 밉맵이 저장 될 수 있는 공간이 만들어짐
        m_Desc.BindFlags = D3D11_BIND_SHADER_RESOURCE;
        m_Desc.MiscFlags = D3D11_RESOURCE_MISC_TEXTURECUBE;
    }
    else
    {
        m_Desc.MipLevels = _iMipLevel;// MAX_MIP;	// 0 ==> 가능한 모든 밉맵이 저장 될 수 있는 공간이 만들어짐	
        m_Desc.BindFlags = D3D11_BIND_RENDER_TARGET | D3D11_BIND_SHADER_RESOURCE;
        m_Desc.MiscFlags = D3D11_RESOURCE_MISC_GENERATE_MIPS;
    }

    m_Desc.ArraySize = m_Image.GetMetadata().arraySize;

    m_Desc.SampleDesc.Count = 1;
    m_Desc.SampleDesc.Quality = 0;

    m_Desc.Usage = D3D11_USAGE_DEFAULT;
    m_Desc.CPUAccessFlags = 0;

    m_Desc.Width = m_Image.GetMetadata().width;
    m_Desc.Height = m_Image.GetMetadata().height;

    HRESULT hr = DEVICE->CreateTexture2D(&m_Desc, nullptr, m_Tex2D.GetAddressOf());
    if (FAILED(hr))
    {
        hr = Load_oldver(_strFilePath);
        if (FAILED(hr))
        {
            MessageBox(nullptr, L"Texture 생성 실패", L"Texture 생성 실패", MB_OK);
            assert(nullptr);
        }
        return 0;
    }
    // 원본데이터(밉맵 레벨 0) 를 각 칸에 옮긴다.	
    for (int i = 0; i < m_Desc.ArraySize; ++i)
    {
        // GPU 에 데이터 옮기기(밉맵 포함)
        UINT iSubresIdx = D3D11CalcSubresource(0, i, m_Desc.MipLevels);

        CONTEXT->UpdateSubresource(m_Tex2D.Get(), iSubresIdx, nullptr
            , m_Image.GetImage(0, i, 0)->pixels
            , m_Image.GetImage(0, i, 0)->rowPitch
            , m_Image.GetImage(0, i, 0)->slicePitch);
    }

    // SRV 생성
    D3D11_SHADER_RESOURCE_VIEW_DESC viewdesc = {};
    viewdesc.Format = m_Desc.Format;

    if (m_Image.GetMetadata().IsCubemap())
    {
        viewdesc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURECUBE;
    }
    else if (1 < m_Desc.ArraySize)
    {
        viewdesc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2DARRAY;
    }
    else
    {
        viewdesc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2D;
    }

    if (1 < m_Desc.ArraySize)
    {
        viewdesc.Texture2DArray.ArraySize = m_Desc.ArraySize;
        viewdesc.Texture2DArray.MipLevels = m_Desc.MipLevels;
        viewdesc.Texture2DArray.MostDetailedMip = 0;
        
    }
    else
    {
        viewdesc.Texture2DArray.ArraySize = 1;
        viewdesc.Texture2D.MipLevels = m_Desc.MipLevels;
        viewdesc.Texture2D.MostDetailedMip = 0;
    }


    DEVICE->CreateShaderResourceView(m_Tex2D.Get(), &viewdesc, m_SRV.GetAddressOf());

    // 밉맵 생성
    if (false == m_Image.GetMetadata().IsCubemap())
    {
        CONTEXT->GenerateMips(m_SRV.Get());
    }

    m_Tex2D->GetDesc(&m_Desc);
}

void CTexture::GenerateMip(UINT _iMipLevel)
{
    m_Tex2D = nullptr;
    m_SRV = nullptr;
    m_RTV = nullptr;
    m_DSV = nullptr;
    m_UAV = nullptr;

    // Texture2D 생성
    D3D11_TEXTURE2D_DESC tDesc = {};
    tDesc.Format = m_Image.GetMetadata().format;

    if (m_Image.GetMetadata().IsCubemap())
    {
        tDesc.MipLevels = 1;	// 0 ==> 가능한 모든 밉맵이 저장 될 수 있는 공간이 만들어짐
        tDesc.BindFlags = D3D11_BIND_SHADER_RESOURCE;
        tDesc.MiscFlags = D3D11_RESOURCE_MISC_TEXTURECUBE;
    }
    else
    {
        tDesc.MipLevels = _iMipLevel;// MAX_MIP;	// 0 ==> 가능한 모든 밉맵이 저장 될 수 있는 공간이 만들어짐	
        tDesc.BindFlags = D3D11_BIND_RENDER_TARGET | D3D11_BIND_SHADER_RESOURCE;
        tDesc.MiscFlags = D3D11_RESOURCE_MISC_GENERATE_MIPS;
    }

    tDesc.ArraySize = m_Image.GetMetadata().arraySize;

    tDesc.SampleDesc.Count = 1;
    tDesc.SampleDesc.Quality = 0;

    tDesc.Usage = D3D11_USAGE_DEFAULT;
    tDesc.CPUAccessFlags = 0;

    tDesc.Width = m_Image.GetMetadata().width;
    tDesc.Height = m_Image.GetMetadata().height;

    HRESULT hr = DEVICE->CreateTexture2D(&tDesc, nullptr, m_Tex2D.GetAddressOf());

    // 원본데이터(밉맵 레벨 0) 를 각 칸에 옮긴다.	
    for (int i = 0; i < tDesc.ArraySize; ++i)
    {
        // GPU 에 데이터 옮기기(밉맵 포함)
        UINT iSubresIdx = D3D11CalcSubresource(0, i, tDesc.MipLevels);

        CONTEXT->UpdateSubresource(m_Tex2D.Get(), iSubresIdx, nullptr
            , m_Image.GetImage(0, i, 0)->pixels
            , m_Image.GetImage(0, i, 0)->rowPitch
            , m_Image.GetImage(0, i, 0)->slicePitch);
    }

    // SRV 생성
    D3D11_SHADER_RESOURCE_VIEW_DESC viewdesc = {};
    viewdesc.Format = tDesc.Format;

    if (m_Image.GetMetadata().IsCubemap())
    {
        viewdesc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURECUBE;
    }
    else if (1 < tDesc.ArraySize)
    {
        viewdesc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2DARRAY;
    }
    else
    {
        viewdesc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2D;
    }

    if (1 < tDesc.ArraySize)
    {
        viewdesc.Texture2DArray.ArraySize = tDesc.ArraySize;
        viewdesc.Texture2DArray.MipLevels = tDesc.MipLevels;
        viewdesc.Texture2DArray.MostDetailedMip = 0;
    }
    else
    {
        viewdesc.Texture2DArray.ArraySize = 1;
        viewdesc.Texture2D.MipLevels = tDesc.MipLevels;
        viewdesc.Texture2D.MostDetailedMip = 0;
    }

    DEVICE->CreateShaderResourceView(m_Tex2D.Get(), &viewdesc, m_SRV.GetAddressOf());

    // 밉맵 생성
    if (false == m_Image.GetMetadata().IsCubemap())
    {
        CONTEXT->GenerateMips(m_SRV.Get());
    }

    m_Tex2D->GetDesc(&m_Desc);
}

void CTexture::Create(UINT _iWidth, UINT _iHeight, DXGI_FORMAT _Format, UINT _iBindFlag, bool _Cpu)
{
    // DepthStencilTexture
    m_Desc.BindFlags = _iBindFlag;
    if (_Cpu)
    {
        m_Desc.CPUAccessFlags = D3D11_CPU_ACCESS_READ;
    }
    else
    {
        m_Desc.Usage = D3D11_USAGE::D3D11_USAGE_DEFAULT;
        m_Desc.CPUAccessFlags = 0;
    }
    

    m_Desc.Format = _Format;
    m_Desc.Width = _iWidth;
    m_Desc.Height = _iHeight;
    m_Desc.ArraySize = 1;

    m_Desc.SampleDesc.Count = 1;
    m_Desc.SampleDesc.Quality = 0;

    m_Desc.MipLevels = 1;
    m_Desc.MiscFlags = 0;

    HRESULT hr = DEVICE->CreateTexture2D(&m_Desc, nullptr, m_Tex2D.GetAddressOf());
    assert(!FAILED(hr));

    CaptureTexture(DEVICE, CONTEXT, m_Tex2D.Get(), m_Image);

    // View 생성
    if (_iBindFlag & D3D11_BIND_DEPTH_STENCIL)
    {
        hr = DEVICE->CreateDepthStencilView(m_Tex2D.Get(), nullptr, m_DSV.GetAddressOf());
    }
    else
    {
        if (_iBindFlag & D3D11_BIND_RENDER_TARGET)
        {
            hr = DEVICE->CreateRenderTargetView(m_Tex2D.Get(), nullptr, m_RTV.GetAddressOf());
            assert(!FAILED(hr));
        }

        if (_iBindFlag & D3D11_BIND_SHADER_RESOURCE)
        {
            D3D11_SHADER_RESOURCE_VIEW_DESC tSRVDesc = {};
            tSRVDesc.Format = m_Desc.Format;
            tSRVDesc.Texture2D.MipLevels = 1;
            tSRVDesc.Texture2D.MostDetailedMip = 0;
            tSRVDesc.ViewDimension = D3D11_SRV_DIMENSION::D3D11_SRV_DIMENSION_TEXTURE2D;
            hr = DEVICE->CreateShaderResourceView(m_Tex2D.Get(), &tSRVDesc, m_SRV.GetAddressOf());
            assert(!FAILED(hr));
        }

        if (_iBindFlag & D3D11_BIND_UNORDERED_ACCESS)
        {
            D3D11_UNORDERED_ACCESS_VIEW_DESC tUAVDesc = {};
            tUAVDesc.Format = m_Desc.Format;
            tUAVDesc.Texture2D.MipSlice = 0;            
            tUAVDesc.ViewDimension = D3D11_UAV_DIMENSION::D3D11_UAV_DIMENSION_TEXTURE2D;
            hr = DEVICE->CreateUnorderedAccessView(m_Tex2D.Get(), &tUAVDesc, m_UAV.GetAddressOf());
            assert(!FAILED(hr));
        }
    }    
}

void CTexture::Create(ComPtr<ID3D11Texture2D> _Tex2D)
{
    m_Tex2D = _Tex2D;
    m_Tex2D->GetDesc(&m_Desc);

    // View 생성
    HRESULT hr = S_OK;
    if (m_Desc.BindFlags & D3D11_BIND_DEPTH_STENCIL)
    {
        hr = DEVICE->CreateDepthStencilView(m_Tex2D.Get(), nullptr, m_DSV.GetAddressOf());
    }
    else
    {
        if (m_Desc.BindFlags & D3D11_BIND_RENDER_TARGET)
        {
            hr = DEVICE->CreateRenderTargetView(m_Tex2D.Get(), nullptr, m_RTV.GetAddressOf());
            assert(!FAILED(hr));
        }

        if (m_Desc.BindFlags & D3D11_BIND_SHADER_RESOURCE)
        {
            D3D11_SHADER_RESOURCE_VIEW_DESC tSRVDesc = {};
            tSRVDesc.Format = m_Desc.Format;
            tSRVDesc.Texture2D.MipLevels = 1;
            tSRVDesc.Texture2D.MostDetailedMip = 0;
            tSRVDesc.ViewDimension = D3D11_SRV_DIMENSION::D3D11_SRV_DIMENSION_TEXTURE2D;
            hr = DEVICE->CreateShaderResourceView(m_Tex2D.Get(), &tSRVDesc, m_SRV.GetAddressOf());
            assert(!FAILED(hr));
        }

        if (m_Desc.BindFlags & D3D11_BIND_UNORDERED_ACCESS)
        {
            D3D11_UNORDERED_ACCESS_VIEW_DESC tUAVDesc = {};
            tUAVDesc.Format = m_Desc.Format;
            tUAVDesc.Texture2D.MipSlice = 0;
            tUAVDesc.ViewDimension = D3D11_UAV_DIMENSION::D3D11_UAV_DIMENSION_TEXTURE2D;
            hr = DEVICE->CreateUnorderedAccessView(m_Tex2D.Get(), &tUAVDesc, m_UAV.GetAddressOf());
            assert(!FAILED(hr));
        }
    }
}

void CTexture::CreateStaging(UINT _iWidth, UINT _iHeight, DXGI_FORMAT _Format)
{

}


int CTexture::CreateArrayTexture(const vector<Ptr<CTexture>>& _vecTex, int _iMapLevel)
{
    m_Desc = _vecTex[0]->GetDesc();
    m_Desc.ArraySize = (UINT)_vecTex.size();
    m_Desc.BindFlags = D3D11_BIND_RENDER_TARGET | D3D11_BIND_SHADER_RESOURCE;
    m_Desc.MipLevels = _iMapLevel;

    HRESULT hr = DEVICE->CreateTexture2D(&m_Desc, nullptr, m_Tex2D.GetAddressOf());
    if (FAILED(hr))
        return hr;

    // 원본데이터(밉맵 레벨 0) 를 각 칸에 옮긴다.	
    for (int i = 0; i < _vecTex.size(); ++i)
    {
        UINT iOffset = D3D11CalcSubresource(0, i, _iMapLevel);
        CONTEXT->UpdateSubresource(m_Tex2D.Get(), iOffset, nullptr, _vecTex[i]->GetSysMem()
            , _vecTex[i]->GetRowPitch(), _vecTex[i]->GetSlicePitch());

        // 배열텍스쳐 만들때 텍스쳐 뷰를 넣어두기
        m_vSRV.push_back(_vecTex[i].Get()->GetSRV());
    }

    // Shader Resource View 생성
    D3D11_SHADER_RESOURCE_VIEW_DESC viewdesc = {};
    viewdesc.Format = m_Desc.Format;
    viewdesc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2DARRAY;
    viewdesc.Texture2DArray.MipLevels = _iMapLevel;
    viewdesc.Texture2DArray.MostDetailedMip = 0;
    viewdesc.Texture2DArray.ArraySize = _vecTex.size();

    DEVICE->CreateShaderResourceView(m_Tex2D.Get(), &viewdesc, m_SRV.GetAddressOf());

    // 밉맵 생성
    CONTEXT->GenerateMips(m_SRV.Get());
    m_Tex2D->GetDesc(&m_Desc);

    return hr;

    
}

ComPtr<ID3D11ShaderResourceView> CTexture::CreateArrTexShaderResourceView(Ptr<CTexture> _Tex, int _iLevel)
{
     // assume this is your array texture
    Ptr<CTexture> arrayTexture; // assume this is your array texture
    ID3D11ShaderResourceView* textureSRV;

    D3D11_TEXTURE2D_DESC desc;
    desc = _Tex.Get()->GetDesc();

    if (desc.ArraySize >= _iLevel)
        _iLevel = 0;

    D3D11_SHADER_RESOURCE_VIEW_DESC srvDesc;
    srvDesc.Format = desc.Format;
    srvDesc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2DARRAY;
    srvDesc.Texture2DArray.MipLevels = desc.MipLevels;
    srvDesc.Texture2DArray.ArraySize = desc.ArraySize; //-_iLevel; // select 1 less than the full array size to skip the first texture
    srvDesc.Texture2DArray.FirstArraySlice = 1; // select the second texture in the array
    srvDesc.Texture2DArray.MostDetailedMip = 0;

    DEVICE->CreateShaderResourceView(m_Tex2D.Get(), &srvDesc, &textureSRV);

    return textureSRV;
}

void CTexture::UpdateData(UINT _iRegisterNum, UINT _iPipelineStage)
{
    if ((UINT)PIPELINE_STAGE::VS & _iPipelineStage)
        CONTEXT->VSSetShaderResources(_iRegisterNum, 1, m_SRV.GetAddressOf());

    if ((UINT)PIPELINE_STAGE::HS & _iPipelineStage)
        CONTEXT->HSSetShaderResources(_iRegisterNum, 1, m_SRV.GetAddressOf());

    if ((UINT)PIPELINE_STAGE::DS & _iPipelineStage)
        CONTEXT->DSSetShaderResources(_iRegisterNum, 1, m_SRV.GetAddressOf());

    if ((UINT)PIPELINE_STAGE::GS & _iPipelineStage)
        CONTEXT->GSSetShaderResources(_iRegisterNum, 1, m_SRV.GetAddressOf());

    if ((UINT)PIPELINE_STAGE::PS & _iPipelineStage)
        CONTEXT->PSSetShaderResources(_iRegisterNum, 1, m_SRV.GetAddressOf());
}

void CTexture::UpdateData_CS(UINT _iRegisterNum, bool _bShaderRes)
{
    if (_bShaderRes)
    {
        CONTEXT->CSSetShaderResources(_iRegisterNum, 1, m_SRV.GetAddressOf());
    }
    else
    {
        assert(m_UAV.Get());

        UINT i = -1;
        CONTEXT->CSSetUnorderedAccessViews(_iRegisterNum, 1, m_UAV.GetAddressOf(), &i);
    }
}

void CTexture::Clear(UINT _iRegisterNum)
{
    ID3D11ShaderResourceView* pSRV = nullptr;
    
    CONTEXT->VSSetShaderResources(_iRegisterNum, 1, &pSRV);
    CONTEXT->HSSetShaderResources(_iRegisterNum, 1, &pSRV);
    CONTEXT->DSSetShaderResources(_iRegisterNum, 1, &pSRV);
    CONTEXT->GSSetShaderResources(_iRegisterNum, 1, &pSRV);
    CONTEXT->PSSetShaderResources(_iRegisterNum, 1, &pSRV);
    CONTEXT->CSSetShaderResources(_iRegisterNum, 1, &pSRV);
}

void CTexture::Clear_CS(UINT _iRegisterNum)
{
    ID3D11ShaderResourceView* pSRV = nullptr;
    CONTEXT->CSSetShaderResources(_iRegisterNum, 1, &pSRV);


    ID3D11UnorderedAccessView* pUAV = nullptr;
    UINT i = -1;
    CONTEXT->CSSetUnorderedAccessViews(_iRegisterNum, 1, &pUAV, &i);
}

void CTexture::GetPixelVector(vector<vector<tBGRA>>& _inVec)
{
    CaptureTexture(DEVICE, CONTEXT, m_Tex2D.Get(), m_Image);

    uint8_t* pixelMem = m_Image.GetPixels();
    const Image* image = m_Image.GetImages();
    if (pixelMem != 0)
    {
        for (int width = 0; width < image->width; ++width)
        {
            static vector<tBGRA> vecTmp;
            static tBGRA tmp = {};
            for (int height = 0; height < image->height; ++height)
            {
                tmp = {};

                tmp.b = (pixelMem[0 + (height * image->width + width) * 4]);
                tmp.g = (pixelMem[1 + (height * image->width + width) * 4]);
                tmp.r = (pixelMem[2 + (height * image->width + width) * 4]);
                tmp.a = (pixelMem[3 + (height * image->width + width) * 4]);
                tmp.check = false;

                vecTmp.push_back(tmp);
            }
            _inVec.push_back(vecTmp);
            vecTmp.clear();
        }
    }
    
}

void CTexture::GetPixelVector_float(vector<vector<float>>& _inVec)
{
    CaptureTexture(DEVICE, CONTEXT, m_Tex2D.Get(), m_Image);

    uint8_t* pixelMem = m_Image.GetPixels();
    const Image* image = m_Image.GetImages();
    float* floatPixelMem = reinterpret_cast<float*>(pixelMem);
    if (pixelMem != 0)
    {
        for (int width = 0; width < image->width; ++width)
        {
            static vector<float> vecTmp;
            for (int height = 0; height < image->height; ++height)
            {
                
                //uint8_t pixelValue = pixelMem[(height * 2048 + width)];
                float floatPixelValue = floatPixelMem[(height * 1024 + width)];
                vecTmp.push_back(floatPixelValue);
            }
            _inVec.push_back(vecTmp);
            vecTmp.clear();
        }
    }

}

float* CTexture::GetPixelfloat()
{
    CaptureTexture(DEVICE, CONTEXT, m_Tex2D.Get(), m_Image);

    uint8_t* pixelMem = m_Image.GetPixels();
    const Image* image = m_Image.GetImages();
    float* floatPixelMem = reinterpret_cast<float*>(pixelMem);

    return floatPixelMem;
}

Vec4 CTexture::WIdthSearch(vector<vector<tBGRA>> _inVec, Vec2 _inPos)
{
    // 위치정보 저장
    static list<Vec2> queue;
    queue.clear();
    // 사방 탐색 변수
    int dx[2] = {1,-1};
    // Vec4 반환 변수
    UINT OutLeft = _inPos.x;
    UINT OutRight = _inPos.x;
    UINT OutUp = _inPos.y;
    UINT OutDown = _inPos.y;

    // -로 가는 거 방지
    if (_inPos.x < 0)
        _inPos.x = 0;
    if (_inPos.y < 0)
        _inPos.y = 0;
    
    if (_inPos.x >= GetWidth())
        _inPos.x = GetWidth();
    if (_inPos.y >= GetHeight())
        _inPos.y = GetHeight();

    // 처음 클릭한곳은 이미 탐지됐다고 표시
    _inVec[_inPos.x][_inPos.y].check = true;
    queue.push_back(_inPos);
    
    // 탐색 시작
    while (!queue.empty())
    {
        // 제일 앞의 큐 꺼내오기
        Vec2 Pos = queue.front();
       
        // 사방 탐색
        for (int i = 0; i < 2; ++i)
        {
            if (Pos.x + dx[i] >= GetWidth() || Pos.y + dx[i] >= GetHeight() )
                continue;
            // X의 +1 과 -1이 알파가 0이 아닐 경우
            if (Pos.x != 0)
            {
                if (_inVec[Pos.x + dx[i]][Pos.y].a != 0)
                {
                    // 탐색할 위치가 미탐색 지역일 경우
                    if (!_inVec[Pos.x + dx[i]][Pos.y].check)
                    {
                        _inVec[Pos.x + dx[i]][Pos.y].check = true;
                        queue.push_back(Vec2(Pos.x + dx[i], Pos.y));
                    }
                }
            }
            if (Pos.y != 0)
            {
                // Y의 +1 과 -1이 알파가 0이 아닐 경우
                if (_inVec[Pos.x][Pos.y + dx[i]].a != 0)
                {
                    // 탐색할 위치가 미탐색 지역일 경우
                    if (!_inVec[Pos.x][Pos.y + dx[i]].check)
                    {
                        _inVec[Pos.x][Pos.y + dx[i]].check = true;
                        queue.push_back(Vec2(Pos.x, Pos.y + dx[i]));
                    }
                }
            }
        }
        // 제일 앞의 큐가 탐색지역일 경우 없애기
        if (_inVec[Pos.x][Pos.y].check)
        {
            queue.pop_front();
        }
        // 왼쪽위 오른쪽아래 구하기
        if (OutLeft > (Pos.x + dx[1]))
            OutLeft = Pos.x + dx[1];
        if (OutUp > (Pos.y + dx[1]))
            OutUp = Pos.y + dx[1];
        if (OutRight < (Pos.x + dx[0]))
            OutRight = Pos.x + dx[0];
        if (OutDown < (Pos.y + dx[0]))
            OutDown = Pos.y + dx[0];

    }

    return Vec4(OutLeft, OutUp, OutRight, OutDown);
    
}


