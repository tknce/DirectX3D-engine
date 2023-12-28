#pragma once
#include <Engine\CScript.h>



class CGameObject;

class CMonsterScript :
    public CScript
{
private:
    CGameObject*            m_pTargetObject;

    MONSTER_STATE_FIGHT      m_Fight;
    vector<wstring>         m_AniPlay;

    Vec3                    m_vDir;

    float                   m_fDistance;

    bool                    m_bAni;

public:
    virtual void begin() override;
    virtual void tick() override;

public:
    void DetectRange();

public:
    virtual void BeginOverlap(CCollider2D* _pOther) override;
    virtual void Overlap(CCollider2D* _pOther) override;
    virtual void EndOverlap(CCollider2D* _pOther) override;

    CLONE(CMonsterScript);
public:
    CMonsterScript();
    ~CMonsterScript();
};

