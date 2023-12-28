#pragma once
#include "PhysEngineDLL.h"
#ifdef _DEBUG
#pragma comment(lib, "Engine\\Engine_debug.lib")
#else
#pragma comment(lib, "Engine\\Engine.lib")
#endif
#include <Engine\global.h>

//#include <Engine/global.h>
class PhysScene
{
public:
	PhysScene();
	~PhysScene();
private:
	DirectX::SimpleMath::Vector3 Grvity;
};

