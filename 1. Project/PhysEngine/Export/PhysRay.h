#pragma once
#include "PhysEngineDLL.h"
#ifdef _DEBUG
#pragma comment(lib, "Engine\\Engine_debug.lib")
#else
#pragma comment(lib, "Engine\\Engine.lib")
#endif
//using namespace DirectX;
#include <Engine/global.h>

//Ray와 충돌한 객체의 정보
struct PHYS_ENGINEDLL RaycastHit
{
	Vec3 HitPoint	= { 0,0,0 };		//충돌 위치
	int HitCount		= 0;				//충돌한 오브젝트 개수
	int FaceIndex		= 0;				//충돌한 오브젝트의 face인덱슨
};

class PHYS_ENGINEDLL PhysRayCast
{
public:
	PhysRayCast();
	~PhysRayCast();

	//Ray의 쏘기 시작한 위치
	void SetStartOrigin(float& x,float& y,float& z);
	void SetStartOrigin(float&& x,float&& y,float&& z);

	//Ray의 방향
	void SetDirection(float& x, float& y, float& z);
	void SetDirection(float&& x, float&& y, float&& z);

	//Ray의 길이
	void SetMaxDistance(float& _MaxDistance);
	void SetMaxDistance(float&& _MaxDistance);


	Vec3 Origin;			//쏘기시작한 위치
	Vec3 Direction;		//쏘는 방향

	RaycastHit Hit;			//충돌한 객체 정보
	float MaxDistance;		//최대 거리
};
