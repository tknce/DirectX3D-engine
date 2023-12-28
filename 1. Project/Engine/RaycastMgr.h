#pragma once

#include "PhysXMgr.h"

class CRayCast;
struct RaycastCCDObject
{
	PxRigidDynamic* _actor;
	PxShape*		_shape;
};


class RaycastMgr
{
private:
	PxScene* m_Scene;
	vector<RaycastCCDObject> m_vecRegisterdObject;
public:
	void Initialize(PxScene* _Scene) { m_Scene = _Scene; }

	//���� ���� �浹�� ��ü�� �浹����
	bool RayCast(CRayCast* ray);

	//�浹�� ��� ��ü�� ����
	bool RayCastMultiple(CRayCast* ray);

	bool registerRaycastCCDObject(PxRigidDynamic* actor, PxShape* shape);
	void doRaycastCCD(bool doDynamicDynamicCCD);
	bool performRaycastCCD(PxTransform& pose, PxShape* shape, PxVec3& velocity, bool doDynamicDynamicCCD);

	void removeActor(PxActor* actor);
public:
	RaycastMgr();
	~RaycastMgr();

};

