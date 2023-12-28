#pragma once
#include "PhysXMgr.h"
//#include "PhysX\characterkinematic\PxController.h"
class MyPxUserControllerHitReport :
    public PxUserControllerHitReport
{
    // PxUserControllerHitReport
	virtual void onShapeHit(const PxControllerShapeHit& hit)override {}
	virtual void onControllerHit(const PxControllersHit& hit)override {}
	virtual void onObstacleHit(const PxControllerObstacleHit& hit)override {}
public:
	MyPxUserControllerHitReport() {};
	~MyPxUserControllerHitReport() {};
};

