#pragma once
#include "CComponent.h"
class CRigidBody2D :
    public CComponent
{
private:
    Vec2    m_vForce;               // 힘
    Vec2    m_vAccel;               // 빠르기
    Vec2    m_vVelocity;            // 속도

    float   m_fMass;                // 질량
    float   m_fFriction;            // 마찰
    float   m_fMaxSpeed;            // 중력이 아닌 최대 속력 제한
    float   m_fMaxGravitySpeed;     // 중력에 의해 발생하는 최대 속력 제한

    Vec2    m_vGravityAccel;        // 중력 가속도
    bool    m_bUseGravity;          // 중력 적용
    bool    m_bGround;              // 땅에 서있는지 확인
    bool    m_bWall;                // 벽에 붙어있는지 확인
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

