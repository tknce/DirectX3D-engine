#pragma once
#include "ComponentUI.h"
class RigidBodyUI :
    public ComponentUI
{
private:
    Vec2    m_vForce;               // 힘
    Vec2    m_vAccel;               // 빠르기
    Vec2    m_vVelocity;            // 속도

    float   m_fMass;                // 질량
    float   m_fFriction;            // 마찰
    float   m_fMaxSpeed;            // 중력이 아닌 최대 속력 제한
    float   m_fMaxGravitySpeed;     // 중력에 의해 발생하는 최대 속력 제한

    Vec2    m_vGravityAccel;        // 중력 가속도
    bool    m_bUseGravity;          // 중력 적용

public:
    virtual void update() override;
    virtual void render_update() override;

public:
    RigidBodyUI();
    ~RigidBodyUI();
};

