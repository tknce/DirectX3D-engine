#include "pch.h"
#include "CGrountScript.h"

#include <Engine/CAnimation2D.h>
#include <Engine/CSoundMgr.h>
#include <Engine/CLevel.h>
#include <Engine/CLevelMgr.h>
#include "CPlayerScript.h"

CGrountScript::CGrountScript()
    : CScript(GROUNTSCRIPT)
    , m_eGruntAniState(Gruntanimator::IDLE)
    , m_eIdleState(GRUNTSTATE_IDLE::RIGHT_WALK)
    , m_eTraceState(GRUNTSTATE_TRACE::ATTACK)
    , m_iTurnJudgment(1)
    , m_bTurn(false)   
    , m_fDefectRange(800)
{
}

CGrountScript::~CGrountScript()
{
}


void CGrountScript::begin()
{
    m_StateValue.enter = true;

    m_State_val_idle.enter = true;
    m_State_val_Trace.enter = true;
    m_pCurMation = Animator2D()->Play(L"TURN", false, m_bTurn);
    RigidBody2D()->SetGround(false);
    RigidBody2D()->SetFriction(10.f);
    CLevel* CurLevel = CLevelMgr::GetInst()->GetCurLevel();
    m_pTarget = CurLevel->FindObjectByName(L"Player");
    RigidBody2D()->SetGravityAccel(Vec2(0,800));
}

void CGrountScript::tick()
{
    m_fAccParticle += DT;
    if (m_fAccParticle > 2.f)
    {
        if (m_pParticle != nullptr)
            m_pParticle->Transform()->SetRelativePos(0.f, 9999.f, 0.f);
    }

    if (KEY_PRESSED(KEY::LSHIFT))
        m_iSkill = 1;
    else
        m_iSkill = 0;
    GetOwner()->MeshRender()->GetCurMaterial(0)->SetScalarParam(INT_0, &m_iSkill);
    if (m_State != State::DEAD)
    {
        if (m_pTarget != nullptr)
        {
            if (m_pTarget->Transform()->GetRelativePos().x > Transform()->GetRelativePos().x)
            {
                Transform()->SetRelativeRotation(0.f, 0.f, 0.f);
            }
            else
            {
                Transform()->SetRelativeRotation(0.f, XM_PI, 0.f);
            }
        }
        
    }
    

    if(m_State != State::DEAD)
    DefectPlayer();
    m_fAcctime += DT;
    switch (m_State)
    {
    case State::IDLE:
        IDLE();
        break;
    case State::TRACE:
        TRACE();
        break;
    case State::DEAD:
        DEAD();
        break;
    }
    if(m_bIschange)
    AniState();
}

void CGrountScript::IDLE()
{
    Vec3 Pos = Transform()->GetRelativePos();
    if (m_StateValue.enter == true)
    {
        IsaniChange(Gruntanimator::IDLE);
        m_StateValue.tick = true;
        m_StateValue.enter = false;
    }
    if (m_StateValue.tick == true)
    {
        switch (m_eIdleState)
        {
        case GRUNTSTATE_IDLE::LEFT_WALK:
            IDLE_LEFT();
            break;
        case GRUNTSTATE_IDLE::RIGHT_WALK:
            IDLE_RIGHT();
            break;
        default:
            break;
        }
    }

}

void CGrountScript::IDLE_LEFT()
{
    if (m_State_val_idle.enter == true)
    {
        m_bTurn = true;
        m_State_val_idle.tick = true;
        m_State_val_idle.enter = false;
    }
    if (m_State_val_idle.tick == true)
    {
        if(m_pCurMation->GetFinish())
        IsaniChange(Gruntanimator::WALK);
        RigidBody2D()->AddForce(Vec2(-30, 0));
        if (m_fAcctime > 3)
        {
            m_State_val_idle.tick = false;
            m_State_val_idle.exit = true;
            m_fAcctime = 0;
        }
    }
    if (m_State_val_idle.exit == true)
    {
        m_eIdleState = GRUNTSTATE_IDLE::RIGHT_WALK;
        m_bTurn = false;
        IsaniChange(Gruntanimator::TURN);
        RigidBody2D()->SetVelocity(Vec2(0, 0));
        m_State_val_idle.enter = true;
        m_State_val_idle.exit = false;
    }
}

void CGrountScript::IDLE_RIGHT()
{
    if (m_State_val_idle.enter == true)
    {
        m_State_val_idle.tick = true;
        m_State_val_idle.enter = false;
    }
    if (m_State_val_idle.tick == true)
    {
        if (m_pCurMation->GetFinish())
        IsaniChange(Gruntanimator::WALK);
        m_bTurn = false;
        RigidBody2D()->AddForce(Vec2(30, 0));
        if (m_fAcctime > 3)
        {
            m_State_val_idle.tick = false;
            m_State_val_idle.exit = true;
            m_fAcctime = 0;
        }
    }
    if (m_State_val_idle.exit == true)
    {
        m_eIdleState = GRUNTSTATE_IDLE::LEFT_WALK;
        m_bTurn = true;
        IsaniChange(Gruntanimator::TURN);
        RigidBody2D()->SetVelocity(Vec2(0, 0));
        m_State_val_idle.enter = true;
        m_State_val_idle.exit = false;
    }
}



