#include "pch.h"
#include "CPlayerScript.h"

#include "CMissileScript.h"
#include <Engine\CMaterial.h>
#include <Engine/CAnimation2D.h>
#include <Engine/GlobalComponent.h>
#include <Engine/CLevelMgr.h>
#include <Engine/CPlayerMgr.h>
#include <Engine/CSoundMgr.h>

CPlayerScript::CPlayerScript()
	: CScript(PLAYERSCRIPT)
	, m_fAccTime(0.f)
	, m_fSpeed(100.f)
	, m_iJumpCount(1)
	, m_bRazerDead(false)
{
	AddScriptParam(SCRIPT_PARAM::FLOAT, "Player MoveSpeed",  &m_fSpeed);
	//PLAYER_STATE_FIGHT STATE = 





}

CPlayerScript::~CPlayerScript()
{
}

void CPlayerScript::begin()
{
	if (RigidBody2D() != nullptr)
	{
		RigidBody2D()->SetGravity(false);
		RigidBody2D()->SetGround(false);
	}
	vector<CGameObject*> child = GetOwner()->GetChildObject();
	for (size_t i = 0; i < child.size(); ++i)
	{
		if (child[i] != nullptr)
		{
			if (L"SWORD" == child[i]->GetName())
			{
				CAnimator2D* Ani = (CAnimator2D*)child[i]->GetComponent(COMPONENT_TYPE::ANIMATOR2D);
				m_pSword = child[i];
				m_CurAni = Ani->Play(L"SWORD_EFFECT", false);
			}
		}
	}
	int a = 0;
}

void CPlayerScript::tick()
{
	m_fAccTime += DT;
	m_fPoweroverwhming += DT;
	if (m_irewind == 1 && CPlayerMgr::GetInst()->GetDead() == 0)
		SetRewind();

	if (m_fPoweroverwhming > 0.5f)
	{
		//m_fPoweroverwhming = 0;
		if (!m_bRazerDead && m_CurAni != nullptr)
		{
			m_vPos = Transform()->GetRelativePos();
			m_iRazerDead = m_CurAni->GetIDX();
		}

		if (CPlayerMgr::GetInst()->GetDead() == 1)
		{
			//CPlayerMgr::GetInst()->SetDead(1);
			m_irewind = 1;
			if (m_bRazerDead)
			{
				RazerDead();
			}

			GetOwner()->RigidBody2D()->SetMaxSpeed(500.f);
			return;
		}
	}
	else
	{
		CPlayerMgr::GetInst()->SetDead(0);
	}
	
	GetOwner()->RigidBody2D()->SetFriction(1000);

	

	if (m_CurAni != nullptr)
		if(m_CurAni->GetFinish())
		m_pSword->Collider2D()->SetScale(Vec2(0, 0));

	if (!GetOwner()->RigidBody2D()->GetWall())
	{
		Move();
		KeyState();
	}
	
	else
	{
		IsWall();
	}
	


	AniState();
	Skill();

	if (m_fAccTime > 0.7f)
	{
		
		IsAttack();
		
	}
	
	m_vPreVel = RigidBody2D()->GetVelocity();
}

