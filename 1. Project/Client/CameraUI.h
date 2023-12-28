#pragma once
#include "ComponentUI.h"

class CGameObject;

class CameraUI :
    public ComponentUI
{
private:
    bool            m_bProjtype;

    float           m_fFar;
    float           m_fScale;

    vector<string>  m_vLayer;
    UINT            m_iLayermask;

private:

    virtual void update() override;
    virtual void render_update() override;

    virtual void Setmask(DWORD_PTR _iMask);
public:
    CameraUI();
    ~CameraUI();
};

