#pragma once
#include <Engine/CScript.h>

// ���� �����ؼ� ������
class CAIScript :
    public CScript
{
private:
    State            m_State;

    Statevalue        m_StateValue;

public:
    void AddState();
    State FindState() { return m_State; }
    void ChangeState(State _State);

public:
    virtual void tick() override;
    void exit();

public:
    void IDLE();
    void TRACE();
    void DEAD();



public:
    CLONE(CAIScript);
public:
    CAIScript();
    CAIScript(const CAIScript& _origin);
    ~CAIScript();
};

