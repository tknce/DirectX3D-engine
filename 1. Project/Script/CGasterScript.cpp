#include "pch.h"
#include "CGasterScript.h"

#include <Engine/CSoundMgr.h>
#include <Engine/CAnimation2D.h>
#include <Engine/CResMgr.h>
#include <Engine/CPrefab.h>
#include <Engine/CLevelMgr.h>
#include <Engine/CLevel.h>
#include <Engine/CPlayerMgr.h>

CGasterScript::CGasterScript()
    : CScript(GASTERSCRIPT)
    , m_eGruntAniState(GANGSTER_ANIMATOR::IDLE)
    , m_eIdleState(GANGSTER_IDLE::RIGHT_WALK)
    , m_eTraceState(GANGSTER_TRACE::ATTACK)
    , m_iTurnJudgment(1)
    , m_bTurn(false)
    , m_fDefectRange(800)
{
}

CGasterScript::~CGasterScript()
{
}

void CGasterScript::AddState()
{
}


void CGasterScript::DefectPlayer()
{
    if (m_pTarget)
    {
        float fDistance = (Transform()->GetRelativePos() - m_pTarget->Transform()->GetRelativePos()).Length();
        if (fDistance < 400)
        {
            Transform()->SetRelativePos(Transform()->GetRelativePos());
            m_State = State::TRACE;
        }
    }
}

void CGasterScript::begin()
{
    m_StateValue.enter = true;

    m_State_val_idle.enter = true;
    m_State_val_Trace.enter = true;
    m_pCurMation = Animator2D()->Play(L"TURN", false, m_bTurn);
    RigidBody2D()->SetGround(false);
    RigidBody2D()->SetFriction(10.f);
    CLevel* CurLevel = CLevelMgr::GetInst()->GetCurLevel();
    m_pTarget = CurLevel->FindObjectByName(L"Player");
    RigidBody2D()->SetGravityAccel(Vec2(0, 800));
    Test = 0;
}

void CGasterScript::tick()
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
        if (m_pTarget)
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

    if (m_State != State::DEAD)
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
    if (m_bIschange)
        AniState();
}

void CGasterScript::IDLE()
{
    Vec3 Pos = Transform()->GetRelativePos();
    if (m_StateValue.enter == true)
    {
        IsaniChange(GANGSTER_ANIMATOR::IDLE);
        m_StateValue.tick = true;
        m_StateValue.enter = false;
    }
    if (m_StateValue.tick == true)
    {
        switch (m_eIdleState)
        {
        case GANGSTER_IDLE::LEFT_WALK:
            IDLE_LEFT();
            break;
        case GANGSTER_IDLE::RIGHT_WALK:
            IDLE_RIGHT();
            break;
        default:
            break;
        }
    }
}

void CGasterScript::IDLE_LEFT()
{
    if (m_State_val_idle.enter == true)
    {
        m_bTurn = true;
        m_State_val_idle.tick = true;
        m_State_val_idle.enter = false;
    }
    if (m_State_val_idle.tick == true)
    {
        if (m_pCurMation->GetFinish())
            IsaniChange(GANGSTER_ANIMATOR::WALK);
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
        m_eIdleState = GANGSTER_IDLE::RIGHT_WALK;
        m_bTurn = false;
        IsaniChange(GANGSTER_ANIMATOR::TURN);
        RigidBody2D()->SetVelocity(Vec2(0, 0));
        m_State_val_idle.enter = true;
        m_State_val_idle.exit = false;
    }
}

void CGasterScript::IDLE_RIGHT()
{
    if (m_State_val_idle.enter == true)
    {
        m_State_val_idle.tick = true;
        m_State_val_idle.enter = false;
    }
    if (m_State_val_idle.tick == true)
    {
        if (m_pCurMation->GetFinish())
            IsaniChange(GANGSTER_ANIMATOR::WALK);
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
        m_eIdleState = GANGSTER_IDLE::LEFT_WALK;
        m_bTurn = true;
        IsaniChange(GANGSTER_ANIMATOR::TURN);
        RigidBody2D()->SetVelocity(Vec2(0, 0));
        m_State_val_idle.enter = true;
        m_State_val_idle.exit = false;
    }
}