void CPlayerScript::Move()
{
	
	if (RigidBody2D() != nullptr)
	{
		Vec3 Pos = GetOwner()->Transform()->GetWorldPos();
		Vec2 MousePos = CKeyMgr::GetInst()->GetCameraMousePos();
		// 점프
		if (KEY_PRESSED(KEY::W))
		{
			m_fAcctime_jump += DT;
			if (m_iJumpCount == 1)
			{
				if (m_fAcctime_jump < 0.2)
				{
					RigidBody2D()->SetGround(false);
					
					RigidBody2D()->SetVelocity(Vec2(RigidBody2D()->GetVelocity().x, 600.f));
				}
				else
				{
					m_fAcctime_jump = 0;
					m_iJumpCount = 0;
				}		
			}
		}

		// 아래 누른 상태로 공격시 만약 뚫리는 
		if (KEY_PRESSED(KEY::S))
		{
			m_fPoweroverwhming = 0;
			
			// 구르기
			if(KEY_TAP(KEY::A))
			{
				
				return;
			}
			// 구르기
			if(KEY_TAP(KEY::D))
			{
				return;
			}
			RigidBody2D()->AddForce(Vec2(0.f, -5000.f));
		}
		if (m_Fight != PLAYER_STATE_FIGHT::IDLE_TO_RUN)
		{
			if (KEY_TAP(KEY::A))
			{
				RigidBody2D()->AddForce(Vec2(-m_fSpeed, 0.f));
			}
			if (KEY_PRESSED(KEY::A))
			{
				RigidBody2D()->AddForce(Vec2(-m_fSpeed, 0.f));

			}

			if (KEY_TAP(KEY::D))
			{
				RigidBody2D()->AddForce(Vec2(m_fSpeed, 0.f));
			}
			if (KEY_PRESSED(KEY::D))
			{
				RigidBody2D()->AddForce(Vec2(m_fSpeed, 0.f));
			}
		}
		else
		{
			m_fSpeed = 1200.f;
			if (KEY_TAP(KEY::A))
			{
				RigidBody2D()->AddForce(Vec2(-m_fSpeed, 0.f));
			}
			if (KEY_PRESSED(KEY::A))
			{
				RigidBody2D()->AddForce(Vec2(-m_fSpeed, 0.f));

			}

			if (KEY_TAP(KEY::D))
			{
				RigidBody2D()->AddForce(Vec2(m_fSpeed, 0.f));
			}
			if (KEY_PRESSED(KEY::D))
			{
				RigidBody2D()->AddForce(Vec2(m_fSpeed, 0.f));
			}
		}
		

		
	}
}

