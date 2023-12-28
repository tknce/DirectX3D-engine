#pragma once
#include "CRes.h"

#include <DirectXTex/DirectXTex.h>

#ifdef _DEBUG
#pragma comment(lib, "DirectXTex/DirectXTex_debug")
#else
#pragma comment(lib, "DirectXTex/DirectXTex")
#endif


class CTexture :
    public CRes
{
private:
    ScratchImage                        m_Image;    // System Mem 에 로딩된 이미지 픽셀데이터 관리
    ComPtr<ID3D11Texture2D>             m_Tex2D;    // GPU Mem 에 옮긴 데이터 관리하는 객체


    ComPtr<ID3D11RenderTargetView>      m_RTV;  // Tex2D 를 OM(OutputMerge) RenderTarget 으로 전달할 때 사용
    ComPtr<ID3D11DepthStencilView>      m_DSV;  // Tex2D 를 OM(OutputMerge) DepthStencil 로 전달할 때 사용
    ComPtr<ID3D11ShaderResourceView>    m_SRV;  // Tex2D 를 t 레지스터에 바인딩할 때 사용
    ComPtr<ID3D11UnorderedAccessView>   m_UAV;  // Tex2D 를 컴퓨트 쉐이더 u 레지스터에 전달할 때 사용
    
    vector< ComPtr<ID3D11ShaderResourceView>> m_vSRV; // 배열텍스쳐생성시 넣는 순서대로 뷰 생성 imgui에서 쓰기 위해


    D3D11_TEXTURE2D_DESC                m_Desc;     // Tex2D Description 구조체


public:
    void GetPixelVector(vector<vector<tBGRA>>& _inVec);
    void GetPixelVector_float(vector<vector<float>>& _inVec);
    float* GetPixelfloat();
    Vec4 WIdthSearch(vector<vector<tBGRA>> _inVec, Vec2 _inPos);
    
    ComPtr<ID3D11ShaderResourceView> CreateArrTexShaderResourceView(Ptr<CTexture> _iTex, int _iLevel); // 배열텍스쳐 찾아주기

public:
    UINT GetWidth() { return m_Desc.Width; }
    UINT GetHeight() { return m_Desc.Height; }
    Vec2 GetResolution() { return Vec2(m_Desc.Width, m_Desc.Height); }
    UINT GetArraySize() { return m_Desc.ArraySize; }

    ComPtr<ID3D11Texture2D>           GetTex2D() { return m_Tex2D; }
	ComPtr<ID3D11RenderTargetView>    GetRTV() { return  m_RTV; }
	ComPtr<ID3D11DepthStencilView>    GetDSV() { return  m_DSV; }
	ComPtr<ID3D11ShaderResourceView>  GetSRV() { return  m_SRV; }
	ComPtr<ID3D11UnorderedAccessView> GetUAV() { return  m_UAV; }
    const D3D11_TEXTURE2D_DESC& GetDesc() { return m_Desc; }
    void* GetSysMem() { return m_Image.GetPixels(); }
    UINT                              GetRowPitch() const { return (UINT)m_Image.GetImages()->rowPitch; }
    UINT                              GetSlicePitch()const { return (UINT)m_Image.GetImages()->slicePitch; }

    void GenerateMip(UINT _iMipLevel);

    // PIPELINE_STAGE
    void UpdateData(UINT _iRegisterNum, UINT _iPipelineStage);
    void UpdateData_CS(UINT _iRegisterNum, bool _bShaderRes);

    static void Clear(UINT _iRegisterNum);
    static void Clear_CS(UINT _iRegisterNum);

private:
    virtual void Save(const wstring& _strRelativePath);
    virtual int Load(const wstring& _strFilePath);
    int Load_oldver(const wstring& _strFilePath);
    int Load(const wstring& _strFilePath, int _iMipLevel);


    // 생성
    void Create(UINT _iWidth, UINT _iHeight, DXGI_FORMAT _Format, UINT _iBindFlag, bool _Cpu = false);
    void Create(ComPtr<ID3D11Texture2D> _Tex2D);
    void CreateStaging(UINT _iWidth, UINT _iHeight, DXGI_FORMAT _Format);
    int CreateArrayTexture(const vector<Ptr<CTexture>>& _vecTex, int _iMapLevel);
    

    CLONE_ASSERT(CTexture);
public:
    CTexture(bool bEngineRes = false);
    ~CTexture();

    friend class CResMgr;
};

