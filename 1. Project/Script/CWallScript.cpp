#include "pch.h"
#include "CWallScript.h"

#include "CPlayerScript.h"

CWallScript::CWallScript()
	: CScript(WALLSCRIPT)
{
}

CWallScript::~CWallScript()
{
}

void CWallScript::tick()
{
	int a = 0;
}

void CWallScript::BeginOverlap(CCollider2D* _pOther)
{
	
	if (_pOther->GetOwner()->GetName() == L"Player")
	{
		_pOther->GetOwner()->GetScript<CPlayerScript>()->IsWall();
		_pOther->GetOwner()->RigidBody2D()->SetWall(true);
		_pOther->GetOwner()->RigidBody2D()->SetGround(false);
		Vec3 Pos = _pOther->GetOwner()->Transform()->GetRelativePos();
		if (Transform()->GetRelativePos().x < Pos.x)
		{
			_pOther->GetOwner()->Transform()->SetRelativeRotation(0.f, XM_PI, 0.f);
		}
		else
		{
			_pOther->GetOwner()->Transform()->SetRelativeRotation(0.f, 0.f, 0.f);
		}

		if (Transform()->GetRelativePos().x < Pos.x && !_pOther->GetOwner()->RigidBody2D()->GetWall())
		{
			Pos.x += 2;
		}
		else
		{
			Pos.x -= 0.5f;
		}
		_pOther->GetOwner()->Transform()->SetRelativePos(Pos);
		
		//_pOther->GetOwner()->Transform()->SetRelativePos(Pos);
	}
	//Defend(_pOther);
}

void CWallScript::Overlap(CCollider2D* _pOther)
{
}

void CWallScript::EndOverlap(CCollider2D* _pOther)
{
	if (_pOther->GetOwner()->GetName() == L"Player")
	{

		_pOther->GetOwner()->RigidBody2D()->SetWall(false);
	}
}





void CWallScript::Defend(CCollider2D* _pOther)
{
	if (L"Player" == _pOther->GetOwner()->GetName())
	{
		// 뭐지이건
		float fLen = (float)fabs(_pOther->Transform()->GetRelativePos().y - Transform()->GetRelativePos().y);
		float fScale = (_pOther->Transform()->GetRelativeScale().y / 2.f) + (Transform()->GetRelativeScale().y / 2.f);
		// 플레이어위치
		Vec3 PlayerPos = _pOther->Transform()->GetRelativePos();


		// 타일의 y값을 구하기
		float if1 = (Transform()->GetRelativePos().y - (Transform()->GetWorldScale().y / 2));
		float if2 = (Transform()->GetRelativePos().y - (Transform()->GetWorldScale().y / 2)) + Transform()->GetRelativeScale().y;

		// 플레이어 
		float PlayerPosA = PlayerPos.y + _pOther->Transform()->GetWorldScale().y / 2 - 5.f;
		float PlayerPosB = PlayerPos.y - _pOther->Transform()->GetWorldScale().y / 2 + 5.f;

		// 밀리는 값
		float bouns = 1.f;
		if (m_bColliderState)
		{
			// 타일의 y와 플레이어의 y을 비교해 안에 들어가 있으면 y값 코드 진행
			if (PlayerPosA >= (if1) && PlayerPosB <= (if2))
			{
				fLen = (float)fabs(_pOther->Transform()->GetRelativePos().x - Transform()->GetRelativePos().x);
				fScale = (_pOther->Transform()->GetWorldScale().x / 2.f + (Transform()->GetWorldScale().x / 2.f));

				// y축에서 접근한다면 왼쪽인지 오른쪽인지 판단
				if ((_pOther->Transform()->GetRelativePos().x < Transform()->GetRelativePos().x))
				{
					if (fLen < fScale)
					{
						Vec3 vPlayerPos = _pOther->Transform()->GetRelativePos();
						vPlayerPos.x -= (fScale - fLen) + bouns;

						_pOther->Transform()->SetRelativePos(vPlayerPos);
						return;
					}
				}
				else
				{
					if (fLen < fScale)
					{
						Vec3 vPlayerPos = _pOther->Transform()->GetRelativePos();
						vPlayerPos.x += (fScale - fLen) + bouns;
						_pOther->Transform()->SetRelativePos(vPlayerPos);
						return;
					}
				}
			}
		}

		// x축에서 접근한다면 위쪽인지 아닌지 판단
		if ((_pOther->Transform()->GetRelativePos().y < Transform()->GetRelativePos().y))
		{
			m_bColliderState = false;
			if (fLen < fScale)
			{
				Vec3 vPlayerPos = _pOther->Transform()->GetRelativePos();
				vPlayerPos.y -= (fScale - fLen) + bouns;
				_pOther->Transform()->SetRelativePos(vPlayerPos);
				return;
			}
		}
		else
		{
			if (fLen < fScale)
			{
				Vec3 vPlayerPos = _pOther->Transform()->GetRelativePos();
				vPlayerPos.y += (fScale - fLen) + bouns;
				_pOther->Transform()->SetRelativePos(vPlayerPos);
				return;
			}
		}
	}
}