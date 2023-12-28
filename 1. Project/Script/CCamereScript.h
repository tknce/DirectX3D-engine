#pragma once
#include <Engine/CScript.h>
class CCamereScript :
    public CScript
{
private:
    float m_fSpeed;


    void Move();
    
public:
    virtual void begin() override;
    virtual void tick() override;
    
    void RotateCamera(float dx, float dy);
    CLONE(CCamereScript)
public:
    CCamereScript();
    ~CCamereScript();
};