void CPlayerScript::KeyState()
{
	if (RigidBody2D()->isGround())
	{
		if (m_Fight == PLAYER_STATE_FIGHT::IDLE)
		{
			GetOwner()->RigidBody2D()->SetFriction(10000);
			
			//GetOwner()->RigidBody2D()->SetFriction(10000);
		}
			
		// 좌우 판단
		if (KEY_TAP(KEY::A) || KEY_PRESSED(KEY::A))
		{
			
			if (L"IDLE_TO_RUN" == m_CurAni->GetName() )
			{
				if (m_CurAni->GetFinish() )
				{
					CSoundMgr::GetInst()->FindSound(L"sound\\player\\player_land.wav", 1, 0.6f);
					m_Fight = PLAYER_STATE_FIGHT::RUN;
					m_fSpeed += 1000 * DT;
					if (m_fSpeed < 5000)
						m_fSpeed = 5000;
					m_bTurn = true;
				}
			}
			if(m_Fight != PLAYER_STATE_FIGHT::RUN)
			{
				m_Fight = PLAYER_STATE_FIGHT::IDLE_TO_RUN;
				m_bTurn = true;
			}
			
		}

		if (KEY_TAP(KEY::D) || KEY_PRESSED(KEY::D))
		{
			//CPlayerMgr::GetInst()->SetDead(2);
			
			if (L"IDLE_TO_RUN" == m_CurAni->GetName() )
			{

				if (m_CurAni->GetFinish())
				{
					CSoundMgr::GetInst()->FindSound(L"sound\\player\\player_land.wav", 1, 0.6f);
					m_Fight = PLAYER_STATE_FIGHT::RUN;
					m_fSpeed += 1000 * DT;
					if (m_fSpeed < 5000)
						m_fSpeed = 5000;
					m_bTurn = false;
				}
			}
			if (m_Fight != PLAYER_STATE_FIGHT::RUN)
			{
				m_Fight = PLAYER_STATE_FIGHT::IDLE_TO_RUN;
				m_bTurn = false;
			}
		}

		// 아래 누른 상태로 좌우 입력시 구르기
		if (KEY_TAP(KEY::S))
		{
			CSoundMgr::GetInst()->FindSound(L"sound\\player\\player_roll.wav", 1, 0.6f);
			// 구르기
			if (RigidBody2D()->GetVelocity().x < 0)
			{
				m_Fight = PLAYER_STATE_FIGHT::ROLL;
				m_bTurn = true;
				return;
			}
			// 구르기
			if (RigidBody2D()->GetVelocity().x > 0)
			{
				m_Fight = PLAYER_STATE_FIGHT::ROLL;
				m_bTurn = false;
				return;
			}
			m_Fight = PLAYER_STATE_FIGHT::FALL;
		}
	}
	else
	{
		if (RigidBody2D()->GetVelocity().x < 0)
		{
			m_Fight = PLAYER_STATE_FIGHT::JUMP;
			m_bTurn = true;
		}

		if (RigidBody2D()->GetVelocity().x > 0)
		{
			m_Fight = PLAYER_STATE_FIGHT::JUMP;
			m_bTurn = false;
		}
	}

	if (KEY_TAP(KEY::W))
	{
		//IsDead(Vec3(5, 5, 5));
		if (m_iJumpCount == 1)
		{
			CSoundMgr::GetInst()->FindSound(L"sound\\player\\player_jump.wav", 1, 0.6f);
			if (RigidBody2D()->GetVelocity().x < 0)
			{
				m_Fight = PLAYER_STATE_FIGHT::JUMP;
				m_bTurn = true;
			}

			if (RigidBody2D()->GetVelocity().x > 0)
			{
				m_Fight = PLAYER_STATE_FIGHT::JUMP;
				m_bTurn = false;
			}
			m_iJumpCount = 0;
		}
	}
	if (KEY_TAP(KEY::LBTN))
	{
		if (m_fAccTime > 0.7f)
		{
			m_pSword->Collider2D()->SetScale(Vec2(1.f, 0.4f));
			m_pSword->Animator2D()->Play(L"SWORD_EFFECT", false);

			m_Fight = PLAYER_STATE_FIGHT::ATTACK;
			
		}	
	}


	if (
		CKeyMgr::GetInst()->GetKeyState(KEY::A) == KEY_STATE::NONE &&
		CKeyMgr::GetInst()->GetKeyState(KEY::D) == KEY_STATE::NONE &&
		CKeyMgr::GetInst()->GetKeyState(KEY::W) == KEY_STATE::NONE &&
		CKeyMgr::GetInst()->GetKeyState(KEY::S) == KEY_STATE::NONE &&
		CKeyMgr::GetInst()->GetKeyState(KEY::LBTN) == KEY_STATE::NONE &&
		RigidBody2D()->isGround())
	{
		if (RigidBody2D()->GetVelocity().x < 0)
			m_Fight = PLAYER_STATE_FIGHT::IDLE;
		if(m_Fight != PLAYER_STATE_FIGHT::IDLE)
		m_Fight = PLAYER_STATE_FIGHT::IDLE;
	}
		
}






