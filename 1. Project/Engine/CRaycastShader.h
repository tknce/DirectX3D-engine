#pragma once
#include "CComputeShader.h"

class CStructuredBuffer;

class CRaycastShader :
    public CComputeShader
{
private:
    // ������ ���� ����
    Ptr<CTexture>       m_pHeightMap;

    // ���� �� ����
    UINT                m_iXFace;
    UINT                m_iZFace;

    // ī�޶� Ray ����
    tRay                m_ray;

    // ������ġ ��� ����
    CStructuredBuffer* m_pOutput;

public:
    void SetFaceCount(UINT _x, UINT _z) { m_iXFace = _x; m_iZFace = _z; }
    void SetCameraRay(const tRay& _ray) { m_ray = _ray; }
    void SetOuputBuffer(CStructuredBuffer* _pOutputBuffer) { m_pOutput = _pOutputBuffer; }
    void SetHeightMap(Ptr<CTexture> _ptex) { m_pHeightMap = _ptex; }

protected:
    virtual void UpdateData();
    virtual void Clear();

public:
    CRaycastShader();
    ~CRaycastShader();
};


