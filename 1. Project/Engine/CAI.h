#pragma once
#include "CComponent.h"

class CState;

class AI :
    public CComponent
{
private:



public:
    void AddState(const wstring& _strKey, CState* _pState);
    CState* FindState(const wstring& _strKey);
    void ChangeState(const wstring& _strKey);

public:
    virtual void tick();
    virtual void finaltick();

    //virtual void SaveToFile(FILE* _File);
    //virtual void LoadFromFile(FILE* _File);

public:
    CLONE(AI);
public:
    AI();
    AI(const AI& _origin);
    ~AI();
};