void CGasterScript::TRACE()
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
        case GANGSTER_TRACE::LEFT_WALK:
            TRACE_LEFT();
            break;
        case GANGSTER_TRACE::RIGHT_WALK:
            TRACE_RIGHT();
            break;
        case GANGSTER_TRACE::ATTACK:
            TRACE_ATTACK();
            break;
        default:
            break;
        }
    }

}

void CGasterScript::TRACE_LEFT()
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
        IsaniChange(GANGSTER_ANIMATOR::RUN);
        RigidBody2D()->AddForce(Vec2(-60, 0));
    }
    if (m_State_val_Trace.exit == true)
    {
        if (TarPos.x < Pos.x)
            m_eTraceState = GANGSTER_TRACE::RIGHT_WALK;

        if (fDistance < 120)
            m_eTraceState = GANGSTER_TRACE::ATTACK;


        RigidBody2D()->SetVelocity(Vec2(0, 0));
        m_State_val_Trace.enter = true;
        m_State_val_Trace.exit = false;
    }
}

void CGasterScript::TRACE_RIGHT()
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

        if (fDistance < 800)
        {
            m_State_val_Trace.tick = false;
            m_State_val_Trace.exit = true;
        }


        IsaniChange(GANGSTER_ANIMATOR::RUN);
        RigidBody2D()->AddForce(Vec2(60, 0));
    }
    if (m_State_val_Trace.exit == true)
    {
        if (TarPos.x > Pos.x)
            m_eTraceState = GANGSTER_TRACE::LEFT_WALK;

        if (fDistance < 800)
            m_eTraceState = GANGSTER_TRACE::ATTACK;

        RigidBody2D()->SetVelocity(Vec2(0, 0));
        m_State_val_Trace.enter = true;
        m_State_val_Trace.exit = false;
    }
}

