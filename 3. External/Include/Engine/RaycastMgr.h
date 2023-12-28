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

	//가장 먼저 충돌한 객체의 충돌여부
	bool RayCast(CRayCast* ray);

	//충돌한 모든 객체의 여부
	bool RayCastMultiple(CRayCast* ray);

	bool registerRaycastCCDObject(PxRigidDynamic* actor, PxShape* shape);
	void doRaycastCCD(bool doDynamicDynamicCCD);
	bool performRaycastCCD(PxTransform& pose, PxShape* shape, PxVec3& velocity, bool doDynamicDynamicCCD);

	void removeActor(PxActor* actor);
public:
	RaycastMgr();
	~RaycastMgr();

};

