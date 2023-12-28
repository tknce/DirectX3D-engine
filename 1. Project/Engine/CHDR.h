#pragma once
#include "CComputeShader.h"
#include "CStructuredBuffer.h"

class CHDR :
    public CComputeShader
{
private:
    Ptr<CTexture>   m_pTargetTex;

    CStructuredBuffer* m_HDR;
    CStructuredBuffer* m_firstHDR;

public:
    void SetTexture(Ptr<CTexture> _Tex) { m_pTargetTex = m_pTargetTex; }
    Ptr<CTexture> GetTexture() { return m_pTargetTex; }

    CStructuredBuffer* GetStructuredBuffer() { return m_HDR; }
    CStructuredBuffer* GetStructuredFirstBuffer() { return m_firstHDR; }

public:
    virtual void UpdateData() override;
    virtual void Clear() override;


    
public:
    CHDR();
    ~CHDR();
};