void CGasterScript::TRACE_ATTACK()
{
    Vec3 Pos = Transform()->GetRelativePos();
    Vec3 TarPos = m_pTarget->Transform()->GetRelativePos();
    float fDistance = (Transform()->GetRelativePos() - m_pTarget->Transform()->GetRelativePos()).Length();
    if (m_State_val_Trace.enter == true)
    {
        RigidBody2D()->SetFriction(100.f);
        RigidBody2D()->SetVelocity(Vec2(0, 0));
        m_State_val_Trace.tick = true;
        m_State_val_Trace.enter = false;
    }
    if (m_State_val_Trace.tick == true)
    {


        CGameObject* SPar = GetOwner()->GetChildwstring(L"GUNSPARK");
        CGameObject* Child = GetOwner()->GetChildwstring(L"Gun");
        CGameObject* RightArm = GetOwner()->GetChildwstring(L"RightArm");
        CGameObject* LeftArm = GetOwner()->GetChildwstring(L"LeftArm");
        if (TarPos.x < Pos.x)
            m_bTurn = true;
        else
            m_bTurn = false;
        if (CPlayerMgr::GetInst()->GetDead() == 0)
        {
            if (fDistance < 100)
            {
                IsaniChange(GANGSTER_ANIMATOR::ATTACK_ALMOST);
                Child->Transform()->SetRelativeScale(0, 0, 0);
                RightArm->Transform()->SetRelativeScale(0, 0, 0);
                LeftArm->Transform()->SetRelativeScale(0, 0, 0);
            }
            else
            {

                if (Child != nullptr)
                {
                    Child->Transform()->SetRelativeScale(250, 250, 0);
                    RightArm->Transform()->SetRelativeScale(250, 250, 0);
                    LeftArm->Transform()->SetRelativeScale(1, 1, 0);
                    Vec3 vRot = Transform()->GetRelativeRotation();
                    Vec3 Targetpos = m_pTarget->Transform()->GetRelativePos();
                    float height;
                    float bottom;
                    float YRot = 0;
                    if (Targetpos.x > Pos.x)
                    {
                        height = Targetpos.y - Pos.y;
                        bottom = Targetpos.x - Pos.x;
                        Vec3 Arm = RightArm->Transform()->GetRelativePos();
                        Arm.z = -5;
                        RightArm->Transform()->SetRelativePos(Arm);
                        Pos = Transform()->GetRelativePos() - m_pTarget->Transform()->GetRelativePos();
                    }
                    else
                    {
                        height = Pos.y - Targetpos.y;
                        bottom = Pos.x - Targetpos.x;
                        YRot = XM_PI;
                        Vec3 Arm = RightArm->Transform()->GetRelativePos();
                        Arm.z = 5;
                        RightArm->Transform()->SetRelativePos(Arm);
                        height *= -1;
                        Pos = m_pTarget->Transform()->GetRelativePos() - Transform()->GetRelativePos();
                    }

                    float Z = atan(height / bottom);

                    Vec3 Rot = Child->Transform()->GetRelativeRotation();
                    Child->Transform()->SetRelativeRotation(Rot.x, YRot, Z);
                    Rot = LeftArm->Transform()->GetRelativeRotation();
                    LeftArm->Transform()->SetRelativeRotation(Rot.x, 0, Z);
                    Rot = RightArm->Transform()->GetRelativeRotation();
                    RightArm->Transform()->SetRelativeRotation(Rot.x, 0, Z);
                    Rot = SPar->Transform()->GetRelativeRotation();
                    SPar->Transform()->SetRelativeRotation(Rot.x, 0, Z);

                    RightArm->Animator2D()->Play_Rotation(L"RIGHTARM", true);
                    Child->Animator2D()->Play_Rotation(L"GUN", true);
                    LeftArm->Animator2D()->Play_Rotation(L"LEFTARM", true);

                    Pos.Normalize();
                    Vec3 MonPos = Transform()->GetRelativePos();

                    //Vec3 a = SPar->Transform()->GetWorldPos();
                    Vec3 b = Vec3(cosf(Z) * 63, sinf(Z) * 63, SPar->Transform()->GetRelativePos().z);
                    SPar->Transform()->SetRelativePos(b);
                    SPar->Transform()->SetRelativeRotation(0.f, 0.f, Z);
                    Vec3 a = SPar->Transform()->GetWorldPos();
                    if (m_fAcctime > 2)
                    {
                        CSoundMgr::GetInst()->FindSound(L"sound\\enemy_gun_fire.wav", 1, 0.6f, true);
                        Ptr<CPrefab> bulletPre = CResMgr::GetInst()->FindRes<CPrefab>(L"prefab\\1.pref");
                        CGameObject* bullet = bulletPre->Instantiate();
                        Vec3 pos = SPar->Transform()->GetWorldPos();
                        Instantiate(bullet, pos, 4);
                        SPar->Animator2D()->Play_Rotation(L"GUNSPARK_1", false);
                        m_fAcctime = 0;
                    }
                }
                IsaniChange(GANGSTER_ANIMATOR::AIM);
            }
        }
    }
    if (m_State_val_Trace.exit == true)
    {
        if (fDistance > 100)
        {
            if (TarPos.x < Pos.x)
                m_eTraceState = GANGSTER_TRACE::LEFT_WALK;
            else
                m_eTraceState = GANGSTER_TRACE::RIGHT_WALK;
        }
        m_State_val_Trace.enter = true;
        m_State_val_Trace.exit = false;
    }
}