void CPlayerScript::AniState()
{
	if (m_Fight == PLAYER_STATE_FIGHT::IDLE_TO_RUN && m_PreState != PLAYER_STATE_FIGHT::IDLE_TO_RUN)
		m_bAniState = true;
	if(m_CurAni != nullptr)
	if (m_Fight == PLAYER_STATE_FIGHT::FLIP)
		m_bAniState = true;
	if (m_PreState == PLAYER_STATE_FIGHT::FLIP && !m_CurAni->GetFinish())
	{
		if(RigidBody2D()->GetWall())
		{
			
		}
		else
		{
			return;
		}
		
	}
		
	//m_Fight == PLAYER_STATE_FIGHT::JUMP &&
	if (m_vPreVel != RigidBody2D()->GetVelocity() &&
		m_PreState != PLAYER_STATE_FIGHT::ATTACK && 
		m_Fight != PLAYER_STATE_FIGHT::FLIP &&
		m_PreState != PLAYER_STATE_FIGHT::FLIP)
	{
		if (!RigidBody2D()->GetStair())
		{
			if (RigidBody2D()->GetVelocity().y < 0.f && m_Fight != PLAYER_STATE_FIGHT::ATTACK)
			{
				if (!RigidBody2D()->GetWall())
					m_Fight = PLAYER_STATE_FIGHT::FALL;
			}
			if (RigidBody2D()->GetVelocity().y > 0.f && m_Fight != PLAYER_STATE_FIGHT::ATTACK)
			{
				m_Fight = PLAYER_STATE_FIGHT::JUMP;
			}
		}
		

		if (RigidBody2D()->GetVelocity().x < 0.f && m_Fight != PLAYER_STATE_FIGHT::ATTACK)
		{
			GetOwner()->Transform()->SetRelativeRotation(0.f, XM_PI, 0.f);
		}
		if (RigidBody2D()->GetVelocity().x > 0.f && m_Fight != PLAYER_STATE_FIGHT::ATTACK)
		{
			GetOwner()->Transform()->SetRelativeRotation(0.f, 0.f, 0.f);
		}
	}
	if (m_PreState == PLAYER_STATE_FIGHT::RUN && m_Fight == PLAYER_STATE_FIGHT::IDLE)
	{
		m_Fight = PLAYER_STATE_FIGHT::RUN_TO_IDLE;
	}
	if (m_bStair)
		m_Fight = PLAYER_STATE_FIGHT::RUN;
	if (m_CurAni != nullptr)
	{
		if (m_CurAni->GetName() == L"RUN_TO_IDLE" && !m_CurAni->GetFinish() && m_Fight == PLAYER_STATE_FIGHT::IDLE)
			return;
	}
	
	if (m_PreState != m_Fight || m_bAniState)
	{
		if (m_PreState == PLAYER_STATE_FIGHT::ATTACK && !Animator2D()->GetCurAnim()->GetFinish())
		{
			return;
		}
		if (m_PreState == PLAYER_STATE_FIGHT::ROLL && !Animator2D()->GetCurAnim()->GetFinish())
		{
			return;
		}


		else
		{
			
			m_PreState = m_Fight;
			switch (m_Fight)
			{
			case PLAYER_STATE_FIGHT::IDLE:
				m_CurAni = Animator2D()->Play(L"IDLE", true, m_bTurn);
				break;
			case PLAYER_STATE_FIGHT::RUN:
				m_CurAni = Animator2D()->Play(L"RUN", true, m_bTurn);
				break;
			case PLAYER_STATE_FIGHT::IDLE_TO_RUN:
				m_CurAni = Animator2D()->Play(L"IDLE_TO_RUN", false, m_bTurn);
				break;
			case PLAYER_STATE_FIGHT::RUN_TO_IDLE:
				m_CurAni = Animator2D()->Play(L"RUN_TO_IDLE", false, m_bTurn);
				break;
			case PLAYER_STATE_FIGHT::WALK:
				m_CurAni = Animator2D()->Play(L"WALK", true, m_bTurn);
				break;
			case PLAYER_STATE_FIGHT::IDLE_TO_WALK:
				m_CurAni = Animator2D()->Play(L"IDLE_TO_WALK", false, m_bTurn);
				break;
			case PLAYER_STATE_FIGHT::WALK_TO_IDLE:
				m_CurAni = Animator2D()->Play(L"WALK_TO_IDLE", false, m_bTurn);
				break;
			case PLAYER_STATE_FIGHT::WALL_SLIDE:
				m_CurAni = Animator2D()->Play(L"WALL_SLIDE", true, m_bTurn);
				break;
			case PLAYER_STATE_FIGHT::WALL_GRAB:
				m_CurAni = Animator2D()->Play(L"WALL_GRAB", true, m_bTurn);
				break;
			case PLAYER_STATE_FIGHT::PRECROUCH: // 웅크리기
				m_CurAni = Animator2D()->Play(L"PRECROUCH", true, m_bTurn);
				break;
			case PLAYER_STATE_FIGHT::ROLL:
				m_CurAni = Animator2D()->Play(L"ROLL", true, m_bTurn);
				break;
			case PLAYER_STATE_FIGHT::ATTACK:
				m_CurAni = Animator2D()->Play_Rotation(L"ATTACK", false, m_bTurn);
				break;
			case PLAYER_STATE_FIGHT::JUMP:
				m_CurAni = Animator2D()->Play(L"JUMP", true, m_bTurn);
				break;
			case PLAYER_STATE_FIGHT::FALL:
				m_CurAni = Animator2D()->Play(L"FALL", true, m_bTurn);
				break;
			case PLAYER_STATE_FIGHT::HURTGROUND:
				m_CurAni = Animator2D()->Play(L"HURTGROUND", false, m_bTurn);
				break;
			case PLAYER_STATE_FIGHT::FLIP:
				m_CurAni = Animator2D()->Play(L"FLIP", false, m_bTurn);
				break;
			case PLAYER_STATE_FIGHT::END:
				m_CurAni = Animator2D()->Play(L"IDLE", true, m_bTurn);
				break;
			default:
				break;
			}
			
		}
		m_bAniState = false;

	}
}

