#pragma once
#include <Engine/CScript.h>
enum class POMP_ANIMATOR
{
    ATTACK,
    HURT_FLY,
    HURT_GROUNT,
    IDLE,
    KNOCKDOWN,
    RUN,
    TURN,
    WALK,
};

enum class POMP_IDLE
{
    LEFT_WALK,
    RIGHT_WALK,
};

enum class POMP_TRACE
{
    LEFT_WALK,
    RIGHT_WALK,
    ATTACK,
};

class CPompScript :
    public CScript
{
private:
    CGameObject*        m_pTarget;
    CGameObject* m_pParticle;

    State               m_State;

    Statevalue          m_StateValue;

    POMP_IDLE           m_eIdleState;
    POMP_TRACE          m_eTraceState;

    STATE_VAL_IDLE      m_State_val_idle;
    STATE_VLA_TRACE     m_State_val_Trace;

    POMP_ANIMATOR       m_eGruntAniState;
    POMP_ANIMATOR       m_eCurGruntAniState;


    CAnimation2D*       m_pCurMation;

    float               m_fAcctime;
    float               m_fDefectRange;
    float               m_fAccParticle;
    int                 m_iTurnJudgment;

    int                 m_iSkill;

    bool                m_bTurn;
    bool                m_bIschange;
public:
    void AddState();
    State FindState() { return m_State; }
    void ChangeState(State _State);

    void AniState();

    void IsaniChange(POMP_ANIMATOR _Type) { m_eCurGruntAniState = _Type; m_bIschange = true; }
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
    CLONE(CPompScript);

    CPompScript();
    ~CPompScript();
};

