#pragma once
#include "ComponentUI.h"

class CGameObject;

class TransformUI :
    public ComponentUI
{
private: 
    Vec3            m_vPos;
    Vec3            m_vScale;
    Vec3            m_vRot;

    bool            m_bIgnorScale;
    bool            m_socket;

    int             m_iLayer;
private:
    virtual void update() override;
    virtual void render_update() override;

    void SetSockey(DWORD_PTR _strobjectname);
    
public:
    TransformUI();
    ~TransformUI();
};

