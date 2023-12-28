#pragma once
#include "CComponent.h"
#include "CRigidBody3D.h"

class CTransform :
	public CComponent
{
private:
	Vec3        m_vRelativePos;
	Vec3        m_vRelativeScale;
	Vec3        m_vRelativeRotation;

	Vec3		m_vRelativeDir[(UINT)DIR::END];
	Vec3		m_vWorldDir[(UINT)DIR::END];

	Matrix		m_matRelativeRot;
	Matrix		m_matRegidRot;
	Matrix		m_matWorld;
	Matrix		m_matWorldInv;

	Matrix		m_matSocket;

	CGameObject* m_pSocket;

	bool		m_bIgnParentScale;

public:
	void SetRelativePos(Vec3 _vPos) { 	m_vRelativePos = _vPos; }
	void SetRelativeScale(Vec3 _vScale) { m_vRelativeScale = _vScale; }
	void SetRelativeRotation(Vec3 _vRotation) { m_vRelativeRotation = _vRotation; }
	void SetRelativeRotate(Vec3 _vRotation) { m_vRelativeRotation += _vRotation; }
	void SetMatRot(Matrix _matRot) { m_matRelativeRot = _matRot; }
	void SetMatrigidRot(Matrix _matRot) { m_matRegidRot = _matRot; }

	void SetRelativePos(float _x, float _y, float _z) 	{ m_vRelativePos = Vec3(_x, _y, _z); }
	void SetRelativeScale(float _x, float _y, float _z) { m_vRelativeScale = Vec3(_x, _y, _z); }
	void SetRelativeRotation(float _x, float _y, float _z) { m_vRelativeRotation = Vec3(_x, _y, _z); }
	void SetRelativeRotationX(float _x) { m_vRelativeRotation.x = _x; }
	void SetRelativeRotationY(float _y) { m_vRelativeRotation.y = _y; }
	void SetRelativeRotationZ(float _z) { m_vRelativeRotation.z = _z; }
	void SetRelativeRotate(float _x, float _y, float _z) { m_vRelativeRotation += Vec3(_x, _y, _z); }
	void SetSocket(CGameObject* _socket);

	void AddRelativePos(Vec3 _vPos) { m_vRelativePos += _vPos; }

	CGameObject* GetSocket() { return m_pSocket; }

	Vec3 GetRelativePos() { return	m_vRelativePos; }
	Vec3 GetWorldPos() { return m_matWorld.Translation(); }

	Vec3 GetRelativeScale() { return m_vRelativeScale; }
	Matrix GetWorldRotation();
	Vec3 GetWorldScale();

	Vec3 GetRelativeRotation() { return m_vRelativeRotation; }
	Vec3 GetRelativeDir(DIR _eType) { return m_vRelativeDir[(UINT)_eType]; }
	Vec3 GetWorldDir(DIR _eType) { return m_vWorldDir[(UINT)_eType]; }

	void SetIgnoreParentScale(bool _bIgn) { m_bIgnParentScale = _bIgn ;}
	bool IsIgnoreParentScale() { return m_bIgnParentScale; }

	const Matrix& GetWorldMat() { return m_matWorld; }
	const Matrix& GetWorldInvMat() { return m_matWorldInv; }

public:
	virtual void tick() override;
	virtual void finaltick() override;

	void UpdateData();

public:
	virtual void SaveToFile(FILE* _File);
	virtual void LoadFromFile(FILE* _File);

	void SaveTurnBackTime(FILE* _File);
	void LoadTurnBackTime(FILE* _File);

	CLONE(CTransform);
public:
	CTransform();
	CTransform(const CTransform& _origin);
	virtual ~CTransform();
};

 