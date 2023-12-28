#include "pch.h"
#include "RaycastMgr.h"
#include "CRayCast.h"

RaycastMgr::RaycastMgr()
{
}

RaycastMgr::~RaycastMgr()
{
	for (int i = 0; i < m_vecRegisterdObject.size(); ++i)
	{
		m_vecRegisterdObject[i]._actor = nullptr;
		m_vecRegisterdObject[i]._shape = nullptr;
	}
}


bool RaycastMgr::RayCast(CRayCast* ray)
{
	PxVec3 O = PxVec3(ray->m_Origin.x, ray->m_Origin.y, ray->m_Origin.z);
	PxVec3 D = PxVec3(ray->m_Direction.x, ray->m_Direction.y, ray->m_Direction.z);


	PxRaycastBuffer buffer;
	bool Collision = m_Scene->raycast(O, D, ray->m_MaxDistance, buffer);

	//충돌한 카운터개수
	ray->m_Hit.HitCount = buffer.getNbAnyHits();
	PxActor* hanjoo = reinterpret_cast<PxActor*>(buffer.block.actor);

	ray->m_Hit.HitPoint.x = buffer.block.position.x;
	ray->m_Hit.HitPoint.y = buffer.block.position.y;
 	ray->m_Hit.HitPoint.z = buffer.block.position.z;

	ray->m_Hit.FaceIndex = buffer.block.faceIndex;

	return Collision;
}

bool RaycastMgr::RayCastMultiple(CRayCast* ray)
{
	PxVec3 O = PxVec3(ray->m_Origin.x, ray->m_Origin.y, ray->m_Origin.z);
	PxVec3 D = PxVec3(ray->m_Direction.x, ray->m_Direction.y, ray->m_Direction.z);

	const PxU32 bufferSize = 100;
	PxRaycastHit hitbuffer[bufferSize];
	PxRaycastBuffer buf(hitbuffer, bufferSize);


	bool Collision = m_Scene->raycast(O, D, ray->m_MaxDistance, buf);

	for (PxU32 i = 0; i < buf.nbTouches; i++)
	{
		buf.getTouch(i).position;
	}

	return Collision;
}

bool RaycastMgr::registerRaycastCCDObject(PxRigidDynamic* actor, PxShape* shape)
{
	if (!actor || !shape)
		return false;

	// Store the registered object in a container or data structure for later use
	RaycastCCDObject raycastObject;
	raycastObject._actor = actor;
	raycastObject._shape = shape;
	m_vecRegisterdObject.push_back(raycastObject);

	return true;
}

void RaycastMgr::doRaycastCCD(bool doDynamicDynamicCCD)
{
	// Perform raycast CCD for each registered object

	for (const RaycastCCDObject& raycastObject : m_vecRegisterdObject)
	{
		
		PxRigidDynamic* actor = raycastObject._actor;
		PxShape* shape = raycastObject._shape;

		// Retrieve object properties (e.g., position, velocity)
		PxTransform pose = actor->getGlobalPose();
		PxVec3 velocity = actor->getLinearVelocity();

		// Perform raycast and adjust object position if necessary
		if (performRaycastCCD(pose, shape, velocity, doDynamicDynamicCCD))
		{
			actor->setGlobalPose(pose);
			actor->setLinearVelocity(velocity);
		}
	}
}

bool RaycastMgr::performRaycastCCD(PxTransform& pose, PxShape* shape, PxVec3& velocity, bool doDynamicDynamicCCD)
{
	// Perform raycast CCD calculations for the given object
		// Adjust the object's pose and velocity if necessary to prevent tunneling or interpenetration
		// You would need to implement the raycast CCD algorithm here

		// Example implementation:
		// Apply a simple displacement based on the velocity and time step
	const float timeStep = 1.0f / 60.0f;
	pose.p += velocity * timeStep;

	return true; // Return true if an adjustment was made, false otherwise
}

void RaycastMgr::removeActor(PxActor* actor)
{

	for (int i = 0; i < m_vecRegisterdObject.size(); ++i)
	{
		if (actor->getName() == m_vecRegisterdObject[i]._actor->getName())
		{
			m_vecRegisterdObject.erase(m_vecRegisterdObject.begin() + i);
		}
	}
}