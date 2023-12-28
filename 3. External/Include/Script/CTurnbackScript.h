#pragma once
#include <Engine/CScript.h>
class CTurnbackScript :
    public CScript
{
private:
    float m_fpos;
    float m_fUV;

    float m_fAccTime;
    float m_fTime;

    bool    m_bDead;
public:
    virtual void begin() override;
    virtual void tick() override;

    CLONE(CTurnbackScript);

    CTurnbackScript();
    ~CTurnbackScript();
};

