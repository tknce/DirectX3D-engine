#pragma once
#include <Engine/CScript.h>
class CUIIconScript :
    public CScript
{
private:

    CGameObject* m_pkatana;
public:
    virtual void begin() override;
    virtual void tick() override;

public:
    CLONE(CUIIconScript);

public:
    CUIIconScript();
    ~CUIIconScript();
};

