#include "pch.h"
#include "CPlayerController.h"
#include "CGameObject.h"
#include "GlobalComponent.h"
CPlayerController::CPlayerController()
:m_pOwner(nullptr)
,m_pMaterial(nullptr)
,m_pCapsuleController(nullptr)

{
}

CPlayerController::CPlayerController(CGameObject* _owner, PxMaterial* _material, PxCapsuleController* _controller)
	:m_pOwner(_owner)
	, m_pMaterial(_material)
	, m_pCapsuleController(_controller)
{
}

CPlayerController::~CPlayerController()
{
}


void CPlayerController::CreatePlayerController()
{
	PxExtendedVec3 Pos = PxExtendedVec3(m_pOwner->Transform()->GetRelativePos().x, m_pOwner->Transform()->GetRelativePos().y + 500, m_pOwner->Transform()->GetRelativePos().z);
	PxCapsuleControllerDesc* desc = new PxCapsuleControllerDesc;
	desc->setToDefault();
	desc->height = 500.f; // 임의의 값
	desc->radius = 50.f; // 임의의 값
	desc->climbingMode = PxCapsuleClimbingMode::eEASY;
	//desc->contactOffset = 0.6f / 2.0f;
	//desc->slopeLimit = 45.f;
	desc->material = m_pMaterial;
	desc->position = Pos;
	desc->maxJumpHeight = 500.f; // player에서 지정해줘야될듯
	//desc->invisibleWallHeight = 0.0001f;
	desc->reportCallback = this;
	desc->behaviorCallback = this;
	if (!desc->isValid())
		int a = 0;
	//desc.reportCallback = (PxUserControllerHitReport*)PhysXMgr::GetInst()->GetControllerHit();
	//desc.behaviorCallback = (PxControllerBehaviorCallback*)PhysXMgr::GetInst()->GetControllerBehavior();
	//PxCapsuleController::create(PhysXMgr::GetInst()->GetScene(), &desc);
	m_pCapsuleController = static_cast<PxCapsuleController*>(PhysXMgr::GetInst()->GetControllerMgr()->createController(*desc));
	PhysXMgr::GetInst()->GetControllerMgr()->getController(0);
	delete desc;
}

void CPlayerController::onShapeHit(const PxControllerShapeHit& hit)
{
	int a = 0;
}

void CPlayerController::onControllerHit(const PxControllersHit& hit)
{
	int a = 0;
}

void CPlayerController::onObstacleHit(const PxControllerObstacleHit& hit)
{
	int a = 0;
}

