#pragma once
#include "PhysEngineDLL.h"
#ifdef _DEBUG
#pragma comment(lib, "Engine\\Engine_debug.lib")
#else
#pragma comment(lib, "Engine\\Engine.lib")
#endif
//using namespace DirectX;
#include <Engine/global.h>

//Ray�� �浹�� ��ü�� ����
struct PHYS_ENGINEDLL RaycastHit
{
	Vec3 HitPoint	= { 0,0,0 };		//�浹 ��ġ
	int HitCount		= 0;				//�浹�� ������Ʈ ����
	int FaceIndex		= 0;				//�浹�� ������Ʈ�� face�ε���
};

class PHYS_ENGINEDLL PhysRayCast
{
public:
	PhysRayCast();
	~PhysRayCast();

	//Ray�� ��� ������ ��ġ
	void SetStartOrigin(float& x,float& y,float& z);
	void SetStartOrigin(float&& x,float&& y,float&& z);

	//Ray�� ����
	void SetDirection(float& x, float& y, float& z);
	void SetDirection(float&& x, float&& y, float&& z);

	//Ray�� ����
	void SetMaxDistance(float& _MaxDistance);
	void SetMaxDistance(float&& _MaxDistance);


	Vec3 Origin;			//�������� ��ġ
	Vec3 Direction;		//��� ����

	RaycastHit Hit;			//�浹�� ��ü ����
	float MaxDistance;		//�ִ� �Ÿ�
};
