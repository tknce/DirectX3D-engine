#pragma once
#include <Engine/CScript.h>
class CDeadScript :
    public CScript
{
private:
    float m_fAcctime;
public:
    virtual void begin() override;
    virtual void tick() override;

    CLONE(CDeadScript);

    CDeadScript();
    ~CDeadScript();
};

