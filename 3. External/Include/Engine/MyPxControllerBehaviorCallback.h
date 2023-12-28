#pragma once
#include "PhysXMgr.h"
//#include "PhysX\characterkinematic\PxControllerBehavior.h"
class MyPxControllerBehaviorCallback :
    public PxControllerBehaviorCallback
{
public:
	// PxControllerBehaviorCallback
	virtual PxControllerBehaviorFlags		getBehaviorFlags(const PxShape&, const PxActor&) override { return PxControllerBehaviorFlags(0); }

	virtual PxControllerBehaviorFlags		getBehaviorFlags(const PxController&)override { return PxControllerBehaviorFlags(0); }

	virtual PxControllerBehaviorFlags		getBehaviorFlags(const PxObstacle&)override { return PxControllerBehaviorFlags(0); }
public:
	MyPxControllerBehaviorCallback() {};
	~MyPxControllerBehaviorCallback() {};
};

