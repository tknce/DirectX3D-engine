#pragma once
#include "CRenderComponent.h"
class CDecal :
    public CRenderComponent
{
private:
    Ptr<CTexture>   m_DecalTex;
    bool            m_bLighting;


public:
    void SetDefaultLit(bool _bSet);
    void SetDecalTexture(Ptr<CTexture> _tex) { m_DecalTex = _tex; }

public:
    bool GetDefaultLit() { return m_bLighting; }
    Ptr<CTexture> GetTexture() { return m_DecalTex; }
    ComPtr<ID3D11ShaderResourceView> GetTexSRV() { m_DecalTex.Get()->GetSRV(); } // imgui에서 쓸 때 뷰만 쉽게 얻어올 수 있게 지원

public:
    virtual void finaltick() override;
    virtual void render() override;
    virtual void render(UINT _iSubset) override;

public:
    virtual void SaveToFile(FILE* _File) override;
    virtual void LoadFromFile(FILE* _File) override;

    CLONE(CDecal);
public:
    CDecal();
    ~CDecal();
};