void CPlayerScript::Skill()
{
	if (KEY_PRESSED(KEY::LSHIFT))
	{
		
		m_ibskill = 1;
		m_vColor = Vec4(0.4, 0.7, 0.85, 0);

	}
	else
	{
		m_ibskill = 0;
		m_vColor = Vec4(0, 0, 0, 0);
	}
	//m_ibskill = 1;
	MeshRender()->GetCurMaterial(0)->SetScalarParam(SCALAR_PARAM::INT_0, &m_ibskill);
	MeshRender()->GetCurMaterial(0)->SetScalarParam(SCALAR_PARAM::VEC4_0, &m_vColor);
}

void CPlayerScript::IsDead(Vec3 _Pos)
{
	if (m_PreState != PLAYER_STATE_FIGHT::ROLL)
	{
		if (CPlayerMgr::GetInst()->GetDead() != 2)
		{
			CSoundMgr::GetInst()->FindSound(L"sound\\player\\player_dead.wav", 1, 0.6f);
			Vec3 Dir = GetOwner()->Transform()->GetRelativePos() - _Pos;
			Dir.Normalize();
			GetOwner()->RigidBody2D()->SetMaxSpeed(50000.f);

			if (m_Fight != PLAYER_STATE_FIGHT::HURTGROUND)
			{
				m_Fight = PLAYER_STATE_FIGHT::HURTGROUND;
				GetOwner()->RigidBody2D()->SetGround(false);
				GetOwner()->RigidBody2D()->SetVelocity(Vec2(3000.f * Dir.x, 8000.f));
				CPlayerMgr::GetInst()->SetDead(1);
				GetOwner()->Animator2D()->Play_Rotation(L"HURTGROUND", false, m_bTurn);
			}
		}
	}
}

void CPlayerScript::RazerDead()
{
	if(CPlayerMgr::GetInst()->Getruturn())
		m_bRazerDead = false;
	Transform()->SetRelativePos(m_vPos);
	
	Animator2D()->Play_Load(m_CurAni->GetName(), m_iRazerDead);

}

