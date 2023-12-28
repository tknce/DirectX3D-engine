#pragma once
#include <Engine/CScript.h>

enum class Gruntanimator
{
    ATTACK,
    HURT_FLY,
    HURT_GROUNT,
    IDLE,
    LASER_FLY,
    RUN,
    TURN,
    WALK,
};

enum class GRUNTSTATE_IDLE
{
    LEFT_WALK,
    RIGHT_WALK,
};

enum class GRUNTSTATE_TRACE
{
    LEFT_WALK,
    RIGHT_WALK,
    ATTACK,
};

class CGrountScript :
    public CScript
{
private:
    CGameObject*        m_pTarget;
    CGameObject* m_pParticle;

    State               m_State;
    GRUNTSTATE_IDLE     m_eIdleState;
    GRUNTSTATE_TRACE    m_eTraceState;

    Statevalue          m_StateValue;
    STATE_VAL_IDLE      m_State_val_idle;
    STATE_VLA_TRACE     m_State_val_Trace;

    Gruntanimator       m_eGruntAniState;
    Gruntanimator       m_eCurGruntAniState;

    CAnimation2D*       m_pCurMation;

    float               m_fAcctime;
    float               m_fDefectRange;
    float               m_fAccParticle;
    int                 m_iTurnJudgment;
    int                 m_iSkill;
    bool                m_bTurn;
    bool                m_bIschange;
   
public:
    State FindState() { return m_State; }
    void ChangeState(State _State);
    void AniState();

    void IsaniChange(Gruntanimator _Type) { m_eGruntAniState = _Type; m_bIschange = true; }
    void JudgTurn();
    void DefectPlayer();

public:
    virtual void begin() override;
    virtual void tick() override;
    void exit();


public:
    void IDLE();
    void IDLE_LEFT();
    void IDLE_RIGHT();

    void TRACE();
    void TRACE_LEFT();
    void TRACE_RIGHT();
    void TRACE_ATTACK();

    void DEAD();

    virtual void BeginOverlap(CCollider2D* _pOther) override;
    virtual void Overlap(CCollider2D* _pOther) override;
    virtual void EndOverlap(CCollider2D* _pOther) override;
public:
    CLONE(CGrountScript);

    CGrountScript();
    ~CGrountScript();
};

