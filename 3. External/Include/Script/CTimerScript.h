#pragma once
#include <Engine/CScript.h>
class CTimerScript :
    public CScript
{
private:
    CGameObject*    m_pTimer;
    CGameObject*    m_pTReduce;

    float           m_fAcctime;
    float           m_fAcctime_Reduce;
public:
    virtual void begin() override;
    virtual void tick() override;

public:
    CLONE(CTimerScript);

public:
    CTimerScript();
    ~CTimerScript();
};

