#pragma once
#include "CEntity.h"

#include "Ptr.h"
#include "CTexture.h"

class CMRT :
    public CEntity
{
private:
    Ptr<CTexture>   m_arrTarget[8];
    Vec4            m_arrClearColor[8];

    Ptr<CTexture>   m_DSTex;
    int             m_RTCount;

    D3D11_VIEWPORT  m_tViewPort;

public:
    void Create(Ptr<CTexture>(&_arrTex)[8], Vec4(&arrClear)[8], Ptr<CTexture> _pDesTex);
    const D3D11_VIEWPORT& GetViewPort() { return m_tViewPort; }

    void Clear();
    void OMSet();

    CLONE_DUMMY(CMRT);
public:
    CMRT();
    ~CMRT();
};