void CGrountScript::TRACE()
{
    Vec3 Pos = Transform()->GetRelativePos();
    Vec3 TarPos = m_pTarget->Transform()->GetRelativePos();




    if (m_StateValue.enter == true)
    {
        m_StateValue.tick = true;
        m_StateValue.enter = false;
    }
    if (m_StateValue.tick == true)
    {
        switch (m_eTraceState)
        {
        case GRUNTSTATE_TRACE::LEFT_WALK:
            TRACE_LEFT();
            break;
        case GRUNTSTATE_TRACE::RIGHT_WALK:
            TRACE_RIGHT();
            break;
        case GRUNTSTATE_TRACE::ATTACK:
            TRACE_ATTACK();
            break;
        default:
            break;
        }
    }


}

void CGrountScript::TRACE_LEFT()
{
    Vec3 Pos = Transform()->GetRelativePos();
    Vec3 TarPos = m_pTarget->Transform()->GetRelativePos();
    float fDistance = (Transform()->GetRelativePos() - m_pTarget->Transform()->GetRelativePos()).Length();
    if (m_State_val_Trace.enter == true)
    {
        RigidBody2D()->SetFriction(10.f);
        m_bTurn = true;
        m_State_val_Trace.tick = true;
        m_State_val_Trace.enter = false;
    }
    if (m_State_val_Trace.tick == true)
    {

        if (TarPos.x > Pos.x)
        {
            m_State_val_Trace.tick = false;
            m_State_val_Trace.exit = true;
        }

        if (fDistance < 120)
        {
            m_State_val_Trace.tick = false;
            m_State_val_Trace.exit = true;
        }

        if (m_pCurMation->GetFinish())
        IsaniChange(Gruntanimator::RUN);
        RigidBody2D()->AddForce(Vec2(-60, 0));
    }
    if (m_State_val_Trace.exit == true)
    {
        if (TarPos.x > Pos.x)
            m_eTraceState = GRUNTSTATE_TRACE::RIGHT_WALK;

        if (fDistance < 90)
            m_eTraceState = GRUNTSTATE_TRACE::ATTACK;

        m_bTurn = false;
        IsaniChange(Gruntanimator::TURN);
        RigidBody2D()->SetVelocity(Vec2(0, 0));
        m_State_val_Trace.enter = true;
        m_State_val_Trace.exit = false;
    }
}

void CGrountScript::TRACE_RIGHT()
{
    Vec3 Pos = Transform()->GetRelativePos();
    Vec3 TarPos = m_pTarget->Transform()->GetRelativePos();
    float fDistance = (Transform()->GetRelativePos() - m_pTarget->Transform()->GetRelativePos()).Length();
    if (m_State_val_Trace.enter == true)
    {
        RigidBody2D()->SetFriction(10.f);
        m_bTurn = false;
        m_State_val_Trace.tick = true;
        m_State_val_Trace.enter = false;
    }
    if (m_State_val_Trace.tick == true)
    {
        
        if (TarPos.x < Pos.x)
        {
            m_State_val_Trace.tick = false;
            m_State_val_Trace.exit = true;
        }
        
        if (fDistance < 90)
        {
            m_State_val_Trace.tick = false;
            m_State_val_Trace.exit = true;
        }
           
        if (m_pCurMation->GetFinish())
        IsaniChange(Gruntanimator::RUN);
        RigidBody2D()->AddForce(Vec2(60, 0));
    }
    if (m_State_val_Trace.exit == true)
    {
        if (TarPos.x < Pos.x)
            m_eTraceState = GRUNTSTATE_TRACE::LEFT_WALK;

        if (fDistance < 120)
            m_eTraceState = GRUNTSTATE_TRACE::ATTACK;

        m_bTurn = true;
        IsaniChange(Gruntanimator::TURN);
        RigidBody2D()->SetVelocity(Vec2(0, 0));
        m_State_val_Trace.enter = true;
        m_State_val_Trace.exit = false;
    }
}

void CGrountScript::TRACE_ATTACK()
{
    Vec3 Pos = Transform()->GetRelativePos();
    Vec3 TarPos = m_pTarget->Transform()->GetRelativePos();
    float fDistance = (Transform()->GetRelativePos() - m_pTarget->Transform()->GetRelativePos()).Length();
    if (m_State_val_Trace.enter == true)
    {
        if (TarPos.x < Pos.x)
            m_bTurn = true;
        else
        {
            m_bTurn = false;
        }

        RigidBody2D()->SetFriction(100.f);
        RigidBody2D()->SetVelocity(Vec2(0, 0));
        m_State_val_Trace.tick = true;
        m_State_val_Trace.enter = false;
    }
    if (m_State_val_Trace.tick == true)
    {
        if (fDistance > 120)
        {
            m_State_val_Trace.tick = false;
            m_State_val_Trace.exit = true;
        }
        else
        {
            IsaniChange(Gruntanimator::ATTACK);
        }
        
    }
    if (m_State_val_Trace.exit == true)
    {
        float fDistance = (Transform()->GetRelativePos() - m_pTarget->Transform()->GetRelativePos()).Length();
        if (fDistance > 120)
        {
            if (TarPos.x < Pos.x)
                m_eTraceState = GRUNTSTATE_TRACE::LEFT_WALK;
            else
                m_eTraceState = GRUNTSTATE_TRACE::RIGHT_WALK;
        }
        m_State_val_Trace.enter = true;
        m_State_val_Trace.exit = false;
    }
}