void CPlayerScript::IsWall()
{
	CSoundMgr::GetInst()->FindSound(L"sound\\player\\player_wallslide.wav", 1, 0.6f);
	//GetOwner()->RigidBody2D()->SetGround(false);
	if (KEY_TAP(KEY::A) || KEY_TAP(KEY::D))
	{
		GetOwner()->RigidBody2D()->SetWall(false);
	}
	IsAttack();
	if (KEY_TAP(KEY::LBTN))
	{
		
			m_pSword->Collider2D()->SetScale(Vec2(1.f, 0.4f));
			m_pSword->Animator2D()->Play(L"SWORD_EFFECT", false);

			m_Fight = PLAYER_STATE_FIGHT::ATTACK;
		
	}
	else
	{
		m_Fight = PLAYER_STATE_FIGHT::WALL_GRAB;
	}
	if (KEY_TAP(KEY::SPACE))
	{
		CSoundMgr::GetInst()->FindSound(L"sound\\player\\player_wallslide.wav", 1, 0.6f);
		GetOwner()->RigidBody2D()->SetWall(false);
		m_Fight = PLAYER_STATE_FIGHT::FLIP;
		if (Transform()->GetRelativeRotation().y != 0)
		{
			RigidBody2D()->SetVelocity(Vec2(5000.f, 500.f));
		}
		else
		{
			RigidBody2D()->SetVelocity(Vec2(-5000.f, 500.f));
		}
	}
	

}

void CPlayerScript::IsAttack()
{
	if (KEY_TAP(KEY::LBTN))
	{
		CSoundMgr::GetInst()->FindSound(L"sound\\player\\Player_slash_1.wav", 1, 0.6f);
		GetOwner()->RigidBody2D()->SetWall(false);
		Vec3 vRot = Transform()->GetRelativeRotation();
		Vec3 Pos = GetOwner()->Transform()->GetWorldPos();
		Vec2 MousePos = CKeyMgr::GetInst()->GetCameraMousePos();
		float height;
		float bottom;
		float YRot = 0;
		if (MousePos.x > Pos.x)
		{
			height = MousePos.y - Pos.y;
			bottom = MousePos.x - Pos.x;
		}
		else
		{
			height = Pos.y - MousePos.y;
			bottom = Pos.x - MousePos.x;
			YRot = XM_PI;
		}

		float Z = atan(height / bottom);
		if (true)
			//Z *= XM_PI;
			Transform()->SetRelativeRotation(0, YRot, Z);
		vector<CGameObject*> child = GetOwner()->GetChildObject();
		for (size_t i = 0; i < child.size(); ++i)
		{
			if (child[i] != nullptr)
			{
				if (L"SWORD" == child[i]->GetName())
				{
					CAnimator2D* Ani = (CAnimator2D*)child[i]->GetComponent(COMPONENT_TYPE::ANIMATOR2D);
					child[i]->Collider2D()->SetRotationZ(Z);
				}
			}
		}

		Vec3 Dir = Vec3(CKeyMgr::GetInst()->GetCameraMousePos().x, CKeyMgr::GetInst()->GetCameraMousePos().y, 0.f) - GetOwner()->Transform()->GetRelativePos();
		Dir.Normalize();
		//GetOwner()->RigidBody2D()->SetMaxSpeed(50000.f);

		GetOwner()->RigidBody2D()->SetGround(false);
		GetOwner()->RigidBody2D()->SetVelocity(Vec2(500.f * Dir.x, 800.f * (Dir.y)));
		m_fAccTime = 0;
	}
	
}

void CPlayerScript::SetRewind()
{
	float a = 0;
	m_irewind = 0;
	
	//MeshRender()->GetDynamicMaterial()->SetScalarParam(FLOAT_3, &a);
	MeshRender()->GetDynamicMaterial(0)->SetScalarParam(INT_1, &a);
	CPlayerMgr::GetInst()->SetDead(0);
}


void CPlayerScript::BeginOverlap(CCollider2D* _pOther)
{
	m_iJumpCount = 1;
	//Vec3 vPos = Transform()->GetRelativePos();
	//vPos.y += 1;
	//Transform()->SetRelativePos(vPos);
}

void CPlayerScript::Overlap(CCollider2D* _pOther)
{
	m_iJumpCount = 1;
	/*Vec3 vPos = Transform()->GetRelativePos();
	vPos.y += 1;
	Transform()->SetRelativePos(vPos);*/
}

void CPlayerScript::EndOverlap(CCollider2D* _pOther)
{
	
}