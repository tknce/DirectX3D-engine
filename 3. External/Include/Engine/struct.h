#pragma once


struct Vtx
{
	Vec3 vPos;
	Vec4 vColor;
	Vec2 vUV;

	Vec3 vTangent;
	Vec3 vBinormal;
	Vec3 vNormal;
	
	Vec4 vWeights;
	Vec4 vIndices;
};


struct tEvent
{
	EVENT_TYPE	eType;
	DWORD_PTR	wParam;
	DWORD_PTR	lParam;
};

struct tLightInfo
{
	Vec4		vDiff;		// ���ݻ� ��
	Vec4		vSpec;		// �ݻ籤 ���
	Vec4		vAmb;		// ȯ�汤
	
	Vec4		vWorldPos;	// ���� ���� ��ġ
	Vec4		vWorldDir;	// ������ ����
	LIGHT_TYPE	iLightType;	// ���� Ÿ��
	float		fRadius;	// ���� �ݰ� (POINT, SPOT)
	float		fAngle;		// ���� ���� (SPOT)
	int			pad;
};

// Material ���
struct tMtrlData
{
	Vec4 vDiff;
	Vec4 vSpec;
	Vec4 vAmb;
	Vec4 vEmv;
};

// ���� ����ü
// �����̶�
// ���콺 ��ġ���� ī�޶� ���������� �Ÿ��� ���ϴ� ����
struct tRay
{
	Vec3 vStart;
	Vec3 vDir;
};

// Raycast ����� ���� ����ü
struct tRaycastOut
{
	Vec2 vUV;
	int  iDist;
	int  bSuccess;
};


// ================
// Animation ����ü
// ================
struct tAnim2DFrm
{
	Vec2 vLeftTop; 
	Vec2 vSlice;
	Vec2 vOffset;
	Vec2 vFullSize;
	float fDuration;
};

// ============
// Animation 3D
// ============
struct tFrameTrans
{
	Vec4	vTranslate;
	Vec4	vScale;
	Vec4	qRot;
};

struct tMTKeyFrame
{
	double	dTime;
	int		iFrame;
	Vec3	vTranslate;
	Vec3	vScale;
	Vec4	qRot;
};


struct tMTBone
{
	wstring				strBoneName;
	int					iDepth;
	int					iParentIndx;
	Matrix				matOffset;	// Offset ���(�� -> ��Ʈ ������ ���)
	Matrix				matBone;   // �̰� �Ⱦ�
	vector<tMTKeyFrame>	vecKeyFrame;
};

struct tMTAnimClip
{
	wstring			strAnimName;

	int				iStartFrame;
	int				iEndFrame;
	int				iFrameLength;

	double			dStartTime;
	double			dEndTime;
	double			dTimeLength;
	float			fUpdateTime; // �̰� �Ⱦ�

	FbxTime::EMode	eMode;
};



// ===========
// Instancing
// ===========
union uInstID
{
	struct {
		UINT iMesh;
		WORD iMtrl;
		WORD iMtrlIdx;
	};
	ULONG64 llID;
};

class CGameObject;
struct tInstObj
{
	CGameObject* pObj;
	UINT		 iMtrlIdx;
};

struct tInstancingData
{
	Matrix matWorld;
	Matrix matWV;
	Matrix matWVP;
	int    iRowIdx;
};





// ===============
// Particle ����ü
// ===============
struct tParticle
{
	Vec4 vRelativePos;
	Vec4 vDir;
		
	float fMaxTime;
	float fCurTime;
	float fSpeed;
	UINT  iActive;
};

struct tParticleShare
{
	UINT	iAliveCount;
	float	fSeta;
	Vec2	vVelopcity;
	Vec2	vForce;
	UINT	Padding[2];
};

// ==============
// TileMap ����ü
// ==============

struct tTile
{
	Vec2 vLeftTop;	// ��Ʋ���� �̹��� ���� �� ������ �������� �ڸ��� uv���� ����ؼ� �ڸ��� ���� ����     tex�̹��� ���μ��� ���̸� ����
	Vec2 vSlice;	// �ڸ� ũ��
};




// ================
// Debug Shape Info
// ================
struct tDebugShapeInfo
{
	DEBUG_SHAPE eShape;
	Vec4		vColor;
	Vec3		vPosition;
	Vec3		vScale;
	Vec3		vRot;
	float		fRadius;
	float		fDuration;
	float		fCurTime;
};


// ================
// Shader Parameter
// ================
struct tScalarParam
{
	SCALAR_PARAM eParam;
	string		 strName;
};

struct tTextureParam
{
	TEX_PARAM	eParam;
	string		strName;
};


// ================
// ������ۿ� ����ü
// ================
struct tTransform
{	
	Matrix matWorld;
	Matrix matWorldInv;
	Matrix matView;
	Matrix matViewInv;
	Matrix matProj;
	Matrix matProjInv;

	Matrix matWV;
	Matrix matWVP;
};

extern tTransform g_transform;

struct tMtrlConst
{
	tMtrlData mtrl;

	int iArr[4];
	float fArr[4];
	Vec2 v2Arr[4];
	Vec4 v4Arr[4];
	Matrix matArr[4];

	int	HasTex[(UINT)TEX_PARAM::TEX_END];

	int	arrAnimData[4];	// 3D Animation ����
};

struct tGlobalData
{
	Vec2	vRenderResolution;
	Vec2	vNoiseResolution;
	int		iLight2DCount;
	int		iLight3DCount;

	float	fAccTime;
	float	fDT;	
};

extern tGlobalData g_global;

struct tAnim2DInfo
{	
	Vec2 vLeftTop;
	Vec2 vSlice;
	Vec2 vOffset;
	Vec2 vFullSize;

	int iAnim2DUse;
	int iPadding[3];
};

struct tBGRA
{
	unsigned char b;
	unsigned char g;
	unsigned char r;
	unsigned char a;

	bool        check;
};

struct Statevalue
{
	bool enter;
	bool tick;
	bool exit;
};

struct STATE_VAL_IDLE
{
	bool enter;
	bool tick;
	bool exit;
};

struct STATE_VLA_TRACE
{
	bool enter;
	bool tick;
	bool exit;
};

struct tWeight_4
{
	float arrWeight[4];
};
struct Triangle
{
	UINT ind0, ind1, ind2;
};