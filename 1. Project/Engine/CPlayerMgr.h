#pragma once
#include "singleton.h"
class CPlayerMgr :
    public CSingleton<CPlayerMgr>
{
private:
    int DEAD;

    bool m_breturn;
public:
    void SetDead(int _dead) { DEAD = _dead; } // 1이 죽음 상태
    int GetDead() { return DEAD; }
    void Setreturn(bool _ret) { m_breturn = _ret; }
    bool Getruturn() { return m_breturn; }
    CPlayerMgr();
    ~CPlayerMgr();
};

