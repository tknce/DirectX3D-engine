#pragma once
#include "CRenderComponent.h"

enum class SKYBOX_TYPE
{
    SPHERE,
    CUBE,
};


class CSkyBox :
    public CRenderComponent
{
private:
    SKYBOX_TYPE         m_Type;
    Ptr<CTexture>       m_SkyBoxTex;

public:
    void SetType(SKYBOX_TYPE _type);
    void SetType(UINT _type);
    void SetSkyBoxTex(Ptr<CTexture> _tex);

public:
    // 0 SPHERE, 1 CUBE
    int GetType() { return (UINT)m_Type; } 
    Ptr<CTexture> GetTexture() { return m_SkyBoxTex; }
    ComPtr<ID3D11ShaderResourceView> GetTexSRV() { m_SkyBoxTex.Get()->GetSRV(); } // imgui에서 쓸 때 뷰만 쉽게 얻어올 수 있게 지원


public:
    virtual void finaltick() override;
    virtual void render() override;
    virtual void render(UINT _iSubset) override;

public:
    virtual void SaveToFile(FILE* _File) override;
    virtual void LoadFromFile(FILE* _File) override;

    CLONE(CSkyBox);
public:
    CSkyBox();
    ~CSkyBox();
};

