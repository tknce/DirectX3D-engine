#pragma once
#include "PhysXMgr.h"
#include <PhysX/PxSimulationEventCallback.h>

class EventCallBack :
    public PxSimulationEventCallback
{
public:
	// PxSimulationEventCallback을(를) 통해 상속됨
	virtual void onConstraintBreak(PxConstraintInfo* constraints, PxU32 count) override;

	virtual void onWake(PxActor** actors, PxU32 count) override;

	virtual void onSleep(PxActor** actors, PxU32 count) override;

	virtual void onContact(const PxContactPairHeader& pairHeader, const PxContactPair* pairs, PxU32 nbPairs) override;

	virtual void onTrigger(PxTriggerPair* pairs, PxU32 count) override;

	virtual void onAdvance(const PxRigidBody* const* bodyBuffer, const PxTransform* poseBuffer, const PxU32 count) override { int a = 0; };


private:
	bool isTargetPush = false;
	bool isOtherPush = false;
	void PushExitObject(CGameObject* Other, CGameObject* Target) {};
public:
	EventCallBack();
	~EventCallBack();
};

