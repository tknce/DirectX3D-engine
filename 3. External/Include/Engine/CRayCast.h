#pragma once
//Ray�� �浹�� ��ü�� ����
struct RaycastHit
{
	Vec3 HitPoint = { 0,0,0 };		//�浹 ��ġ
	int HitCount = 0;				//�浹�� ������Ʈ ����
	int FaceIndex = 0;				//�浹�� ������Ʈ�� face�ε���
};

class CRayCast
{
public:
	Vec3 m_Origin;			//�������� ��ġ
	Vec3 m_Direction;		//��� ����

	RaycastHit m_Hit;			//�浹�� ��ü ����
	float m_MaxDistance;		//�ִ� �Ÿ�

public:
	//Ray�� ��� ������ ��ġ
	void SetStartOrigin(float& x, float& y, float& z) { m_Origin = { x,y,z }; }
	void SetStartOrigin(float&& x, float&& y, float&& z) { m_Origin = { x,y,z }; }
	void SetStartOrigin(Vec3 start) { m_Origin = start; }


	//Ray�� ����
	void SetDirection(float& x, float& y, float& z) { m_Direction = { x,y,z }; }
	void SetDirection(float&& x, float&& y, float&& z) { m_Direction = { x,y,z }; }
	void SetDirection(Vec3 start) { m_Direction = start; }

	//Ray�� ����
	void SetMaxDistance(float& _MaxDistance) { m_MaxDistance = _MaxDistance; }
	void SetMaxDistance(float&& _MaxDistance) { m_MaxDistance = _MaxDistance; }




public:
	CRayCast();
	~CRayCast();
};

