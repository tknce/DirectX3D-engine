#pragma once
#include "ComponentUI.h"
class RigidBodyUI :
    public ComponentUI
{
private:
    Vec2    m_vForce;               // ��
    Vec2    m_vAccel;               // ������
    Vec2    m_vVelocity;            // �ӵ�

    float   m_fMass;                // ����
    float   m_fFriction;            // ����
    float   m_fMaxSpeed;            // �߷��� �ƴ� �ִ� �ӷ� ����
    float   m_fMaxGravitySpeed;     // �߷¿� ���� �߻��ϴ� �ִ� �ӷ� ����

    Vec2    m_vGravityAccel;        // �߷� ���ӵ�
    bool    m_bUseGravity;          // �߷� ����

public:
    virtual void update() override;
    virtual void render_update() override;

public:
    RigidBodyUI();
    ~RigidBodyUI();
};

