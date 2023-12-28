#pragma once
//Ray와 충돌한 객체의 정보
struct RaycastHit
{
	Vec3 HitPoint = { 0,0,0 };		//충돌 위치
	int HitCount = 0;				//충돌한 오브젝트 개수
	int FaceIndex = 0;				//충돌한 오브젝트의 face인덱슨
};

class CRayCast
{
public:
	Vec3 m_Origin;			//쏘기시작한 위치
	Vec3 m_Direction;		//쏘는 방향

	RaycastHit m_Hit;			//충돌한 객체 정보
	float m_MaxDistance;		//최대 거리

public:
	//Ray의 쏘기 시작한 위치
	void SetStartOrigin(float& x, float& y, float& z) { m_Origin = { x,y,z }; }
	void SetStartOrigin(float&& x, float&& y, float&& z) { m_Origin = { x,y,z }; }
	void SetStartOrigin(Vec3 start) { m_Origin = start; }


	//Ray의 방향
	void SetDirection(float& x, float& y, float& z) { m_Direction = { x,y,z }; }
	void SetDirection(float&& x, float&& y, float&& z) { m_Direction = { x,y,z }; }
	void SetDirection(Vec3 start) { m_Direction = start; }

	//Ray의 길이
	void SetMaxDistance(float& _MaxDistance) { m_MaxDistance = _MaxDistance; }
	void SetMaxDistance(float&& _MaxDistance) { m_MaxDistance = _MaxDistance; }




public:
	CRayCast();
	~CRayCast();
};

