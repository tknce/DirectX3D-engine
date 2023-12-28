#pragma once
#include <Engine\CScript.h>

enum class CAMERAEFFECT
{
    SHAKE,
    END
};

class CCameraScript :
    public CScript
{
private:
    float       m_fSpeed;


public:
    virtual void begin() override;
    virtual void tick() override;

private:
    void Move();
    void CameraEffect();

    CLONE(CCameraScript);
public:
    CCameraScript();
    ~CCameraScript();
};


