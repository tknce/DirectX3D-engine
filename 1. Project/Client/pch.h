#pragma once

#include "framework.h"

#include <crtdbg.h>

#include <Engine\global.h>
#include <Engine\CEngine.h>

enum class SliceMode
{
    TILEMAP,
    SLICE,
    END,
};

#ifdef _DEBUG
#pragma comment(lib, "Engine\\Engine_debug.lib")
#else
#pragma comment(lib, "Engine\\Engine.lib")
#endif

#ifdef _DEBUG
#pragma comment(lib, "Script\\Script_debug.lib")
#else
#pragma comment(lib, "Script\\Script.lib")
#endif

// 물리엔진
#ifdef _DEBUG
#pragma comment(lib, "PhysEngine\\PhysEngine_x64d.lib")
#else
#pragma comment(lib, "PhysEngine\\PhysEngine.lib")
#endif

