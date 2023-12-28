#pragma once
#include "singleton.h"

union COLLIDER_ID
{
	struct {
		UINT LEFT_ID;
		UINT RIGHT_ID;
	};
	ULONGLONG ID;
};



class CLevel;
class CCollider2D;

class CCollisionMgr :
    public CSingleton<CCollisionMgr>
{
private:
	WORD					m_matrix[MAX_LAYER];
	map<ULONGLONG, bool>	m_mapColInfo;


public:
	void tick();


private:
	void CollisionBtwLayer(CLevel* _pCurScene, int _iLeft, int _iRight);
	void CollisionBtwCollider(CCollider2D* _pLeft, CCollider2D* _pRight);
	bool IsCollision(CCollider2D* _pLeft, CCollider2D* _pRight);

public:
	void clear() {
		memset(m_matrix, 0, sizeof(WORD) * MAX_LAYER);
		m_mapColInfo.clear();
	}
	void CollisionLayerCheck(int _iLeft, int _iRight);
	void CollisionLayerRelease(int _iLeft, int _iRight);
	WORD GetCollisionLayer(int _Idx) { return m_matrix[_Idx]; }

public:
	CCollisionMgr();
	~CCollisionMgr();
};

