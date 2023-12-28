#pragma once
#include <Engine/CScript.h>
class CTitleScript :
    public CScript
{
private:
    float m_fAcctime_Zer;
    float m_fAcctime_O;

    CGameObject* m_pZer;
    CGameObject* m_pO;
    CGameObject* m_pMenu;

    float   m_fAcctime;
    bool    m_bNexttime;

public:
    virtual void begin() override;
    virtual void tick() override;

    CLONE(CTitleScript)

public:
    CTitleScript();
    ~CTitleScript();
};

