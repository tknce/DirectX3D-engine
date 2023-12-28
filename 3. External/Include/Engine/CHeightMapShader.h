#pragma once
#include "CComputeShader.h"
#include "CStructuredBuffer.h"

class CHeightMapShader :
    public CComputeShader
{
private:
    Ptr<CTexture>       m_pHeightMap;
    Ptr<CTexture>       m_pBrushTex;
    Vec2                m_vScale; // ���� Brush ũ��
    int                 m_iBrushIdx;
    CStructuredBuffer* m_pInput; // Ray �浹 ��ġ    


public:
    void SetHeightMap(Ptr<CTexture> _pTex) { m_pHeightMap = _pTex; }
    void SetBrushTex(Ptr<CTexture> _pTex) { m_pBrushTex = _pTex; }
    void SetBrushScale(Vec2 _vScale) { m_vScale = _vScale; }
    void SetBrushIndex(int _iIdx) { m_iBrushIdx = _iIdx; }
    void SetInputBuffer(CStructuredBuffer* _pInput) { m_pInput = _pInput; }


public:
    virtual void UpdateData();
    virtual void Clear();


public:
    CHeightMapShader();
    ~CHeightMapShader();
};