void CGasterScript::DEAD()
{
    if (m_StateValue.enter == true)
    {
        IsaniChange(GANGSTER_ANIMATOR::HURT_FLY);

        CGameObject* SPar = GetOwner()->GetChildwstring(L"GUNSPARK");
        CGameObject* Child = GetOwner()->GetChildwstring(L"Gun");
        CGameObject* RightArm = GetOwner()->GetChildwstring(L"RightArm");
        CGameObject* LeftArm = GetOwner()->GetChildwstring(L"LeftArm");
        Child->Transform()->SetRelativeScale(0, 0, 0);
        RightArm->Transform()->SetRelativeScale(0, 0, 0);
        LeftArm->Transform()->SetRelativeScale(0, 0, 0);

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
            IsaniChange(GANGSTER_ANIMATOR::HURT_GROUND);
            m_StateValue.tick = false;
        }

    }

}

void CGasterScript::exit()
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
void CGasterScript::ChangeState(State _State)
{
    exit();
    m_State = _State;
    m_StateValue.enter = true;
}

void CGasterScript::AniState()
{
    if (m_eCurGruntAniState == GANGSTER_ANIMATOR::AIM)
    {
        m_pCurMation = Animator2D()->Play(L"AIM", false, m_bTurn);
        CGameObject* SPar = GetOwner()->GetChildwstring(L"GUNSPARK");
       
    }
    m_bIschange = false;
    if (m_eGruntAniState != m_eCurGruntAniState)
    {
        switch (m_eCurGruntAniState)
        {
        case GANGSTER_ANIMATOR::AIM:
        {
            m_pCurMation = Animator2D()->Play(L"AIM", false, m_bTurn);
            CGameObject* SPar = GetOwner()->GetChildwstring(L"GUNSPARK");
            
        }
            break;
        
        case GANGSTER_ANIMATOR::ATTACK_ALMOST:
            m_pCurMation = Animator2D()->Play(L"ATTACK_ALMOST", true, m_bTurn);
            break;
        case GANGSTER_ANIMATOR::HURT_FLY:
            m_pCurMation = Animator2D()->Play(L"HURT_FLY", false, m_bTurn);
            break;
        case GANGSTER_ANIMATOR::HURT_GROUND:
            Animator2D()->Play(L"HURT_GROUND", false, m_bTurn);
            break;
        case GANGSTER_ANIMATOR::IDLE:
            Animator2D()->Play(L"IDLE", true, m_bTurn);
            break;
        case GANGSTER_ANIMATOR::RUN:
            Animator2D()->Play(L"RUN", true, m_bTurn);
            break;
        case GANGSTER_ANIMATOR::TURN:
            m_pCurMation = Animator2D()->Play(L"TURN", false, m_bTurn);
            break;
        case GANGSTER_ANIMATOR::WALK:
            Animator2D()->Play(L"WALK", true, m_bTurn);
        default:
            break;
        }
        m_eGruntAniState = m_eCurGruntAniState;
    }
}

void CGasterScript::BeginOverlap(CCollider2D* _pOther)
{
    RigidBody2D()->SetGround(true);
    if (_pOther->GetOwner()->GetName() == L"SWORD")
    {
        CSoundMgr::GetInst()->FindSound(L"sound\\enemy_drath_sword.wav", 1, 0.6f, true);
        m_State = State::DEAD;
        m_StateValue.enter = true;
    }
    if (_pOther->GetOwner()->GetLayerIdx() == 4)
    {
        //::GetInst()->FindSound(L"sound\\enemy_drath_sword.wav", 1, 0.6f, true);
        m_State = State::DEAD;
        m_StateValue.enter = true;
    }
    if (_pOther->GetOwner()->GetName() == L"Player")
    {
        CSoundMgr::GetInst()->FindSound(L"sound\\enemy_punch_hit.wav", 1, 0.6f, true);
        if (m_eTraceState == GANGSTER_TRACE::ATTACK)
            _pOther->GetOwner()->GetScript<CPlayerScript>()->IsDead(GetOwner()->Transform()->GetRelativePos());
    }
}

void CGasterScript::Overlap(CCollider2D* _pOther)
{
    RigidBody2D()->SetGround(true);
}

void CGasterScript::EndOverlap(CCollider2D* _pOther)
{
    
}


