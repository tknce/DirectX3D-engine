#pragma once
#include <Engine/CScript.h>
class CMainScript :
    public CScript
{
private:
    vector<CGameObject*>    m_pMenu;

    float                   m_fAcctime;

    bool                    m_bNextStage;
    
public:
    virtual void begin() override;
    virtual void tick() override;

    CLONE(CMainScript)

public:
    CMainScript();
    ~CMainScript();
};

