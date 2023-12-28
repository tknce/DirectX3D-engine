#pragma once
#include <Engine/CScript.h>
class CUIskillScript :
    public CScript
{
private:
    vector<CGameObject*>        m_pSkilltime;
    CGameObject*                m_pShift;

    int                         m_iSpark;
    int                         m_iIdx;

    float                       m_fAcctime;
    float                       m_fAccSkill;
    float                       m_fAccrecover;

    bool                        m_fskillreturn;
public:
    virtual void begin() override;
    virtual void tick() override;

public:
    CLONE(CUIskillScript);

public:
    CUIskillScript();
    ~CUIskillScript();
};


