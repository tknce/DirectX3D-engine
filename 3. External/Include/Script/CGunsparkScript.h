#pragma once
#include <Engine/CScript.h>


class CGunsparkScript :
    public CScript
{
private:
    CAnimation2D* m_pAni;
public:
    virtual void begin() override;
    virtual void tick() override;

    CLONE(CGunsparkScript);

    CGunsparkScript();
    ~CGunsparkScript();
};

