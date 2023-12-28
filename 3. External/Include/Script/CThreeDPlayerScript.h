#pragma once
#include <Engine/CScript.h>
class CThreeDPlayerScript :
    public CScript
{
private:
    CGameObject* m_pAnimationObj;

    bool    m_isJump;

    float   m_fspeed;
    Vec3    m_vAxis;
    Vec3    m_vStart;
    Vec3    m_vEnd;

public:
    virtual void begin() override;
    virtual void tick() override;

    void SetSpeed(float _speed) { m_fspeed = _speed; }

    float GetSpeed() { return m_fspeed; }
    float Lerp(float A, float B, float Alpha)
    {
        return A * (1 - Alpha) + B * Alpha;
    }
// ��ó: https://eastroot1590.tistory.com/entry/Lerp [�۱׸� ��α�:Ƽ���丮]

private:
    void Move();
    void Jump();

    CLONE(CThreeDPlayerScript)
public:
    CThreeDPlayerScript();
    ~CThreeDPlayerScript();
};

