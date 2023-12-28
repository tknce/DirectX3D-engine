#pragma once
#include "CComponent.h"
class CRigidBody2D :
    public CComponent
{
private:
    Vec2    m_vForce;               // ��
    Vec2    m_vAccel;               // ������
    Vec2    m_vVelocity;            // �ӵ�

    float   m_fMass;                // ����
    float   m_fFriction;            // ����
    float   m_fMaxSpeed;            // �߷��� �ƴ� �ִ� �ӷ� ����
    float   m_fMaxGravitySpeed;     // �߷¿� ���� �߻��ϴ� �ִ� �ӷ� ����

    Vec2    m_vGravityAccel;        // �߷� ���ӵ�
    bool    m_bUseGravity;          // �߷� ����
    bool    m_bGround;              // ���� ���ִ��� Ȯ��
    bool    m_bWall;                // ���� �پ��ִ��� Ȯ��
    bool    m_bWall_nomal;
    bool    m_bstair;   

public:
    virtual void finaltick() override;


public:
    void AddForce(Vec2 _vF) { m_vForce += _vF; }
    void AddVelocity(Vec2 _vVelocity) { m_vVelocity += _vVelocity; }
    void SetVelocity(Vec2 _vVelocity) { m_vVelocity = _vVelocity; }
    

    void SetMass(float _fMass) { m_fMass = _fMass; }
    void SetMaxSpeed(float _fMax) { m_fMaxSpeed = _fMax; }
    void SetMaxGravitySpeed(float _fMax) { m_fMaxGravitySpeed = _fMax; }
    void SetGravity(bool _bSet) { m_bUseGravity = _bSet; }
    void SetGravityAccel(Vec2 _vAccel) { m_vGravityAccel = _vAccel; }
    void SetGround(bool _bGround) { m_bGround = _bGround; }
    void SetFriction(float _Friction) { m_fFriction = _Friction; }
    void SetWall(bool _bWall) { m_bWall = _bWall; }
    void SetWall_namal(bool _bWall_nomal) { m_bWall_nomal = _bWall_nomal; }
    void SetStair(bool _bstair) { m_bstair = _bstair; }

    bool isGround() { return m_bGround; }

    Vec2   GetForce()           { return m_vForce; }
    Vec2   GetAccel()           {return m_vAccel;}
    Vec2   GetVelocity()        { return m_vVelocity; }
    Vec2   GetGravityAccel()    { return m_vGravityAccel; }
    float  GetMass()            { return m_fMass; }
    float  GetFriction()        {return m_fFriction;}
    float  GetMaxSpeed()        {return m_fMaxSpeed;}
    float  GetMaxGravitySpeed() {return  m_fMaxGravitySpeed;}
    bool   GetUseGravity()      {return m_bUseGravity;}
    bool   GetWall() { return m_bWall; }
    bool   GetWall_namal() { return m_bWall_nomal; }
    bool   GetStair() { return m_bstair; }
    
    virtual void SaveToFile(FILE* _File);
    virtual void LoadFromFile(FILE* _File);

   
    CLONE(CRigidBody2D);

public:
    CRigidBody2D();
    ~CRigidBody2D();
};

