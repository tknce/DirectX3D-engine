#pragma once
#include <Engine/CScript.h>
enum class GANGSTER_ANIMATOR
{
    AIM,
    ATTACK_ALMOST,
    HURT_FLY,
    HURT_GROUND,
    IDLE,
    RUN,
    TURN,
    WALK,
};

enum class GANGSTER_IDLE
{
    LEFT_WALK,
    RIGHT_WALK,
};

enum class GANGSTER_TRACE
{
    LEFT_WALK,
    RIGHT_WALK,
    ATTACK,
};

class CGasterScript :
    public CScript
{
private:
private:
    CGameObject* m_pTarget;
    CGameObject* m_pParticle;

    State               m_State;

    Statevalue          m_StateValue;

    GANGSTER_IDLE           m_eIdleState;
    GANGSTER_TRACE          m_eTraceState;

    STATE_VAL_IDLE      m_State_val_idle;
    STATE_VLA_TRACE     m_State_val_Trace;

    GANGSTER_ANIMATOR       m_eGruntAniState;
    GANGSTER_ANIMATOR       m_eCurGruntAniState;


    CAnimation2D* m_pCurMation;

    float               m_fAcctime;
    float               m_fDefectRange;
    float               m_fAccParticle;

    int                 m_iTurnJudgment;
    int                 m_iSkill;
    float                 Test;


    bool                m_bTurn;
    bool                m_bIschange;
public:
    void AddState();
    State FindState() { return m_State; }
    void ChangeState(State _State);

    void AniState();

    void IsaniChange(GANGSTER_ANIMATOR _Type) { m_eCurGruntAniState = _Type; m_bIschange = true; }
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
    CLONE(CGasterScript);

    CGasterScript();
    ~CGasterScript();
};

