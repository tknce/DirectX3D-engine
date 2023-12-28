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
    ScratchImage                        m_Image;    // System Mem �� �ε��� �̹��� �ȼ������� ����
    ComPtr<ID3D11Texture2D>             m_Tex2D;    // GPU Mem �� �ű� ������ �����ϴ� ��ü


    ComPtr<ID3D11RenderTargetView>      m_RTV;  // Tex2D �� OM(OutputMerge) RenderTarget ���� ������ �� ���
    ComPtr<ID3D11DepthStencilView>      m_DSV;  // Tex2D �� OM(OutputMerge) DepthStencil �� ������ �� ���
    ComPtr<ID3D11ShaderResourceView>    m_SRV;  // Tex2D �� t �������Ϳ� ���ε��� �� ���
    ComPtr<ID3D11UnorderedAccessView>   m_UAV;  // Tex2D �� ��ǻƮ ���̴� u �������Ϳ� ������ �� ���
    
    vector< ComPtr<ID3D11ShaderResourceView>> m_vSRV; // �迭�ؽ��Ļ����� �ִ� ������� �� ���� imgui���� ���� ����


    D3D11_TEXTURE2D_DESC                m_Desc;     // Tex2D Description ����ü


public:
    void GetPixelVector(vector<vector<tBGRA>>& _inVec);
    void GetPixelVector_float(vector<vector<float>>& _inVec);
    float* GetPixelfloat();
    Vec4 WIdthSearch(vector<vector<tBGRA>> _inVec, Vec2 _inPos);
    
    ComPtr<ID3D11ShaderResourceView> CreateArrTexShaderResourceView(Ptr<CTexture> _iTex, int _iLevel); // �迭�ؽ��� ã���ֱ�

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


    // ����
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

