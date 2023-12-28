#pragma once

#include "CComputeShader.h"

class CBloom :
    public CComputeShader
{
    Ptr<CTexture>       m_HDRTex;
    Ptr<CTexture>       m_HDRBloomTex;

public:
    virtual void UpdateData() override;
    virtual void Clear() override;

    void SetRenderTex(Ptr<CTexture> _Tex) { m_HDRBloomTex = _Tex; }
    Ptr<CTexture> GetTex() { return m_HDRBloomTex; }

public:

    CBloom();
    ~CBloom();

};

