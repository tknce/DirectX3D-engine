#pragma once

#include <Engine\CScript.h>
enum class PLAYER_STATE_FIGHT
{
    IDLE,
    RUN,
    IDLE_TO_RUN,
    RUN_TO_IDLE,
    WALK,
    IDLE_TO_WALK,
    WALK_TO_IDLE,
    WALL_SLIDE,
    WALL_GRAB,
    PRECROUCH,
    ROLL,
    ATTACK,
    JUMP,
    FALL,
    HURTGROUND,
    FLIP,
    END,
};

class CPlayerScript :
    public CScript
{
private:
    float               m_fAccTime;
    float               m_fAcctime_jump;
    float               m_fPoweroverwhming;
    float               m_fSpeed;


    PLAYER_STATE_FIGHT  m_Fight;
    PLAYER_STATE_FIGHT  m_PreState;

    CAnimation2D*       m_CurAni;

    int                 m_iCount;
    int                 m_iJumpCount;
    int                 m_iRazerDead;
    int                 m_irewind;

    CGameObject*        m_pSword;

    Vec2                m_vPreVel;
    Vec3                m_vPos;

    bool                m_bTurn;
    bool                m_bAniState;
    bool                m_bStair;
    bool                m_bRazerDead;

    //renderupdate
    int                 m_ibskill;
    Vec4                m_vColor;

public:
    virtual void begin() override;
    virtual void tick() override;


    virtual void BeginOverlap(CCollider2D* _pOther) override;
    virtual void Overlap(CCollider2D* _pOther) override;
    virtual void EndOverlap(CCollider2D* _pOther) override;

    PLAYER_STATE_FIGHT GetState() { return m_Fight; }

    void Move();
    void KeyState();
    void AniState();
    void Skill();
    void IsDead(Vec3 _pos);
    void SetRazerDead(bool _dead) { m_bRazerDead = _dead; }
    void RazerDead();
    void IsWall();
    void IsAttack();
    void SetStair(bool _stair) { m_bStair = _stair; }
    void SetRewind();
    
    float GetPower() { return m_fPoweroverwhming; }

    CLONE(CPlayerScript);
public:
    CPlayerScript();
    ~CPlayerScript();
};

