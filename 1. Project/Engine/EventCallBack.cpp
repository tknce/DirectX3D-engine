#include "pch.h"
#include "EventCallBack.h"

void EventCallBack::onConstraintBreak(PxConstraintInfo* constraints, PxU32 count)
{
	int a = 0;
}

void EventCallBack::onWake(PxActor** actors, PxU32 count)
{
	int a = 0;
}

void EventCallBack::onSleep(PxActor** actors, PxU32 count)
{
	int a = 0;
	a += 5;
}

void EventCallBack::onContact(const PxContactPairHeader& pairHeader, const PxContactPair* pairs, PxU32 nbPairs)
{
	for (PxU32 i = 0; i < nbPairs; ++i) {
		const PxContactPair& cp = pairs[i];
	}
}

void EventCallBack::onTrigger(PxTriggerPair* pairs, PxU32 count)
{
	for (PxU32 i = 0; i < count; i++)
	{
		isTargetPush = false;
		isOtherPush = false;
		CGameObject* Other = reinterpret_cast<CGameObject*>(pairs[i].otherActor->userData);
		CGameObject* Target = reinterpret_cast<CGameObject*>(pairs[i].triggerActor->userData);

		if (pairs[i].status & PxPairFlag::eNOTIFY_TOUCH_FOUND)
		{
			int a = 0;
		}

		if (pairs[i].status & PxPairFlag::eNOTIFY_TOUCH_LOST)
		{
			int a = 0;
		}
	}
}


EventCallBack::EventCallBack()
{
}

EventCallBack::~EventCallBack()
{
}
