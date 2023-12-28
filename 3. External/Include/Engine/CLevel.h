#pragma once

#include "CEntity.h"
#include "CLayer.h"

class CGameObject;

class CLevel :
    public CEntity 
{
private:
    CLayer          m_arrLayer[MAX_LAYER];
    LEVEL_STATE     m_State;

public:
    // 시점 함수
    void begin();
    void tick();
    void finaltick();
    
    void ClearLayer();

public:
    void SetState(LEVEL_STATE _State) { m_State = _State; }
    LEVEL_STATE GetState() { return m_State; }


    CLayer* GetLayer(UINT _iIdx) { return &m_arrLayer[_iIdx]; }
    CLayer* GetLayer(const wstring& _strLayerName);
    void GetGameObject(vector<vector<CGameObject*>>& _out);

    void AddGameObject(CGameObject* _pObject, UINT _iLayerIdx);
    void AddGameObject(CGameObject* _pObject, const wstring& _strLayerName);

    
    CGameObject* FindObjectByName(const wstring& _Name);
    CGameObject* FindObjectByID(int _ID);
    void FindObjectByName(const wstring& _Name, vector<CGameObject*>& _out);
    void FindObjectByWord(const wstring& _Name, vector<CGameObject*>& _out);
    void FindObjectByPos(CGameObject* _Pos, vector<CGameObject*>& _out);
public:
    CLONE_DUMMY(CLevel);

public:
    CLevel();
    virtual ~CLevel();
};

