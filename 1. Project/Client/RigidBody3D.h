#pragma once
#include "ComponentUI.h"
class RigidBody3D :
    public ComponentUI
{
private:
    bool        m_bIsGravity;
    bool        m_bIsDynamic;

    Vec3        m_vRockPos = {0,0,0};
    Vec3        m_vRockRot = {0,0,0};

    Vec3        m_vVelocity;
    Vec3        m_vAngularVelocity;
    Vec3        m_vPosition;
    Vec3        m_vRot;
public:
    virtual void update() override;
    virtual void render_update() override;

    RigidBody3D();
    ~RigidBody3D();
};

