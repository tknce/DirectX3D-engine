#pragma once
#include "PhysX\extensions\PxDefaultErrorCallback.h"

using namespace physx;
class UserErrorCallback :
    public PxDefaultErrorCallback
{
	virtual void reportError(PxErrorCode::Enum code, const char* message, const char* file, int line);
public:
	UserErrorCallback();
	~UserErrorCallback();

	
};

