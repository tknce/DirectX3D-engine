#pragma once
#include "ComponentUI.h"

class CGameObject;

class Collider2DUI :
    public ComponentUI
{
private:
    Vec3            m_vOffset;
    Vec3            m_vScale;
    Vec3            m_vRot;
    Vec3            m_vPreTransformPos;

    COLLIDER2D_TYPE m_Type;

    bool            m_bIgnorObjectScale;
    bool            m_bEditOffset;

private:
    virtual void update() override;
    virtual void render_update() override;

    void MouseDragMode_Render();
    void MouseDragMode_TileMap();

public:
    Collider2DUI();
    ~Collider2DUI();

};

