#pragma once
#include "CRigidBody3D.h"

class CPlayerController
	: public PxUserControllerHitReport
	, public PxControllerBehaviorCallback
{
	CGameObject*			m_pOwner;
	PxMaterial*				m_pMaterial;
	PxCapsuleController*	m_pCapsuleController;


public:
	void CreatePlayerController();

	PxRigidDynamic* GetActor() { return m_pCapsuleController->getActor(); }
	PxCapsuleController* GetController() { return m_pCapsuleController; }

private:
	// Implements PxUserControllerHitReport
	virtual void							onShapeHit(const PxControllerShapeHit& hit) override;
	virtual void							onControllerHit(const PxControllersHit& hit) override;
	virtual void							onObstacleHit(const PxControllerObstacleHit& hit) override;

	// Implements PxControllerBehaviorCallback
	virtual PxControllerBehaviorFlags		getBehaviorFlags(const PxShape&, const PxActor&)override { return PxControllerBehaviorFlags(0); };
	virtual PxControllerBehaviorFlags		getBehaviorFlags(const PxController&)override { return PxControllerBehaviorFlags(0); };
	virtual PxControllerBehaviorFlags		getBehaviorFlags(const PxObstacle&)override { return PxControllerBehaviorFlag::eCCT_CAN_RIDE_ON_OBJECT; };

public:
	CPlayerController();
	CPlayerController(CGameObject* _owner, PxMaterial* _material, PxCapsuleController* _controller);
	~CPlayerController();
};