void CGrountScript::DEAD()
{
    if (m_StateValue.enter == true)
    {
        IsaniChange(Gruntanimator::HURT_FLY);
        if (m_pParticle == nullptr)
        {
            m_fAccParticle = 0;
            m_pParticle = CResMgr::GetInst()->FindRes<CPrefab>(L"prefab\\blood.pref").Get()->Instantiate();
            m_pParticle->ParticleSystem()->GetCurMaterial(0)->SetTexParam(TEX_0, CResMgr::GetInst()->FindRes<CTexture>(L"texture\\particle\\Blood.png"));
            Vec3 Pos = Transform()->GetRelativePos();
            //Pos.z = 0;
            Pos.y -= 50;
            Instantiate(m_pParticle, Pos);
        }
        RigidBody2D()->SetVelocity(Vec2(0.f, 0.f));
        m_StateValue.tick = true;
        m_StateValue.enter = false;
    }
    if (m_StateValue.tick == true)
    {
        if (m_pCurMation->GetFinish())
        {
            IsaniChange(Gruntanimator::HURT_GROUNT);
            m_StateValue.tick = false;
        }
        
    }

}

void CGrountScript::exit()
{
    switch (m_State)
    {
    case State::IDLE:
        break;
    case State::TRACE:
        break;
    case State::DEAD:
        break;
    default:
        break;
    }
}

void CGrountScript::ChangeState(State _State)
{
    exit();
    m_State = _State;
    m_StateValue.enter = true;
}

void CGrountScript::AniState()
{
    m_bIschange = false;
    if (m_eGruntAniState != m_eCurGruntAniState )
    {
        switch (m_eGruntAniState)
        {
        case Gruntanimator::ATTACK:
            Animator2D()->Play(L"ATTACK", true, m_bTurn);
            break;
        case Gruntanimator::HURT_FLY:
            m_pCurMation = Animator2D()->Play(L"HURT_FLY", false, m_bTurn);
            break;
        case Gruntanimator::HURT_GROUNT:
            Animator2D()->Play(L"HURT_GROUND", false, m_bTurn);
            break;
        case Gruntanimator::IDLE:
            Animator2D()->Play(L"IDLE", true, m_bTurn);
            break;
        case Gruntanimator::LASER_FLY:
            Animator2D()->Play(L"LASER_FLY", true, m_bTurn);
            break;
        case Gruntanimator::RUN:
            Animator2D()->Play(L"RUN", true, m_bTurn);
            break;
        case Gruntanimator::TURN:
            m_pCurMation = Animator2D()->Play(L"TURN", false, m_bTurn);
            break;
        case Gruntanimator::WALK:
            Animator2D()->Play(L"WALK", true, m_bTurn);
            break;
        default:
            break;
        }
        m_eCurGruntAniState = m_eGruntAniState;
    }

}

void CGrountScript::JudgTurn()
{
    if (m_iTurnJudgment == 0)
    {
        IsaniChange(Gruntanimator::TURN);
    }
}

void CGrountScript::DefectPlayer()
{
    if (m_pTarget != nullptr)
    {
        float fDistance = (Transform()->GetRelativePos() - m_pTarget->Transform()->GetRelativePos()).Length();
        if (fDistance < m_fDefectRange)
        {
            Transform()->SetRelativePos(Transform()->GetRelativePos());
            m_State = State::TRACE;
        }
    }
   
}

void CGrountScript::BeginOverlap(CCollider2D* _pOther)
{
    RigidBody2D()->SetGround(true);
    if (_pOther->GetOwner()->GetName() == L"SWORD")
    {
        CSoundMgr::GetInst()->FindSound(L"sound\\enemy_drath_sword.wav", 1, 0.6f, true);
        m_State = State::DEAD;
        m_StateValue.enter = true;
    }
    if (_pOther->GetOwner()->GetName() == L"Player")
    {
        CSoundMgr::GetInst()->FindSound(L"sound\\enemy_punch_hit.wav", 1, 0.6f, true);
        if(m_eTraceState == GRUNTSTATE_TRACE::ATTACK)
            _pOther->GetOwner()->GetScript<CPlayerScript>()->IsDead(GetOwner()->Transform()->GetRelativePos());
    }
}

void CGrountScript::Overlap(CCollider2D* _pOther)
{
    RigidBody2D()->SetGround(true);
}

void CGrountScript::EndOverlap(CCollider2D* _pOther)
{
    //RigidBody2D()->SetGround(false);
}
