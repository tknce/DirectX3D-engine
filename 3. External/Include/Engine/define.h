#pragma once

typedef Vector2 Vec2;
typedef Vector3 Vec3;
typedef Vector4 Vec4;

#define DT CTimeMgr::GetInst()->GetDeltaTime()

#define DEVICE CDevice::GetInst()->GetDevice()
#define CONTEXT CDevice::GetInst()->GetContext()

#define KEY_PRESSED(key)	CKeyMgr::GetInst()->GetKeyState(key) == KEY_STATE::PRESS
#define KEY_TAP(key)		CKeyMgr::GetInst()->GetKeyState(key) == KEY_STATE::TAP
#define KEY_RELEASE(key)	CKeyMgr::GetInst()->GetKeyState(key) == KEY_STATE::RELEASE

#define CLONE(type) public: type* Clone() { return new type(*this); }
#define CLONE_DUMMY(type) public: type* Clone() { return nullptr; }
#define CLONE_ASSERT(type) public: type* Clone() { assert(nullptr); return nullptr; }

#define SAFE_DELETE(p) if(nullptr != p) delete p; p = nullptr;

#define MAX_LAYER 32
#define PX_RELEASE(x)	if(x)	{ x->release(); x = NULL;	}
#define VAILD(x) if(x == nullptr) { assert(nullptr);}
enum class SAMPLER_TYPE
{
	ANISOTROPIC, // 이방성
	POINT,		 

	END,
};

enum class CB_TYPE
{
	TRANSFORM,	 // b0
	MATERIAL,	 // b1
	GLOBAL,	     // b2
	ANIMATION2D, // b3

	END,
};

// Rasterizer State
enum class RS_TYPE
{
	CULL_BACK,
	CULL_FRONT,
	CULL_NONE,
	WIRE_FRAME,
	END,
};

enum class DS_TYPE
{
	LESS,				// Less, Depth Write
	LESS_EQUAL,			// Less, Depth Write
	GREATER,			// Greater, Depth Write
	NO_WRITE,			// Less, Depth No Write
	NO_TEST_NO_WRITE,	// NoTest, Depth No Write

	END,
};

// BlendState Type
enum class BS_TYPE
{
	DEFAULT,
	ALPHABLEND,
	ONE_ONE,

	END,
};


enum PIPELINE_STAGE
{
	VS = 0x01,
	HS = 0x02,
	DS = 0x04,
	GS = 0x08,
	PS = 0x10,
	ALL_STAGE = VS | HS | DS | GS | PS,
};

enum class SHADER_DOMAIN
{
	DOMAIN_DEFERRED_OPAQUE, // 지연 렌더링
	DOMAIN_DEFERRED_MASK, // 지연 렌더링
	DOMAIN_DEFERRED_DECAL,		// 지연처리 데칼

	DOMAIN_OPAQUE,		// 불투명 단계
	DOMAIN_MASK,		// 출력, 비출력

	DOMAIN_DECAL,				// 데칼 처리
	DOMAIN_TRANSPARENT, // 반투명

	DOMAIN_POST_PROCESS,// 후처리	

	// Engine 차원에서 처리
	DOMAIN_LIGHT,				// 광원처리
	NONE,
};

enum class RES_TYPE
{
	PREFAB,
	MESHDATA,
	COMPUTE_SHADER,

	MATERIAL,
	MESH,
	TEXTURE,	
	SOUND,
	GRAPHICS_SHADER,
	
	END,
};

extern const char* RES_TYPE_CHAR[(UINT)RES_TYPE::END];
extern const wchar_t* RES_TYPE_WCHAR[(UINT)RES_TYPE::END];

enum class COMPONENT_TYPE
{
	TRANSFORM,
	CAMERA,
	COLLIDER2D,
	COLLIDER3D,
	ANIMATOR2D,
	ANIMATOR3D,
	LIGHT2D,
	LIGHT3D,
	RIGIDBODY2D,
	RIGIDBODY3D,
	
	// render
	MESHRENDER,
	TILEMAP,
	PARTICLESYSTEM,
	SKYBOX,
	DECAL,
	LANDSCAPE,
	AI,

	END,

	SCRIPT,
};

extern const char* COMPONENT_TYPE_CHAR[(UINT)COMPONENT_TYPE::END];
extern const wchar_t* COMPONENT_TYPE_WCHAR[(UINT)COMPONENT_TYPE::END];

enum class EVENT_TYPE
{
	CREATE_OBJECT,  // wParam : Object Adress, lParam : Layer Index
	DELETE_OBJECT,	// wParam : Object Adress,
	ADD_CHILD,		// wParam : Child Adress, IParam : Parent Adress
	ADD_RES,		// wParam : RES_TYPE, lParam : Resource Adress
	DELETE_RES,
	CHANGE_LEVEL,
	CHANGE_LEVEL_STATE, // wParam : Next Level State
	DELETE_COMPONENT,	// wParam : Object Adress, IParam : COMPONETN_TYPE
	CHANGE_LAYER_IDX,	// wParam : Object Adress, IParam : LayerIdx	
	END,
};

enum class DEBUG_SHAPE
{
	// 2D
	RECT,
	CIRCLE,

	// 3D
	CUBE,
	SPHERE,
	CONE,

	END,
};


enum class COLLIDER2D_TYPE
{
	COLLIDER2D_RECT,
	COLLIDER2D_CIRCLE,
};



enum SCALAR_PARAM
{
	INT_0,
	INT_1,
	INT_2,
	INT_3,

	FLOAT_0,
	FLOAT_1,
	FLOAT_2,
	FLOAT_3,

	VEC2_0,
	VEC2_1,
	VEC2_2,
	VEC2_3,

	VEC4_0,
	VEC4_1,
	VEC4_2,
	VEC4_3,

	MAT_0,
	MAT_1,
	MAT_2,
	MAT_3,
};

enum TEX_PARAM
{
	TEX_0,
	TEX_1,
	TEX_2,
	TEX_3,
	TEX_4,
	TEX_5,
	TEX_6,
	TEX_7,

	TEX_CUBE_0,
	TEX_CUBE_1,

	TEX_ARR_0,
	TEX_ARR_1,

	TEX_END,
};


enum class DIR
{
	RIGHT,
	UP,
	FRONT,
	END,
};

enum class SB_TYPE
{
	SRV_ONLY,
	UAV_INC,
};

enum class LIGHT_TYPE
{
	DIRECTIONAL,	// 방향성
	POINT,			// 점광원
	SPOT,			// 스포트 라이트
};

enum class LEVEL_STATE
{
	PLAY,
	STOP,
	PAUSE,
};



enum class MONSTER_STATE_FIGHT
{
	IDLE, // 발견하고 공격
	WALK,
	WALK_REVERSE,
	TURN,
	TURN_REVERSE,
	RUN,
	RUN_REVERSE,
	HURTFLY,
	HURTFLY_REVERSE,
	HURTGROUND,
	HURTGROUND_REVERSE,
	ATTACK_ALMOST,
	END,
};

enum class State
{
	IDLE,
	TRACE,
	DEAD,
};

enum class MRT_TYPE
{
	SWAPCHAIN,
	DEFERRED,
	LIGHT,
	SHADOW,
	END,
};

enum class LANDSCAPE_MOD
{
	HEIGHT_MAP,
	SPLAT,
	NONE,
};
